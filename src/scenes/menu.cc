#include <CinderImGui.h>
#include <configuration.h>
#include <scenes/game.h>
#include <scenes/menu.h>
#include <utils.h>

namespace epidemic {

using cfg::BaseProperty;
using cfg::kProperties;
using cfg::VType;
using std::make_unique;
using utils::DrawIndividualInfo;

Menu::Menu(SceneManager* manager) : View(manager, true) {
}

void Menu::Draw() {
  ImGui::Text("Epidemic");
  if (ImGui::Button("start")) {
    cfg::NormalizeProperties();
    manager_->SetScene(make_unique<Game>(manager_));
  }

  DrawSettings();

  DrawIndividualInfo();
}

void Menu::DrawSettings() {
  for (BaseProperty* p : kProperties) {
    switch (p->v_type) {
      case VType::kInt: {
        auto* property = dynamic_cast<cfg::Property<int>*>(p);
        ImGui::SliderInt(property->label.c_str(), &property->value,
                         property->value_range.first,
                         property->value_range.second);
        if (property->label == "Population Size") {
          cfg::kStartingSickCount.value_range.second = property->value;
        }
        break;
      }
      case VType::kFloat: {
        auto* property = dynamic_cast<cfg::Property<float>*>(p);
        ImGui::SliderFloat(property->label.c_str(), &property->value,
                           property->value_range.first,
                           property->value_range.second,
                           property->format.c_str());
        break;
      }
      case VType::kVec2: {
        auto* property = dynamic_cast<cfg::Property<vec2>*>(p);
        // https://github.com/ocornut/imgui/issues/779
        ImGui::DragFloat2(
            property->label.c_str(), reinterpret_cast<float*>(&property->value),
            property->value_range.second.x, property->value_range.first.x,
            property->value_range.first.y, property->format.c_str(),
            property->value_range.second.y);
        break;
      }
      case VType::kBool: {
        auto* property = dynamic_cast<cfg::Property<bool>*>(p);
        ImGui::Checkbox(property->label.c_str(), &property->value);
        break;
      }
    }
  }
}

}  // namespace epidemic