#include <configuration.h>
#include <scene_manager.h>
#include <scenes/game.h>

#include <catch2/catch.hpp>

namespace epidemic {

namespace cfg {

TEST_CASE("PopulateProperties") {
  SECTION("Populates kProperties") {
    REQUIRE(kProperties.empty());
    PopulateProperties();
    REQUIRE_FALSE(kProperties.empty());
  }
}

TEST_CASE("kProperties") {
  SceneManager manager;

  SECTION("Changing a property changes it globally") {
    int pop_size = 1;
    kPopulationSize.value = pop_size;
    Game game(&manager);
    REQUIRE(game.GetIndividuals().size() == pop_size);
  }
}

}  // namespace cfg

}  // namespace epidemic