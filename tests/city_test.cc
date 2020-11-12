#include <cinder/gl/gl.h>
#include <city.h>
#include <configuration.h>

#include <catch2/catch.hpp>

namespace epidemic {

using glm::vec2;

TEST_CASE("City Constructor", "[city][constructor") {
  vec2 bounds(100, 100);
  SECTION("Creates city with population size") {
    City city(bounds);
    REQUIRE(city.GetIndividuals().size() ==
            Configuration::kDefaultPopulationSize);
  }

  SECTION("Creates city with correct number of sick") {
    City city(bounds);
    const vector<Individual>& individuals = city.GetIndividuals();
    size_t sick_count = 0;
    for (const Individual& individual : individuals) {
      if (individual.GetStatus() == Individual::Status::kAsymptomatic) {
        ++sick_count;
      }
    }
    REQUIRE(sick_count == Configuration::kDefaultSickCount);
  }
}

}  // namespace epidemic