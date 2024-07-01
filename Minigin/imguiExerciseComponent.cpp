#include "imguiExerciseComponent.h"

namespace dae
{
    imguiExerciseComponent::imguiExerciseComponent() = default;
    imguiExerciseComponent::~imguiExerciseComponent() = default;

    void imguiExerciseComponent::Update() {}

    void imguiExerciseComponent::Render() const
    {
        RenderExercise1Window();
        RenderExercise2Window();
    }

    void imguiExerciseComponent::RenderExercise1Window() const
    {
        ImGui::Begin("Exercise 1");

        ImGui::InputInt("# samples", &inputValue);

        if (ImGui::Button("Trash the cache")) {
            timingsIntBuffer.clear();
            GameObjectBenchmark::RunIntBenchmark(inputValue, buffer_size, timingsIntBuffer);
        }

        if (!timingsIntBuffer.empty()) {
            ImGui::PlotLines("Int Time Table", timingsIntBuffer.data(), static_cast<int>(timingsIntBuffer.size()), 0, "Step Size", FLT_MIN, FLT_MAX, ImVec2(0, 200));
        }

        ImGui::End();
    }

    void imguiExerciseComponent::RenderExercise2Window() const
    {
        ImGui::Begin("Exercise 2");

        ImGui::InputInt("#samples", &inputValue);

        if (ImGui::Button("Trash the cache with GameObject3D")) {
            timingsGameObject3D.clear();
            GameObjectBenchmark::RunGameObjectBenchmark(inputValue, buffer_size, timingsGameObject3D);
        }

        if (!timingsGameObject3D.empty())
        {
            ImGui::PlotConfig conf;
            conf.values.ys = timingsGameObject3D.data();
            conf.values.count = static_cast<int>(timingsGameObject3D.size());
            conf.scale.min = -1;
            conf.scale.max = 700000;
            conf.tooltip.show = true;
            conf.tooltip.format = "x=%.2f, y=%.2f";
            conf.grid_x.show = true;
            conf.grid_y.show = true;
            conf.values.color = ImColor(0, 255, 0);
            conf.frame_size = ImVec2(400, 200);
            conf.line_thickness = 2.f;

            Plot("GameObject3D Timing Plot", conf);
        }

        if (ImGui::Button("Trash the cache with GameObject3DALT")) {
            timingsGameObject3DAlt.clear();
            GameObjectBenchmark::RunGameObjectaltBenchmark(inputValue, buffer_size, timingsGameObject3DAlt);
        }

        if (!timingsGameObject3DAlt.empty())
        {
            ImGui::PlotConfig conf;
            conf.values.ys = timingsGameObject3DAlt.data();
            conf.values.count = static_cast<int>(timingsGameObject3DAlt.size());
            conf.scale.min = -1;
            conf.scale.max = 700000;
            conf.tooltip.show = true;
            conf.tooltip.format = "x=%.2f, y=%.2f";
            conf.grid_x.show = true;
            conf.grid_y.show = true;
            conf.values.color = ImColor(255, 0, 0);
            conf.frame_size = ImVec2(400, 200);
            conf.line_thickness = 2.f;

            Plot("GameObject3DAlt Timing Plot", conf);
        }

        ImGui::End();
    }
}