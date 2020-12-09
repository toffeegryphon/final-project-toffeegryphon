#pragma once

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <city.h>
#include <hand.h>
#include <isolation.h>
#include <scene_manager.h>

#include "view.h"

namespace epidemic {

using ci::app::App;
using cinder::app::MouseEvent;
using glm::vec2;

class EpidemicApp : public App {
 public:
  EpidemicApp();

  void setup() override;
  void update() override;
  void draw() override;
  void mouseDown(MouseEvent event) override;
  void mouseUp(MouseEvent event) override;
  void mouseMove(MouseEvent event) override;
  void mouseDrag(MouseEvent event) override;
  void keyDown(cinder::app::KeyEvent event) override;

 private:
  // https://stackoverflow.com/questions/57657546/how-to-downcast-in-c-to-call-a-child-function-from-a-parent-instance
  SceneManager manager_;
};

}  // namespace epidemic
