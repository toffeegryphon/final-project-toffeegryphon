#include <view.h>

namespace epidemic {

View::View(SceneManager* manager) : id_(GetNextID()), manager_(manager) {
}

void View::Setup() {
}

void View::Update() {
}

void View::Draw() {
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

}  // namespace epidemic