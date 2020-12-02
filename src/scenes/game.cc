#include <cinder/gl/gl.h>
#include <configuration.h>
#include <scenes/end.h>
#include <scenes/game.h>
#include <scenes/menu.h>
#include <utils.h>

#include <unordered_set>

namespace epidemic {

using std::make_unique;
using std::unordered_set;
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

// TODO Test all updates

void Game::Update() {
  if (HasEnded()) {
    // TODO better way to check end and get values
    Win::Data data{0, 0, 0};
    for (const Individual& individual : individuals_) {
      switch (individual.GetStatus()) {
        case Individual::Status::kUninfected:
          ++data.uninfected;
          break;
        case Individual::Status::kRecovered:
          ++data.recovered;
          break;
        case Individual::Status::kDead:
          ++data.dead;
          break;
        default:
          break;
      }
    }
    manager_->SetScene(make_unique<Win>(manager_, data));
    return;
  }

  city_.Update();

  for (pair<Isolation, Location::Data>& isolation : isolations_) {
    isolation.first.Update();
  }
}

void Game::Draw() {
  for (const pair<Isolation, Location::Data>& isolation : isolations_) {
    isolation.first.Draw(isolation.second.offset);
  }

  city_.Draw(kCityData.offset);
  hand_.Draw();
}

// TODO Test mouse interaction

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

  for (int i = 0; i < Configuration::kIsolationCount.value; ++i) {
    isolations_.emplace_back(
        Isolation(kTemplateIsolationData.size),
        Location::Data{kTemplateIsolationData.size, offset});
    offset.y += kTemplateIsolationData.size.y + kTemplateIsolationData.offset.y;
  }
}

const vector<pair<Isolation, Location::Data>>& Game::GetIsolations() const {
  return isolations_;
}

bool Game::HasEnded() {
  // Checking Win, might need to do a faster method using an event bus
  size_t infected = 0;
  unordered_set<Individual::Status> infected_statuses{
      Individual::Status::kAsymptomatic, Individual::Status::kSymptomatic,
      Individual::Status::kDying};
  // TODO need to check that spread rates are 0 if there are still uninfected
  // people
  for (const Individual& individual : individuals_) {
    if (infected_statuses.find(individual.GetStatus()) !=
        infected_statuses.end()) {
      ++infected;
    }
  }
  return (infected == 0);
}

}  // namespace epidemic