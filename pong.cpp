// PONG
#include <iostream>
#include "raylib.h"

using namespace std;

#define screen_width 1280
#define screen_height 800

Color Auburn = Color{ 158, 43, 37, 255 };
Color Earth_Yellow = Color{ 242, 192, 120, 255 };
Color Dutch_White = Color{ 250, 237, 202, 255 };
Color Vanilla = Color{ 250, 237, 177, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;
        void Draw() {
            DrawCircle(x, y, radius, WHITE);
        }

        void Update() {
            x += speed_x;
            y += speed_y;

            if (y + radius >= GetScreenHeight() || y - radius <= 0) {
                speed_y *= -1;
            }
            if (x + radius >= GetScreenWidth() || x - radius <= 0) {
                speed_x *= -1;
            }
            if (x - radius <= 0) {
                player_score++;
                ResetBall();
            }
            if (x + radius >= GetScreenWidth()) {
                cpu_score++;
                ResetBall();
            }
        }

        void ResetBall() {
            x = GetScreenWidth() / 2;
            y = GetScreenHeight() / 2;

            int speed_choices[2] = { -1, 1 };
            speed_x *= speed_choices[GetRandomValue(0, 1)];
            speed_y *= speed_choices[GetRandomValue(0, 1)];
        }
};


class Paddle {
public:
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangleRounded(Rectangle{ x, y, width, height }, 0.8, 0, Auburn);
    }

    void Update() {
        if (IsKeyDown(KEY_W) && y>0) {
            y -= speed;
        }
        if (IsKeyDown(KEY_S) && y+height<GetScreenHeight()){
            y += speed;
        }
    }
};

class CPUPaddle: public Paddle {
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {
            y -= speed;
        }
        if (y + height / 2 <= ball_y) {
            y += speed;
        }
    }
};

CPUPaddle cpu;
Paddle player;
Ball ball;

int main()
{
    InitWindow(screen_width, screen_height, "Bing Pong");
    SetTargetFPS(90);

    // Ball
    ball.radius = 25;
    ball.x = screen_width / 2;
    ball.y = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Player
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // CPU
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = 340;
    cpu.speed = 6;

    while (!WindowShouldClose()) {
        BeginDrawing();

        //Updating
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //Draw everything
        ClearBackground(Vanilla);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Earth_Yellow);
        DrawLine(640, 0, 640, 800, GRAY);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width / 4 - 20, 20, 80, WHITE);

        //Check for collision
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height })) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height })) {
            ball.speed_x *= -1;
        }



        EndDrawing();
    }
    CloseWindow();
    return 0;
}

