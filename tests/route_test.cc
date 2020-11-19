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

  SECTION("Sets mode to kContinuous") {
    REQUIRE(route.GetMode() == Route::Mode::kContinuous);
  }
}

TEST_CASE("Route Update kContinuous", "[route][update][continuous]") {
  vec2 bounds(100, 100);
  Route route(bounds);
  route.SetMode(Route::Mode::kContinuous);

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

  SECTION("Refills destination queue if reaches minimum") {
    float speed = 10000;

    for (size_t i = 0; i <= Route::kQueueSize.second - Route::kQueueSize.first;
         ++i) {
      route.Update(speed, bounds);
    }

    // Refill before move
    REQUIRE(route.GetDestinations().size() == Route::kQueueSize.second - 1);
  }

  SECTION("Refills destination queue if empty") {
    float speed = 10000;
    route.SetDestinations(queue<vec2>());
    route.Update(speed, bounds);

    // Refill before move
    REQUIRE(route.GetDestinations().size() == Route::kQueueSize.second - 1);
  }
}

TEST_CASE("Route Update kDeplete", "[route][update][deplete]") {
  vec2 bounds(100, 100);
  Route route(bounds);
  route.SetMode(Route::Mode::kDeplete);

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

  SECTION("Depletes destination queue") {
    float speed = 10000;

    for (size_t i = 0; i <= Route::kQueueSize.second; ++i) {
      route.Update(speed, bounds);
    }

    REQUIRE(route.GetDestinations().empty());
  }

  SECTION("Does not move when depleted") {
    float speed = 10000;

    for (size_t i = 0; i <= Route::kQueueSize.second; ++i) {
      route.Update(speed, bounds);
    }
    vec2 pos = route.GetPosition();
    route.Update(speed, bounds);

    REQUIRE(route.GetPosition() == pos);
  }
}

}  // namespace epidemic