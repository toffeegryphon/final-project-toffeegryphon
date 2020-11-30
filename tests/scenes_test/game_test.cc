#include <configuration.h>
#include <scene_manager.h>
#include <scenes/game.h>
#include <utils.h>

#include <catch2/catch.hpp>

namespace epidemic {

TEST_CASE("Game Constructor") {
  SceneManager manager;
  Game game(&manager);

  SECTION("Initializes Individual vector with population size") {
    REQUIRE(game.GetIndividuals().size() ==
            Configuration::kDefaultPopulationSize);
  }

  SECTION("Initializes Individual vector with correct number of sick") {
    const vector<Individual>& individuals = game.GetIndividuals();
    size_t sick_count = 0;
    for (const Individual& individual : individuals) {
      if (individual.GetStatus() == Individual::Status::kAsymptomatic) {
        ++sick_count;
      }
    }
    REQUIRE(sick_count == Configuration::kDefaultSickCount);
  }

  SECTION("Initializes City with all Individuals") {
    vector<const Individual*> og_ptrs;
    vector<const Individual*> city_ptrs;
    for (const Individual& individual : game.GetIndividuals()) {
      og_ptrs.push_back(&individual);
    }
    for (Individual* ptr : game.GetCity().GetIndividuals()) {
      city_ptrs.push_back(ptr);
    }

    REQUIRE(city_ptrs == og_ptrs);
  }

  SECTION("Initializes correct number of Isolations") {
    REQUIRE(game.GetIsolations().size() == Configuration::kIsolationCount);
    // TODO Probably should check location as well
  }
}

TEST_CASE("Game Update") {
  SceneManager manager;
  Game game(&manager);

  SECTION("Changes scene to win when no infected individuals are left") {
    // TODO Implement after implementing custom settings
  }

  SECTION("Checks for win before updating") {
    // TODO Implement after implementing custom settings
  }

}

}  // namespace epidemic