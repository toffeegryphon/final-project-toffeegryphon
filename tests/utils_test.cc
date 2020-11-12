#include <utils.h>

#include <catch2/catch.hpp>

using std::srand;

static constexpr unsigned int kSeed = 0;

namespace epidemic {

namespace utils {

TEST_CASE("GetRandom") {
  srand(kSeed);

  SECTION("Mathematically correct within 0 and 1") {
    REQUIRE(GetRandom() == Approx(0.0011597));
  }
}

TEST_CASE("GetRandomInRange") {
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

}  // namespace utils

}  // namespace epidemic