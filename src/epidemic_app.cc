#include <configuration.h>
#include <epidemic_app.h>
#include <utils.h>

namespace epidemic {

using ci::gl::clear;
using cinder::app::MouseEvent;
using utils::IsInLocation;

EpidemicApp::EpidemicApp() : city_(kCityData.size) {
  srand(static_cast<unsigned int>(time(nullptr)));

  ci::app::setWindowSize(static_cast<int>(kWindowSize.x),
                         static_cast<int>(kWindowSize.y));

  // TODO Own private method
  vec2 offset(
      kCityData.offset.x + kCityData.size.x + kTemplateIsolationData.offset.x,
      kTemplateIsolationData.offset.y);
  for (size_t i = 0; i < Configuration::kIsolationCount; ++i) {
    isolations_.emplace_back(
        Isolation(kTemplateIsolationData.size),
        Location::Data{kTemplateIsolationData.size, offset});
    offset.y += kTemplateIsolationData.size.y + kTemplateIsolationData.offset.y;
  }
}

void EpidemicApp::setup() {
  AppBase::setup();
}

void EpidemicApp::update() {
  city_.Update();

  for (pair<Isolation, Location::Data>& isolation : isolations_) {
    isolation.first.Update();
  }
}

void EpidemicApp::draw() {
  clear();
  for (const pair<Isolation, Location::Data>& isolation : isolations_) {
    isolation.first.Draw(isolation.second.offset);
  }

  city_.Draw(kCityData.offset);
  hand_.Draw();
}

void EpidemicApp::mouseDown(MouseEvent event) {
  vec2 pos = event.getPos();
  if (IsInLocation(pos, kCityData)) {
    hand_.Add(city_.ExtractIndividualsAt(pos - kCityData.offset));
    hand_.Update(pos);
    return;
  }

  for (pair<Isolation, Location::Data>& isolation : isolations_) {
    if (IsInLocation(pos, isolation.second)) {
      hand_.Add(
          isolation.first.ExtractIndividualsAt(pos - isolation.second.offset));
      hand_.Update(pos);
      return;
    }
  }
}

void EpidemicApp::mouseUp(MouseEvent event) {
  vec2 pos = event.getPos();
  if (IsInLocation(pos, kCityData)) {
    hand_.Update(pos - kCityData.offset);
    city_.Add(hand_.Release());
    return;
  }

  for (pair<Isolation, Location::Data>& isolation : isolations_) {
    if (IsInLocation(pos, isolation.second)) {
      hand_.Update(pos - isolation.second.offset);
      // Handle overflow TODO maybe better way to write
      hand_.Add(isolation.first.Add(hand_.Release()));
      break;
    }
  }

  pos -= kCityData.offset;
  hand_.Update(pos);
  city_.Add(hand_.Release());
}

void EpidemicApp::mouseMove(MouseEvent event) {
  hand_.Update(event.getPos());
}

void EpidemicApp::mouseDrag(MouseEvent event) {
  hand_.Update(event.getPos());
}

}  // namespace epidemic
