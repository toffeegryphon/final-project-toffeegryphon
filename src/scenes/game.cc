#include <cinder/gl/gl.h>
#include <configuration.h>
#include <scenes/game.h>
#include <utils.h>

namespace epidemic {

using ci::gl::clear;
using utils::IsInLocation;
using utils::ToPointers;

Game::Game(SceneManager* manager) : View(manager) {
  GenerateIndividuals();
  city_ = City(kCityData.size, ToPointers(&individuals_));
  GenerateIsolations();
}

// Lifecycle

void Game::Setup() {
  View::Setup();
}

void Game::Update() {
  city_.Update();

  for (pair<Isolation, Location::Data>& isolation : isolations_) {
    isolation.first.Update();
  }
}

void Game::Draw() {
  clear();
  for (const pair<Isolation, Location::Data>& isolation : isolations_) {
    isolation.first.Draw(isolation.second.offset);
  }

  city_.Draw(kCityData.offset);
  hand_.Draw();
}

void Game::MouseDown(MouseEvent event) {
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

void Game::MouseUp(MouseEvent event) {
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

void Game::MouseDrag(MouseEvent event) {
  hand_.Update(event.getPos());
}

void Game::MouseMove(MouseEvent event) {
  hand_.Update(event.getPos());
}

// Getters & Setters

const vector<Individual>& Game::GetIndividuals() const {
  return individuals_;
}
const City& Game::GetCity() const {
  return city_;
}

// Private

void Game::GenerateIndividuals() {
  size_t sick_count = Configuration::kDefaultSickCount;
  size_t population_size = Configuration::kDefaultPopulationSize;
  for (size_t i = 0; i < sick_count; ++i) {
    individuals_.emplace_back(kCityData.size,
                              Individual::Status::kAsymptomatic);
  }
  for (size_t i = 0; i < population_size - sick_count; ++i) {
    individuals_.emplace_back(kCityData.size);
  }
}

void Game::GenerateIsolations() {
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

const vector<pair<Isolation, Location::Data>>& Game::GetIsolations() const {
  return isolations_;
}

}  // namespace epidemic