#include <CinderImGui.h>
#include <configuration.h>
#include <epidemic_app.h>
#include <scenes/menu.h>

namespace epidemic {

using cfg::PopulateProperties;
using cinder::app::MouseEvent;
using cinder::app::KeyEvent;
using std::make_unique;

EpidemicApp::EpidemicApp() {
  srand(static_cast<unsigned int>(time(nullptr)));
  PopulateProperties();

  setWindowPos(100, 100);
  setWindowSize(static_cast<int>(cfg::kWindowSize.x),
                static_cast<int>(cfg::kWindowSize.y));

  manager_.SetScene(make_unique<Menu>(&manager_));
}

void EpidemicApp::setup() {
  ImGui::initialize();
  manager_.GetScene().Setup();
}

void EpidemicApp::update() {
  manager_.GetScene().Update();
}

void EpidemicApp::draw() {
  manager_.GetScene().DrawExternal();
}

void EpidemicApp::mouseDown(MouseEvent event) {
  manager_.GetScene().MouseDown(event);
}

void EpidemicApp::mouseUp(MouseEvent event) {
  manager_.GetScene().MouseUp(event);
}

void EpidemicApp::mouseMove(MouseEvent event) {
  manager_.GetScene().MouseMove(event);
}

void EpidemicApp::mouseDrag(MouseEvent event) {
  manager_.GetScene().MouseDrag(event);
}

void EpidemicApp::keyDown(KeyEvent event) {
  manager_.GetScene().KeyDown(event);
}

}  // namespace epidemic
