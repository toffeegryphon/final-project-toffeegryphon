#include <epidemic_app.h>

namespace epidemic {

using ci::gl::clear;
using cinder::app::MouseEvent;

EpidemicApp::EpidemicApp() : city_(kCitySize) {
  srand(static_cast<unsigned int>(time(nullptr)));

  ci::app::setWindowSize(static_cast<int>(kWindowSize.x),
                         static_cast<int>(kWindowSize.y));
}

void EpidemicApp::update() {
  city_.Update();
}

void EpidemicApp::draw() {
  clear();
  city_.Draw(kCityOffset);
}

void EpidemicApp::mouseDown(MouseEvent event) {
  vec2 pos = event.getPos();
  // TODO binary search
}
void EpidemicApp::mouseUp(MouseEvent event) {
  AppBase::mouseUp(event);
}

}  // namespace epidemic
