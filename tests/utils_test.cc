#include <individual.h>
#include <utils.h>

#include <catch2/catch.hpp>

using std::srand;

static constexpr unsigned int kSeed = 0;

namespace epidemic {

namespace utils {

TEST_CASE("GetRandom", "[random]") {
  srand(kSeed);

  SECTION("Mathematically correct within 0 and 1") {
    REQUIRE(GetRandom() == Approx(0.0011597));
  }
}

TEST_CASE("GetRandomInRange", "[random]") {
  srand(kSeed);

  SECTION("Mathematically correct within range from start") {
    vec2 start_range(100, 10);
    REQUIRE(GetRandomInRange(start_range) == Approx(100.0116));
  }

  SECTION("Mathematically correct between 0 and end") {
    float end(100);
    REQUIRE(GetRandomInRange(end) == Approx(0.11597));
  }
}

TEST_CASE("DistanceX") {
  vec2 bounds(100, 100);

  SECTION("Returns positive x distance if first greater") {
    Individual first(bounds), second(bounds);
    first.SetPosition(vec2(10, 10));
    second.SetPosition(vec2(5, 5));
    REQUIRE(DistanceX(first, second) == 5.0f);
  }

  SECTION("Returns positive x distance if second greater") {
    Individual first(bounds), second(bounds);
    first.SetPosition(vec2(5, 10));
    second.SetPosition(vec2(10, 5));
    REQUIRE(DistanceX(first, second) == 5.0f);
  }
}

TEST_CASE("CompareX", "[comparison]") {
  vec2 bounds(100, 100);

  SECTION("Returns true if first has smaller x position than second") {
    Individual first(bounds), second(bounds);
    first.SetPosition(vec2(5, 10));
    second.SetPosition(vec2(10, 5));
    REQUIRE(CompareX(first, second));
  }

  SECTION("Returns false if first has larger x position than second") {
    Individual first(bounds), second(bounds);
    first.SetPosition(vec2(10, 5));
    second.SetPosition(vec2(5, 10));
    REQUIRE_FALSE(CompareX(first, second));
  }

  SECTION("Returns false if first has same x position second") {
    Individual first(bounds), second(bounds);
    first.SetPosition(vec2(10, 5));
    second.SetPosition(vec2(10, 10));
    REQUIRE_FALSE(CompareX(first, second));
  }
}

TEST_CASE("IsInLocation") {
  SECTION("Returns true if position is entirely within bounds") {
    Location::Data data{vec2(100, 100), vec2(20, 20)};
    vec2 position(50, 50);
    REQUIRE(IsInLocation(position, data));
  }

  SECTION("Returns false if position is exactly at left bounds") {
    Location::Data data{vec2(100, 100), vec2(20, 20)};
    vec2 position(20, 50);
    REQUIRE_FALSE(IsInLocation(position, data));
  }

  SECTION("Returns false if position is exactly at right bounds") {
    Location::Data data{vec2(100, 100), vec2(20, 20)};
    vec2 position(120, 50);
    REQUIRE_FALSE(IsInLocation(position, data));
  }

  SECTION("Returns false if position is exactly at top bounds") {
    Location::Data data{vec2(100, 100), vec2(20, 20)};
    vec2 position(50, 20);
    REQUIRE_FALSE(IsInLocation(position, data));
  }

  SECTION("Returns false if position is exactly at bottom bounds") {
    Location::Data data{vec2(100, 100), vec2(20, 20)};
    vec2 position(50, 120);
    REQUIRE_FALSE(IsInLocation(position, data));
  }

  SECTION("Returns false if position is out of bounds") {
    Location::Data data{vec2(100, 100), vec2(20, 20)};
    vec2 position(200, 200);
    REQUIRE_FALSE(IsInLocation(position, data));
  }
}

}  // namespace utils

}  // namespace epidemic