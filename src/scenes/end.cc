#include <CinderImGui.h>
#include <cinder/gl/gl.h>
#include <configuration.h>
#include <scenes/end.h>
#include <scenes/menu.h>

namespace epidemic {

using glm::vec2;
using std::make_unique;
using std::string;
using std::to_string;

End::End(SceneManager* manager, const End::Data& data) : View(manager, true) {
  data_ = data;
}

void End::Draw() {
  ImGui::Text("%s", GetText().c_str());

  if (ImGui::Button("Return to Main Menu")) {
    manager_->SetScene(make_unique<Menu>(manager_));
    return;
  }
}

string End::GetText() {
  string text = (data_.recovered + data_.uninfected > data_.dead)
                    ? cfg::kEndWin
                    : cfg::kEndLose;
  text += "\nStats:\nUninfected: " + to_string(data_.uninfected) +
          "\nRecovered: " + to_string(data_.recovered) +
          "\nDead: " + to_string(data_.dead);
  return text;
}

}  // namespace epidemic