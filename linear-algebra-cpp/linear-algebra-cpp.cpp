#include <cmath>
#include <algorithm>
#include "raylib.h"
#include "imgui.h"
#include "rlImGui-main/rlImGui.h"
#include "nlohmann/json.hpp"
#include <fstream>
using json = nlohmann::json;

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
class App {
private:
    json settings_data;
public:
    App() {
        std::ifstream config_file("config.json");
        config_file >> settings_data;
        config_file.close();
    }
    enum STATE {
        START_SCREEN,
        SETTINGS_SCREEN,
        WORK_SCREEN,
        EXIT
    };

    void config_file_save(const auto& new_option) const {
        std::ofstream save_config_file("config.json");
        save_config_file << new_option;
        save_config_file.close();
    }
    auto get_settings_data() {
        return settings_data;
    }
};
using STATE = App::STATE;

int main()
{
    App app;

    mylinar::Vector3 vec = { 1, 5, 3 };
    mylinar::Vector3 vec2 = { 4, 2, 5 };

    InitWindow(1600, 900, "linear-algebra-visualisator");
    SetTargetFPS(60);
    rlImGuiSetup(true);
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoDecoration;

    Camera3D camera = { 0 };
    camera.position = { 20.0f, 15.0f, 5.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    json settings_data_now = app.get_settings_data();
    bool debug_mode = true;

    bool exit_pressed = false;
    STATE app_state = STATE::START_SCREEN;
    float radius = 15;
    float last_mouse_wheel_move = 0.0f;
    float horizontal_angle = 0.0;
    float vertical_angle = 0.0;
    while (!WindowShouldClose()) {
        BeginDrawing();

        auto mouse_wheel_move = GetMouseWheelMove();
        Color bgColor = (settings_data_now["Theme"] == "Dark") ? BLACK : WHITE;
        Color textColor = (settings_data_now["Theme"] == "Dark") ? WHITE : BLACK;

        ClearBackground(bgColor);
        if (settings_data_now["Theme"] == "Dark") {
            ImGui::StyleColorsDark();
        }
        else {
            ImGui::StyleColorsLight();
        }
        rlImGuiBegin();

        switch (app_state) {
            case STATE::START_SCREEN:
                ImGui::SetNextWindowPos({ (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 50 });
                ImGui::SetNextWindowSize({ 200, 200 });

                DrawText("WELCOME TO LINEAR ALGEBRA VISUALISATOR!", 450, 200, 30, textColor);
                ImGui::Begin("Main menu", nullptr, window_flags);

                if (ImGui::Button("Start", { 180, 40 })) {
                    app_state = STATE::WORK_SCREEN;
                }
                if (ImGui::Button("Settings", { 180, 40 })) {
                    app_state = STATE::SETTINGS_SCREEN;
                }
                if (ImGui::Button("Exit", { 180, 40 })) {
                    app_state = STATE::EXIT;
                }
                ImGui::End();

                DrawText("Author: DanSynko", 1400, 850, 20, textColor);
                break;

            case STATE::SETTINGS_SCREEN:
                ImGui::SetNextWindowPos({ (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 50 });
                ImGui::SetNextWindowSize({ 200, 200 });

                ImGui::Begin("Settings menu", nullptr, window_flags);

                if (ImGui::Button("Dark/Light Theme", { 180, 40 })) {
                    if (settings_data_now["Theme"] == "Light") {
                        ImGui::StyleColorsDark();
                        settings_data_now["Theme"] = "Dark";
                        app.config_file_save(settings_data_now);
                    }
                    else if (settings_data_now["Theme"] == "Dark") {
                        ImGui::StyleColorsLight();
                        settings_data_now["Theme"] = "Light";
                        app.config_file_save(settings_data_now);
                    }
                }
                if (ImGui::Button("Back to menu", { 180, 40 })) {
                    app_state = STATE::START_SCREEN;
                }
                ImGui::End();

                break;

            case STATE::WORK_SCREEN:
                if (mouse_wheel_move != 0) {
                    last_mouse_wheel_move = mouse_wheel_move;
                    Vector3 direct_vec = { (camera.target.x - camera.position.x), (camera.target.y - camera.position.y), (camera.target.z - camera.position.z) };
                    float length = sqrt((direct_vec.x * direct_vec.x) + (direct_vec.y * direct_vec.y) + (direct_vec.z * direct_vec.z));
                    direct_vec.x /= length;
                    direct_vec.y /= length;
                    direct_vec.z /= length;
                    radius -= mouse_wheel_move * 8.5;
                    radius = std::clamp(radius, 0.1f, 100.0f);
                    if (radius != 100) {
                        camera.position.x += direct_vec.x * (mouse_wheel_move * 8.5);
                        camera.position.y += direct_vec.y * (mouse_wheel_move * 8.5);
                        camera.position.z += direct_vec.z * (mouse_wheel_move * 8.5);
                    }
                }
                if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    Vector2 delta = GetMouseDelta();
                    horizontal_angle += delta.x * 0.01f;
                    vertical_angle += delta.y * 0.01f;
                    if (horizontal_angle != 0.0f && vertical_angle != 0.0f) {
                        camera.position.x = radius * sin(vertical_angle) * cos(horizontal_angle);
                        camera.position.y = radius * cos(vertical_angle);
                        camera.position.z = radius * sin(vertical_angle) * sin(horizontal_angle);
                    }
                }
                BeginMode3D(camera);
                DrawGrid(10, 1.0f);
                DrawLine3D({ 0,0,0 }, { vec.x, vec.y, vec.z }, RED);
                DrawLine3D({ 0,0,0 }, { vec2.x, vec2.y, vec2.z }, GREEN);

                mylinar::Vector3 cross_product_vec = vec.cross_product(vec2);
                DrawLine3D({ 0,0,0 }, { cross_product_vec.x, cross_product_vec.y, cross_product_vec.z }, BLUE);
                EndMode3D();

                ImGui::SetNextWindowPos({ 10.0f, 10.0f });
                ImGui::SetNextWindowSize({ 200, 250 });
                ImGui::Begin("Tools", nullptr, window_flags);

                    ImGui::Text("Red Vector");
                    ImGui::PushID("RedVector");
                        ImGui::SliderFloat("X", &vec.x, -10, 10);
                        ImGui::SliderFloat("Y", &vec.y, -10, 10);
                        ImGui::SliderFloat("Z", &vec.z, -10, 10);
                    ImGui::PopID();

                    ImGui::Separator();

                    ImGui::Text("Green Vector");
                    ImGui::PushID("GreenVector");
                        ImGui::SliderFloat("X", &vec2.x, -10, 10);
                        ImGui::SliderFloat("Y", &vec2.y, -10, 10);
                        ImGui::SliderFloat("Z", &vec2.z, -10, 10);
                    ImGui::PopID();

                    ImGui::Separator();

                    if (ImGui::Button("Back to menu.")) {
                        app_state = STATE::START_SCREEN;
                    }

                ImGui::End();
                
                ImGui::SetNextWindowPos({ 1450.0f, 10.0f });
                ImGui::SetNextWindowSize({ 125, 25 });
                ImGui::Begin("Debug mode", nullptr, window_flags);
                    ImGui::Checkbox("Debug mode", &debug_mode);
                ImGui::End();
                if (debug_mode) {
                    DrawText(TextFormat("mouse_delta_x: %.2f", GetMouseDelta().x), 500, 20, 20, YELLOW);
                    DrawText(TextFormat("mouse_delta_y: %.2f", GetMouseDelta().y), 500, 40, 20, YELLOW);
                    DrawText(TextFormat("horizontal_angle(pfi): %.2f", horizontal_angle), 500, 60, 20, YELLOW);
                    DrawText(TextFormat("vertical_angle(theta): %.2f", vertical_angle), 500, 80, 20, YELLOW);
                    DrawText(TextFormat("mouse_wheel_move: %.2f", last_mouse_wheel_move), 500, 100, 20, YELLOW);
                    DrawText(TextFormat("radius: %.2f", radius), 500, 120, 20, YELLOW);
                    DrawText(TextFormat("camera_position_x: %.2f", camera.position.x), 500, 140, 20, YELLOW);
                    DrawText(TextFormat("camera_position_y: %.2f", camera.position.y), 500, 160, 20, YELLOW);
                    DrawText(TextFormat("camera_position_z: %.2f", camera.position.z), 500, 180, 20, YELLOW);
                }

                DrawText("work_mode", 300, 20, 20, YELLOW);

                DrawText("Hold down RMB to look around the scene. Scroll the mouse wheel to zoom in/out.", 10, 880, 18, YELLOW);
                break;
            case STATE::EXIT:
                exit_pressed = true;
                break;
        }

        rlImGuiEnd();
        EndDrawing();

        if (exit_pressed) break;
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}