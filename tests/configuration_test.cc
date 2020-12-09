#include <configuration.h>
#include <scene_manager.h>
#include <scenes/game.h>

#include <catch2/catch.hpp>

namespace epidemic {

namespace cfg {

TEST_CASE("PopulateProperties and NormalizeProperties") {
  SECTION("Populates kProperties") {
    REQUIRE(kProperties.empty());
    PopulateProperties();
    REQUIRE_FALSE(kProperties.empty());
  }

  SECTION("Int less than min normalizes to min") {
    kPopulationSize.value = -10;
    NormalizeProperties();
    REQUIRE(kPopulationSize.value == kPopulationSize.value_range.first);
  }

  SECTION("Int more than max normalizes to max") {
    kPopulationSize.value = 10000;
    NormalizeProperties();
    REQUIRE(kPopulationSize.value == kPopulationSize.value_range.second);
  }

  SECTION("Float less than min normalizes to min") {
    kDyingThreshold.value = -1.0f;
    NormalizeProperties();
    REQUIRE(kDyingThreshold.value == kDyingThreshold.value_range.first);
  }

  SECTION("Float more than max normalizes to max") {
    kDyingThreshold.value = 2.0f;
    NormalizeProperties();
    REQUIRE(kDyingThreshold.value == kDyingThreshold.value_range.second);
  }

  SECTION("Vec2.x less than min normalizes to min") {
    kSpreadChanceRange.value.x = -1.0f;
    NormalizeProperties();
    REQUIRE(kSpreadChanceRange.value.x == kSpreadChanceRange.value_range.first.x);
  }

  SECTION("Vec2.x more than max normalizes to max") {
    kSpreadChanceRange.value.x = 2.0f;
    NormalizeProperties();
    REQUIRE(kSpreadChanceRange.value.x == kSpreadChanceRange.value_range.first.y);
  }

  SECTION("Vec2.x less than min normalizes to min") {
    kSpreadChanceRange.value.y = -1.0f;
    NormalizeProperties();
    REQUIRE(kSpreadChanceRange.value.y == kSpreadChanceRange.value_range.first.x);
  }

  SECTION("Vec2.x more than max normalizes to max") {
    kSpreadChanceRange.value.y = 2.0f;
    NormalizeProperties();
    REQUIRE(kSpreadChanceRange.value.y == kSpreadChanceRange.value_range.first.y);
  }

  SECTION("cfg::kStartingSickCount is limited to cfg::kPopulationSize") {
    kPopulationSize.value = 30;
    kStartingSickCount.value = 50;
    NormalizeProperties();
    REQUIRE(kStartingSickCount.value == kPopulationSize.value);
  }
}

TEST_CASE("kProperties") {
  SceneManager manager;

  SECTION("Changing a property changes it globally") {
    int pop_size = 1;
    kPopulationSize.value = pop_size;
    kStartingSickCount.value = 0;
    Game game(&manager);
    REQUIRE(game.GetIndividuals().size() == pop_size);
  }
}

}  // namespace cfg

}  // namespace epidemic