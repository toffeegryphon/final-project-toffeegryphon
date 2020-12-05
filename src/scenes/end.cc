#include <CinderImGui.h>
#include <cinder/gl/gl.h>
#include <scenes/end.h>
#include <scenes/menu.h>

namespace epidemic {

using glm::vec2;
using std::string;
using std::to_string;
using std::make_unique;

End::End(SceneManager* manager, const End::Data& data) : View(manager, true) {
  data_ = data;
}

void End::Draw() {
  // TODO Abstract, use format string
  string text = (data_.recovered + data_.uninfected > data_.dead)
                    ? "Congratulations! More recovered than died!"
                    : "Congratulations...?";
  text += "\nStats:\nUninfected: " + to_string(data_.uninfected) +
          "\nRecovered: " + to_string(data_.recovered) +
          "\nDead: " + to_string(data_.dead);
  ImGui::Text("%s", text.c_str());

  if (ImGui::Button("Return to Main Menu")) {
    manager_->SetScene(make_unique<Menu>(manager_));
    return;
  }
}

}  // namespace epidemic