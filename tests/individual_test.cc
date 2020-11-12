#include <cinder/gl/gl.h>
#include <configuration.h>
#include <individual.h>

#include <catch2/catch.hpp>

using epidemic::Configuration;
using epidemic::Individual;
using glm::vec2;

TEST_CASE("Individual ID", "[individual][property]") {
  vec2 bounds(100, 100);

  SECTION("Increments for subsequent") {
    Individual i_1(bounds), i_2(bounds);
    REQUIRE(i_2.GetID() - i_1.GetID() == 1);
  }
}

TEST_CASE("Individual CheckAndBecomeInfected", "[individual]") {
  vec2 bounds(100, 100);

  SECTION(
      "Becomes kAsymptomatic if uninfected, within range and fail healthiness "
      "check") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(
        vec2(position.x + Configuration::kDefaultSneezeRadius / 2, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kAsymptomatic);
  }

  SECTION("Does not become kAsymptomatic if at edge of sneeze range") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(
        vec2(position.x + Configuration::kDefaultSneezeRadius, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
  }

  SECTION("Does not become kAsymptomatic if outside of sneeze range") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(vec2(
        position.x + Configuration::kDefaultSneezeRadius + 1.0f, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
  }

  SECTION(
      "Does not become kAsymptomatic if already at some level of infection") {
    Individual::Status status = Individual::Status::kRecovered;
    Individual individual(bounds, status), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(
        vec2(position.x + Configuration::kDefaultSneezeRadius / 2, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == status);
  }

  SECTION("Does not become kAsymptomatic if pass healthiness check") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(
        vec2(position.x + Configuration::kDefaultSneezeRadius / 2, position.y));

    individual.SetHealthiness(1.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
  }
}

TEST_CASE("Individual Update", "[individual][update]") {
  vec2 bounds(100, 100);

  SECTION("Moves if not kDead") {
    Individual i(bounds);
    vec2 pos = i.GetPosition();
    i.Update(bounds);

    REQUIRE_FALSE(i.GetPosition() == pos);
  }

  SECTION("Does not move if kDead") {
    Individual i(bounds, Individual::Status::kDead);
    vec2 pos = i.GetPosition();
    i.Update(bounds);

    REQUIRE(i.GetPosition() == pos);
  }
}