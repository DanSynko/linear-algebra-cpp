#include <string>
#include "raylib.h"

namespace mylinar {

    struct Vector3 {
        using component = float;
        component x, y, z;

        float get_length() const {
            return sqrt((x * x) + (y * y) + (z * z));
        }


        const Vector3 operator+(const Vector3& vec) const {
            return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
        }
        const Vector3 operator-(const Vector3& vec) const {
            return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
        }


        float dot_product(const Vector3& other_vec) const {
            return (this->x * other_vec.x) + (this->y * other_vec.y) + (this->z * other_vec.z);
        }
        Vector3 cross_product(const Vector3& vec) {
            return Vector3(
                this->y * vec.z - this->z * vec.y,
                this->z * vec.x - this->x * vec.z,
                this->x * vec.y - this->y * vec.x
            );
        }
        void normalize() {
            float length = get_length();
            x /= length;
            y /= length;
            z /= length;
        }
    };
};

int main()
{
    mylinar::Vector3 vec = { 1, 5, 3 };
    mylinar::Vector3 vec2 = { 4, 2, 5 };

    InitWindow(1600, 900, "linear-algebra-cpp visualisation");

    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {


        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode3D(camera);
                DrawGrid(10, 1.0f);

                DrawLine3D( { 0.0f, 0.0f, 0.0f }, { vec.x, vec.y, vec.z }, RED);
                DrawLine3D({ 0.0f, 0.0f, 0.0f }, { vec2.x, vec2.y, vec2.z }, GREEN);
                mylinar::Vector3 cross_product_vec = vec.cross_product(vec2);
                DrawLine3D({ 0.0f, 0.0f, 0.0f }, { cross_product_vec.x, cross_product_vec.y, cross_product_vec.z }, BLUE);

            EndMode3D();

            int xPos = 20;
            int yPos = 20;
            int fontSize = 20;
            int lineSpacing = 25;

            DrawText(TextFormat("Red Vector:    { %.2f, %.2f, %.2f }", vec.x, vec.y, vec.z), xPos, yPos, fontSize, RED);
            DrawText(TextFormat("Green Vector:  { %.2f, %.2f, %.2f }", vec2.x, vec2.y, vec2.z), xPos, yPos + lineSpacing, fontSize, GREEN);

            DrawText("------------------------------------------", xPos, yPos + lineSpacing * 2, fontSize, GRAY);

            float dot = vec.dot_product(vec2);
            DrawText(TextFormat("Dot Product:  %.2f", dot), xPos, yPos + lineSpacing * 3, fontSize, YELLOW);

        EndDrawing();

    }

    CloseWindow();

    return 0;
}