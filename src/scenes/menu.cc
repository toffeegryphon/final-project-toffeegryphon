#include <CinderImGui.h>
#include <configuration.h>
#include <scenes/game.h>
#include <scenes/menu.h>

namespace epidemic {

using std::make_unique;

Menu::Menu(SceneManager* manager) : View(manager, true) {
}

void Menu::Draw() {
  ImGui::Text("Epidemic");
  if (ImGui::Button("start")) {
    manager_->SetScene(make_unique<Game>(manager_));
  }

  DrawSettings();
}

void Menu::DrawSettings() {
  for (Configuration::BaseProperty* p : Configuration::kProperties) {
    switch (p->v_type) {
      case Configuration::VType::kInt: {
        auto* property = dynamic_cast<Configuration::Property<int>*>(p);
        ImGui::SliderInt(property->label.c_str(), &property->value,
                         property->value_range.first,
                         property->value_range.second);
        break;
      }
      case Configuration::VType::kFloat: {
        auto* property = dynamic_cast<Configuration::Property<float>*>(p);
        ImGui::SliderFloat(property->label.c_str(), &property->value,
                           property->value_range.first,
                           property->value_range.second,
                           property->format.c_str());
        break;
      }
      case Configuration::VType::kVec2: {
        auto* property = dynamic_cast<Configuration::Property<vec2>*>(p);
        // https://github.com/ocornut/imgui/issues/779
        ImGui::DragFloat2(
            property->label.c_str(), reinterpret_cast<float*>(&property->value),
            property->value_range.second.x, property->value_range.first.x,
            property->value_range.first.y, property->format.c_str(),
            property->value_range.second.y);
        break;
      }
      case Configuration::VType::kBool: {
        auto* property = dynamic_cast<Configuration::Property<bool>*>(p);
        ImGui::Checkbox(property->label.c_str(), &property->value);
        break;
      }
    }
  }
}

}  // namespace epidemic