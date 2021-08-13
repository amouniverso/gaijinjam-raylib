#include <raylib.h>
#include <stdlib.h>

#define MAX_BUNNIES        50000    // 50K bunnies limit
#define MAX_BATCH_ELEMENTS  8192 // This is the maximum amount of elements (quads) per batch
#define NUM_FRAMES_PER_LINE     5
#define NUM_LINES               5

typedef struct Bunny {
    Vector2 position;
    Vector2 speed;
    Color color;
    bool killed = false;
    int id;
} Bunny;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int bunniesSpawnCount = 25;
static const int screenWidth = 1280;
static const int screenHeight = 720;

static const int ballRadius = 40;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Good day");

    Vector2 screenCenter = { (float)screenWidth / 2, (float)screenHeight / 2 };

    Texture2D texBunny = LoadTexture("resources/wabbit_alpha.png");
    Texture2D explosionTex = LoadTexture("resources/explosion.png");

    // Init variables for animation
    float frameWidth = explosionTex.width / NUM_FRAMES_PER_LINE;   // Sprite one frame rectangle width
    float frameHeight = explosionTex.height / NUM_LINES;           // Sprite one frame rectangle height
    int currentFrame = 0;
    int currentLine = 0;

    Rectangle explosionframeRec = { 0, 0, frameWidth, frameHeight };
    Vector2 explosionPosition = { 0.0f, 0.0f };

    bool isExplosionActive = false;
    int framesCounter = 0;

    int aliveCount = bunniesSpawnCount;

    Bunny* bunnies = (Bunny*)malloc(MAX_BUNNIES * sizeof(Bunny));    // Bunnies array

    int bunniesCount = 0;           // Bunnies counter

    // Create bunnies
    for (int i = 0; i < bunniesSpawnCount; i++)
    {
        if (bunniesCount < MAX_BUNNIES)
        {
            bunnies[bunniesCount].id = GetRandomValue(1245, 8543);
            bunnies[bunniesCount].position = GetMousePosition();
            bunnies[bunniesCount].speed.x = (float)GetRandomValue(-150, 150) / 60.0f;
            bunnies[bunniesCount].speed.y = (float)GetRandomValue(-150, 150) / 60.0f;
            bunnies[bunniesCount].color = {
                static_cast<unsigned char>(GetRandomValue(50, 240)),
                static_cast<unsigned char>(GetRandomValue(80, 240)),
                static_cast<unsigned char>(GetRandomValue(100, 240)), 255 };
            bunniesCount++;
        }
    }

    SetTargetFPS(120);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Update bunnies
        for (int i = 0; i < bunniesCount; i++)
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointCircle(GetMousePosition(), bunnies[i].position, ballRadius)) {
                    if (!isExplosionActive && !bunnies[i].killed) {
                        if (!bunnies[i].killed) {
                            isExplosionActive = true;
                            explosionPosition = GetMousePosition();
                            explosionPosition.x -= frameWidth / 2.0f;
                            explosionPosition.y -= frameHeight / 2.0f;
                        }
                        bunnies[i].killed = true;
                        aliveCount -= 1;
                    }
                }

            }

            bunnies[i].position.x += bunnies[i].speed.x;
            bunnies[i].position.y += bunnies[i].speed.y;

            if (((bunnies[i].position.x + texBunny.width / 2) > GetScreenWidth()) ||
                ((bunnies[i].position.x + texBunny.width / 2) < 0)) bunnies[i].speed.x *= -1;
            if (((bunnies[i].position.y + texBunny.height / 2) > GetScreenHeight()) ||
                ((bunnies[i].position.y + texBunny.height / 2 - 40) < 0)) bunnies[i].speed.y *= -1;
        }

        // Compute explosion animation frames
        if (isExplosionActive)
        {
            framesCounter++;

            if (framesCounter > 2)
            {
                currentFrame++;

                if (currentFrame >= NUM_FRAMES_PER_LINE)
                {
                    currentFrame = 0;
                    currentLine++;

                    if (currentLine >= NUM_LINES)
                    {
                        currentLine = 0;
                        isExplosionActive = false;
                    }
                }

                framesCounter = 0;
            }
        }

        explosionframeRec.x = frameWidth * currentFrame;
        explosionframeRec.y = frameHeight * currentLine;
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < bunniesCount; i++)
        {
            // NOTE: When internal batch buffer limit is reached (MAX_BATCH_ELEMENTS),
            // a draw call is launched and buffer starts being filled again;
            // before issuing a draw call, updated vertex data from internal CPU buffer is send to GPU...
            // Process of sending data is costly and it could happen that GPU data has not been completely
            // processed for drawing while new data is tried to be sent (updating current in-use buffers)
            // it could generates a stall and consequently a frame drop, limiting the number of drawn bunnies
            //DrawTexture(texBunny, (int)bunnies[i].position.x, (int)bunnies[i].position.y, bunnies[i].color);
            if (!bunnies[i].killed) {
                DrawCircleV(bunnies[i].position, ballRadius, bunnies[i].color);
                DrawText(TextFormat("JIRAID-%i", 
                    bunnies[i].id), bunnies[i].position.x - ballRadius, 
                    bunnies[i].position.y - 10, 20, BLACK);
            }
            if (isExplosionActive) {
                DrawTextureRec(explosionTex, explosionframeRec, explosionPosition, WHITE);
            }
            if (aliveCount == 0) {
                DrawText("It was a good day!", 50, screenCenter.y - 40, 100, LIME);
            }
        }

        DrawRectangle(0, 0, screenWidth, 40, BLACK);
        //DrawText(TextFormat("batched draw calls: %i", 1 + bunniesCount / MAX_BATCH_ELEMENTS), 320, 10, 20, MAROON);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(bunnies); 

    UnloadTexture(texBunny); 
    UnloadTexture(explosionTex);

    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}