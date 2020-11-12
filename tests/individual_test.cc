#include <cinder/gl/gl.h>
#include <individual.h>

#include <catch2/catch.hpp>

using epidemic::Individual;
using glm::vec2;

TEST_CASE("ID", "[property]") {
  vec2 bounds(100, 100);

  SECTION("Starts at 0") {
    Individual i(bounds);
    REQUIRE(i.GetID() == 0);
  }

  SECTION("Increments for subsequent") {
    Individual i_1(bounds), i_2(bounds);
    REQUIRE(i_2.GetID() - i_1.GetID() == 1);
  }
}