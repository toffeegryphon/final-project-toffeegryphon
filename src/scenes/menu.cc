#include <CinderImGui.h>
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
}

void Menu::MouseDown(MouseEvent event) {
}

}  // namespace epidemic