#include <scene_manager.h>
#include <view.h>

#include <catch2/catch.hpp>

namespace epidemic {

TEST_CASE("View ID", "[view][property]") {
  SceneManager manager;
  SECTION("Increments for subsequent") {
    View v_1(&manager), v_2(&manager);
    REQUIRE(v_2.GetID() - v_1.GetID() == 1);
  }
}

TEST_CASE("View Equality", "[view][operator]") {
  SceneManager manager;
  SECTION("Equal if ID equals") {
    View v_1(&manager);
    View v_2(v_1);
    REQUIRE(v_1 == v_2);
  }
}

}  // namespace epidemic