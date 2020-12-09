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

TEST_CASE("Mouse Down") {
  SceneManager manager;

  size_t size = 3;
  cfg::kPopulationSize.value = size;
  cfg::kStartingSickCount.value = 0;
  Game game(&manager);

  SECTION("Does not move individuals if empty spot") {
    MouseEvent event;
    event.setPos(vec2(0, cfg::kCityData.size.y + cfg::kCityData.offset.y +
                             3 * cfg::kDefaultIndividualRadius));
    game.MouseDown(event);
    REQUIRE(game.GetCity().GetIndividuals().size() == size);
    REQUIRE(game.GetHand().GetIndividuals().empty());
  }

  SECTION("Removes individual at MouseEvent from city and adds to hand") {
    size_t i = 0;
    size_t id = game.GetIndividuals()[i].GetID();
    MouseEvent event;
    event.setPos(game.GetIndividuals()[i].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseDown(event);
    REQUIRE(game.GetHand().GetIndividuals().size() == 1);
    REQUIRE(game.GetHand().GetIndividuals()[0]->GetID() == id);

    const vector<Individual*>& v_i = game.GetCity().GetIndividuals();
    REQUIRE(v_i.size() == size - 1);
    REQUIRE(std::find_if(v_i.begin(), v_i.end(), [id](Individual* i) {
              return (i->GetID() == id);
            }) == v_i.end());
  }

  SECTION("Removes individual at MouseEvent from isolation and adds to hand") {
    size_t i = 0;
    size_t id = game.GetIndividuals()[i].GetID();
    MouseEvent event;
    event.setPos(game.GetIndividuals()[i].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseDown(event);
    event.setPos(game.GetIsolations()[0].second.offset + vec2(5, 5));
    game.MouseUp(event);
    REQUIRE(game.GetIsolations()[0].first.GetIndividuals().size() == 1);

    game.MouseDown(event);
    REQUIRE(game.GetIsolations()[0].first.GetIndividuals().empty());
    REQUIRE(game.GetHand().GetIndividuals().size() == 1);
    REQUIRE(game.GetHand().GetIndividuals()[0]->GetID() == id);
  }
}

TEST_CASE("Mouse Up") {
  SceneManager manager;

  size_t size = 3;
  cfg::kPopulationSize.value = size;
  cfg::kStartingSickCount.value = 0;

  SECTION("Does not move anything on empty hand") {
    Game game(&manager);
    MouseEvent event;
    event.setPos(game.GetIndividuals()[0].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseUp(event);
    REQUIRE(game.GetCity().GetIndividuals().size() == size);
    REQUIRE(game.GetHand().GetIndividuals().empty());
  }

  SECTION(
      "Removes all individuals from hand and adds to city if released in "
      "city") {
    Game game(&manager);
    MouseEvent event;
    event.setPos(game.GetIndividuals()[0].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseDown(event);
    REQUIRE(game.GetHand().GetIndividuals().size() == 1);
    REQUIRE(game.GetCity().GetIndividuals().size() == size - 1);

    event.setPos(cfg::kCityData.offset + vec2(10, 10));
    game.MouseUp(event);
    REQUIRE(game.GetHand().GetIndividuals().empty());
    REQUIRE(game.GetCity().GetIndividuals().size() == size);
  }

  SECTION(
      "Removes all individuals from hand and adds to isolation if released in "
      "isolation") {
    Game game(&manager);
    MouseEvent event;
    event.setPos(game.GetIndividuals()[0].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseDown(event);
    REQUIRE(game.GetHand().GetIndividuals().size() == 1);
    REQUIRE(game.GetCity().GetIndividuals().size() == size - 1);

    event.setPos(game.GetIsolations()[0].second.offset + vec2(5, 5));
    game.MouseUp(event);
    REQUIRE(game.GetHand().GetIndividuals().empty());
    REQUIRE(game.GetCity().GetIndividuals().size() == size - 1);
    REQUIRE(game.GetIsolations()[0].first.GetIndividuals().size() == 1);
  }

  SECTION(
      "Removes all individuals from hand and adds to city if released in "
      "isolation but overflows") {
    cfg::kIsolationCapacity.value = 0;
    Game game(&manager);

    MouseEvent event;
    event.setPos(game.GetIndividuals()[0].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseDown(event);
    REQUIRE(game.GetHand().GetIndividuals().size() == 1);
    REQUIRE(game.GetCity().GetIndividuals().size() == size - 1);

    event.setPos(game.GetIsolations()[0].second.offset + vec2(5, 5));
    game.MouseUp(event);
    REQUIRE(game.GetHand().GetIndividuals().empty());
    REQUIRE(game.GetIsolations()[0].first.GetIndividuals().empty());
    REQUIRE(game.GetCity().GetIndividuals().size() == size);
  }

  SECTION(
      "Removes all individuals from hand and adds to city if released "
      "elsewhere") {
    Game game(&manager);
    MouseEvent event;
    event.setPos(game.GetIndividuals()[0].GetPosition() +
                 cfg::kCityData.offset);
    game.MouseDown(event);
    REQUIRE(game.GetHand().GetIndividuals().size() == 1);
    REQUIRE(game.GetCity().GetIndividuals().size() == size - 1);

    event.setPos(cfg::kCityData.offset + cfg::kCityData.size + vec2(10, 10));
    game.MouseUp(event);
    REQUIRE(game.GetHand().GetIndividuals().empty());
    REQUIRE(game.GetCity().GetIndividuals().size() == size);
  }
}

TEST_CASE("Mouse Drag and Move") {
  SceneManager manager;

  size_t size = 3;
  cfg::kPopulationSize.value = size;
  cfg::kStartingSickCount.value = 0;
  Game game(&manager);

  MouseEvent event;
  for (const Individual& individual : game.GetIndividuals()) {
    event.setPos(individual.GetPosition() + cfg::kCityData.offset);
    game.MouseDown(event);
  }
  REQUIRE(game.GetHand().GetIndividuals().size() == size);

  vec2 pos(1000, 1000);
  event.setPos(pos);
  game.MouseMove(event);

  SECTION("MouseDrag updates position of individuals in hand") {
    for (const Individual& individual : game.GetIndividuals()) {
      REQUIRE(individual.GetPosition() == pos);
    }
  }

  SECTION("MouseMove updates position of individuals in hand") {
    for (const Individual& individual : game.GetIndividuals()) {
      REQUIRE(individual.GetPosition() == pos);
    }
  }
}

}  // namespace epidemic