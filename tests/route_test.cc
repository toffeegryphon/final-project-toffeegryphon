#include <route.h>

#include <catch2/catch.hpp>

namespace epidemic {

using glm::length;

TEST_CASE("Route Constructor", "[route][constructor") {
  vec2 bounds(100, 100);
  Route route(bounds);

  SECTION("Sets starting position") {
    REQUIRE(route.GetPosition() != vec2());
  }

  SECTION("Sets number of destinations to maximum") {
    REQUIRE(route.GetDestinations().size() == Route::kQueueSize.second);
  }
}

TEST_CASE("Route Update", "[route][update]") {
  vec2 bounds(100, 100);
  Route route(bounds);

  SECTION("Moves towards destination and does not pop if does not reach") {
    float speed = 0.002f;
    vec2 pos = route.GetPosition();
    vec2 dest = route.GetDestinations().front();
    vec2 to_travel = dest - pos;
    vec2 new_pos = pos + to_travel * speed / length(to_travel);

    route.Update(speed, bounds);
    REQUIRE(route.GetPosition() == new_pos);
    REQUIRE(route.GetDestinations().front() == dest);
  }

  SECTION("Moves to destination and pops if exceeds") {
    float speed = 10000;
    vec2 dest = route.GetDestinations().front();

    route.Update(speed, bounds);
    REQUIRE(route.GetPosition() == dest);
    REQUIRE_FALSE(route.GetDestinations().front() == dest);
  }

  SECTION("Moves to destination and pops if exactly reach") {
    vec2 dest = route.GetDestinations().front();
    vec2 to_travel = dest - route.GetPosition();
    float speed = length(to_travel);

    route.Update(speed, bounds);
    REQUIRE(route.GetPosition().x == Approx(dest.x));
    REQUIRE(route.GetPosition().y == Approx(dest.y));
    REQUIRE_FALSE(route.GetDestinations().front() == dest);
  }
}

}  // namespace epidemic