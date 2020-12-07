#include <configuration.h>
#include <scene_manager.h>
#include <scenes/game.h>

#include <catch2/catch.hpp>

namespace epidemic {

using std::make_unique;
using std::unordered_map;

TEST_CASE("Game Constructor") {
  SceneManager manager;
  Game game(&manager);

  SECTION("Initializes Individual vector with population size") {
    REQUIRE(game.GetIndividuals().size() == cfg::kPopulationSize.value);
  }

  SECTION("Initializes Individual vector with correct number of sick") {
    const vector<Individual>& individuals = game.GetIndividuals();
    size_t sick_count = 0;
    for (const Individual& individual : individuals) {
      if (individual.GetStatus() == Individual::Status::kAsymptomatic) {
        ++sick_count;
      }
    }
    REQUIRE(sick_count == cfg::kStartingSickCount.value);
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
    REQUIRE(game.GetIsolations().size() == cfg::kIsolationCount.value);
    // TODO Probably should check position and offset of Isolation as well
  }
}

TEST_CASE("Game Update") {
  SceneManager manager;

  SECTION("Changes scene to win when no infected individuals are left") {
    cfg::kPopulationSize.value = 1;
    cfg::kStartingSickCount.value = 0;
    manager.SetScene(make_unique<Game>(&manager));
    View& game = manager.GetScene();
    game.Update();
    REQUIRE_FALSE(manager.GetScene() == game);
  }

  SECTION("Update does not reorder individuals") {
    cfg::kPopulationSize.value = 5;
    Game game(&manager);
    vector<size_t> ids;
    for (const Individual& individual : game.GetIndividuals()) {
      ids.push_back(individual.GetID());
    }
    game.Update();
    vector<size_t> new_ids;
    for (const Individual& individual : game.GetIndividuals()) {
      new_ids.push_back(individual.GetID());
    }
    REQUIRE(new_ids == ids);
  }

  SECTION("Updates all locations") {
    // Currently unable to test Isolation
    cfg::kPopulationSize.value = 2;
    cfg::kStartingSickCount.value = 1;
    Game game(&manager);
    unordered_map<size_t, vec2> id_to_position;
    for (const Individual& individual : game.GetIndividuals()) {
      id_to_position[individual.GetID()] = individual.GetPosition();
    }
    game.Update();
    for (const Individual& individual : game.GetIndividuals()) {
      REQUIRE_FALSE(individual.GetPosition() ==
                    id_to_position[individual.GetID()]);
    }
  }
}

}  // namespace epidemic