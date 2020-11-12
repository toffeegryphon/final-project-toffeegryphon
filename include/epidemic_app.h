#pragma once

#include <cinder/gl/gl.h>
#include <cinder/app/App.h>
#include <city.h>
#include <unordered_set>

namespace epidemic {

using ci::app::App;
using glm::vec2;
using std::unordered_set;
using cinder::app::MouseEvent;

class EpidemicApp : public App {
 public:
  EpidemicApp();

  void update() override;
  void draw() override;
  void mouseDown(MouseEvent event) override;
  void mouseUp(MouseEvent event) override;

 private:
  const vec2 kWindowSize{800, 800};
  const vec2 kCitySize{500, 500};
  const vec2 kCityOffset{20, 20};
  City city_;
  unordered_set<Individual*> grabbed_;
};

}  // namespace epidemic
