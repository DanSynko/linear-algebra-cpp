#include <cmath>
#include <string>
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
    camera.position = { 10.0f, 10.0f, 10.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    json settings_data_now = app.get_settings_data();

    bool exit_pressed = false;
    STATE app_state = STATE::START_SCREEN;
    while (!WindowShouldClose()) {


        BeginDrawing();
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
                BeginMode3D(camera);
                DrawGrid(10, 1.0f);
                DrawLine3D({ 0,0,0 }, { vec.x, vec.y, vec.z }, RED);
                DrawLine3D({ 0,0,0 }, { vec2.x, vec2.y, vec2.z }, GREEN);

                mylinar::Vector3 cross_product_vec = vec.cross_product(vec2);
                DrawLine3D({ 0,0,0 }, { cross_product_vec.x, cross_product_vec.y, cross_product_vec.z }, BLUE);
                EndMode3D();

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

                DrawText("work_mode", 20, 20, 20, YELLOW);

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