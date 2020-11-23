#include <scenes/game.h>
#include <scenes/menu.h>

namespace epidemic {

using std::make_unique;

Menu::Menu(SceneManager* scene_manager) : View(scene_manager) {
}

void Menu::Update() {
  View::Update();
}

void Menu::Draw() {
  View::Draw();
}

void Menu::MouseDown(MouseEvent event) {
  manager_->SetScene(make_unique<Game>(manager_));
}

}  // namespace epidemic