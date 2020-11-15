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

  // Movement

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

  // Sneeze and Symptomatic

  SECTION("Sneeze if kAsymptomatic and pass sneeze check") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kSymptomatic and pass sneeze check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kDying and pass sneeze check") {
    Individual i(bounds, Individual::Status::kDying);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kRecovered and pass sneeze check") {
    Individual i(bounds, Individual::Status::kRecovered);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Not sneeze if kUninfected") {
    Individual i(bounds, Individual::Status::kUninfected);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds);

    REQUIRE_FALSE(i.IsSneezing());
  }

  SECTION("Not sneeze if fail sneeze check") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(0, 0));
    i.Update(bounds);

    REQUIRE_FALSE(i.IsSneezing());
  }

  SECTION("Updates spread rate") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    float exp_rate = i.GetSpread().x + i.GetSpread().y;
    i.Update(bounds);

    REQUIRE(i.GetSpread().x == exp_rate);
  }

  SECTION("Updates spread rate to negative if become recovered") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds);
    float exp_rate = i.GetSpread().x;
    i.Update(bounds);

    REQUIRE(i.GetSpread().x < exp_rate);
  }

  SECTION("Increase state to kSymptomatic if cross threshold") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold + 0.1f));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == Individual::Status::kSymptomatic);
  }

  SECTION("Does not decrease state to kSymptomatic if kDead") {
    Individual::Status status = Individual::Status::kDead;
    Individual i(bounds, status);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold + 0.1f));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Does not decrease state to kSymptomatic if kDying") {
    srand(3);
    Individual::Status status = Individual::Status::kDying;
    Individual i(bounds, status);
    i.SetDeath(vec2(Configuration::kDyingThreshold + 0.1f, 0));
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold + 0.1f));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Does not decrease state to kSymptomatic if kRecovered") {
    Individual::Status status = Individual::Status::kRecovered;
    Individual i(bounds, status);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold + 0.1f));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == status);
  }

  // Recover or Die

  SECTION("Recover if pass recover check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == Individual::Status::kRecovered);
  }

  SECTION("Die if pass death check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetRecovery(vec2());
    i.SetDeath(vec2(1, 0));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == Individual::Status::kDead);
  }

  SECTION("No change if fail both checks") {
    Individual::Status status = Individual::Status::kAsymptomatic;
    Individual i(bounds, status);
    i.SetSpread(vec2());
    i.SetRecovery(vec2());
    i.SetDeath(vec2());
    i.Update(bounds);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Dying if cross dying threshold") {
    srand(3);
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2());
    i.SetRecovery(vec2());
    i.SetDeath(vec2(Configuration::kDyingThreshold + 0.1f, 0));
    i.Update(bounds);

    REQUIRE(i.GetStatus() == Individual::Status::kDying);
  }

  SECTION("Updates recover and death rate if not dead or recovered") {
    Individual i(bounds, Individual::Status::kDying);
    float exp_recovery = i.GetRecovery().x + i.GetRecovery().y;
    float exp_death = i.GetDeath().x + i.GetDeath().y;
    i.Update(bounds);

    REQUIRE(i.GetRecovery().x == exp_recovery);
    REQUIRE(i.GetDeath().x == exp_death);
  }
}

TEST_CASE("Individual Equality", "[individual][operator][equality]") {
  vec2 bounds(100, 100);
  Individual first(bounds);
  Individual second(first);

  SECTION("Equal if IDs are equal") {
    REQUIRE(first == second);
  }
}