#pragma once

#include <cinder/app/App.h>
#include <cinder/gl/gl.h>
#include <city.h>
#include <hand.h>
#include <isolation.h>

namespace epidemic {

using ci::app::App;
using cinder::app::MouseEvent;
using glm::vec2;
using std::pair;

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

 private:
  const vec2 kWindowSize{800, 800};
  const Location::Data kCityData{vec2{500, 500}, vec2{20, 20}};
  City city_;

  // TODO Vector tie to each Isolation
  const Location::Data kTemplateIsolationData{vec2{200, 100}, vec2{20, 20}};
  // TODO Maybe isolation manager
  vector<pair<Isolation, Location::Data>> isolations_;
  // TODO Maybe here contain unordered_set of individuals,
  // or have a separate handler class. Everything else just have
  // vector of appropriate pointers.
  Hand hand_;
};

}  // namespace epidemic
