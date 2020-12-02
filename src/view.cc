#include <CinderImGui.h>
#include <cinder/gl/gl.h>
#include <configuration.h>
#include <view.h>

namespace epidemic {

using ci::gl::clear;
using glm::vec2;
using ImGui::Begin;
using ImGui::End;

View::View(SceneManager* manager)
    : id_(GetNextID()), manager_(manager), with_im_gui_(false) {
}

View::View(SceneManager* manager, bool with_im_gui)
    : id_(GetNextID()), manager_(manager), with_im_gui_(with_im_gui) {
}

void View::Setup() {
}

void View::Update() {
}

void View::DrawExternal() {
  clear();
  if (with_im_gui_) {
    ImGui::SetNextWindowPos(vec2());
    ImGui::SetNextWindowSize(Configuration::kWindowSize);
    Begin("gui", nullptr,
          ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
  }
  Draw();
  if (with_im_gui_) {
    End();
  }
}

void View::MouseDown(MouseEvent event) {
}

void View::MouseUp(MouseEvent event) {
}

void View::MouseMove(MouseEvent event) {
}

void View::MouseDrag(MouseEvent event) {
}

size_t View::GetID() const {
  return id_;
}

bool View::operator==(const View& rhs) const {
  return id_ == rhs.id_;
}

bool View::operator!=(const View& rhs) const {
  return !(rhs == *this);
}

size_t View::kNextID = 0;
size_t View::GetNextID() {
  return kNextID++;
}

void View::Draw() {
}

}  // namespace epidemic