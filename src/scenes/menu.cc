#include <CinderImGui.h>
#include <configuration.h>
#include <scenes/game.h>
#include <scenes/menu.h>

namespace epidemic {

using std::make_unique;

Menu::Menu(SceneManager* scene_manager) : View(scene_manager, true) {
}

void Menu::Update() {
  View::Update();
}

void Menu::Draw() {
  ImGui::Text("Epidemic");
  if (ImGui::Button("start")) {
    manager_->SetScene(make_unique<Game>(manager_));
  }

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
      default:
        break;
    }
  }
}

void Menu::MouseDown(MouseEvent event) {
}

}  // namespace epidemic