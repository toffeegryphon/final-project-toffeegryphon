#include <cinder/gl/gl.h>
#include <configuration.h>
#include <individual.h>
#include <location.h>

#include <catch2/catch.hpp>

using epidemic::Configuration;
using epidemic::Individual;
using epidemic::Location;
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
        vec2(position.x + Configuration::kSneezeRadius.value / 2, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kAsymptomatic);
  }

  SECTION("Does not become kAsymptomatic if at edge of sneeze range") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(
        vec2(position.x + Configuration::kSneezeRadius.value, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
  }

  SECTION("Does not become kAsymptomatic if outside of sneeze range") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(vec2(
        position.x + Configuration::kSneezeRadius.value + 1.0f, position.y));

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
        vec2(position.x + Configuration::kSneezeRadius.value / 2, position.y));

    individual.SetHealthiness(0.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == status);
  }

  SECTION("Does not become kAsymptomatic if pass healthiness check") {
    Individual individual(bounds), other(bounds);
    vec2 position(10, 10);
    individual.SetPosition(position);
    other.SetPosition(
        vec2(position.x + Configuration::kSneezeRadius.value / 2, position.y));

    individual.SetHealthiness(1.0f);
    individual.CheckAndBecomeInfected(other);

    REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
  }
}

TEST_CASE("Individual Update City", "[individual][update]") {
  vec2 bounds(100, 100);
  Location::Type location_type(Location::Type::kCity);

  // Movement

  SECTION("Moves if not kDead") {
    Individual i(bounds);
    vec2 pos = i.GetPosition();
    i.Update(bounds, location_type);

    REQUIRE_FALSE(i.GetPosition() == pos);
  }

  SECTION("Does not move if kDead") {
    Individual i(bounds, Individual::Status::kDead);
    vec2 pos = i.GetPosition();
    i.Update(bounds, location_type);

    REQUIRE(i.GetPosition() == pos);
  }

  SECTION("Only moves if out of bounds") {
    Individual::Status status = Individual::Status::kSymptomatic;
    Individual i(bounds, status);
    vec2 pos(200, 200);
    i.SetPosition(pos);
    i.SetSpread(vec2(1, 0));
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetPosition() != pos);
    REQUIRE_FALSE(i.IsSneezing());
    REQUIRE(i.GetStatus() == status);
  }

  // Sneeze and Symptomatic

  SECTION("Sneeze if kAsymptomatic and pass sneeze check") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kSymptomatic and pass sneeze check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kDying and pass sneeze check") {
    Individual i(bounds, Individual::Status::kDying);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kRecovered and pass sneeze check") {
    Individual i(bounds, Individual::Status::kRecovered);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Not sneeze if kUninfected") {
    Individual i(bounds, Individual::Status::kUninfected);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE_FALSE(i.IsSneezing());
  }

  SECTION("Not sneeze if fail sneeze check") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(0, 0));
    i.Update(bounds, location_type);

    REQUIRE_FALSE(i.IsSneezing());
  }

  SECTION("Updates spread rate") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    float exp_rate = i.GetSpread().x + i.GetSpread().y;
    i.Update(bounds, location_type);

    REQUIRE(i.GetSpread().x == exp_rate);
  }

  SECTION("Updates spread rate to negative if become recovered") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds, location_type);
    float exp_rate = i.GetSpread().x;
    i.Update(bounds, location_type);

    REQUIRE(i.GetSpread().x < exp_rate);
  }

  SECTION("Increase state to kSymptomatic if cross threshold") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kSymptomatic);
  }

  SECTION("Does not decrease state to kSymptomatic if kDead") {
    Individual::Status status = Individual::Status::kDead;
    Individual i(bounds, status);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Does not decrease state to kSymptomatic if kDying") {
    srand(3);
    Individual::Status status = Individual::Status::kDying;
    Individual i(bounds, status);
    i.SetDeath(vec2(Configuration::kDyingThreshold.value + 0.1f, 0));
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Does not decrease state to kSymptomatic if kRecovered") {
    Individual::Status status = Individual::Status::kRecovered;
    Individual i(bounds, status);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  // Recover or Die

  SECTION("Recover if pass recover check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kRecovered);
  }

  SECTION("Die if pass death check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetRecovery(vec2());
    i.SetDeath(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kDead);
  }

  SECTION("No change if fail both checks") {
    Individual::Status status = Individual::Status::kAsymptomatic;
    Individual i(bounds, status);
    i.SetSpread(vec2());
    i.SetRecovery(vec2());
    i.SetDeath(vec2());
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Dying if cross dying threshold") {
    srand(3);
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2());
    i.SetRecovery(vec2());
    i.SetDeath(vec2(Configuration::kDyingThreshold.value + 0.1f, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kDying);
  }

  SECTION("Updates recover and death rate if not dead or recovered") {
    Individual i(bounds, Individual::Status::kDying);
    float exp_recovery = i.GetRecovery().x + i.GetRecovery().y;
    float exp_death = i.GetDeath().x + i.GetDeath().y;
    i.Update(bounds, location_type);

    REQUIRE(i.GetRecovery().x == exp_recovery);
    REQUIRE(i.GetDeath().x == exp_death);
  }
}

TEST_CASE("Individual Update Isolation", "[individual][update]") {
  vec2 bounds(100, 100);
  Location::Type location_type(Location::Type::kIsolation);

  // Movement

  SECTION("Moves if not kDead") {
    Individual i(bounds);
    vec2 pos = i.GetPosition();
    i.Update(bounds, location_type);

    REQUIRE_FALSE(i.GetPosition() == pos);
  }

  SECTION("Does not move if kDead") {
    Individual i(bounds, Individual::Status::kDead);
    vec2 pos = i.GetPosition();
    i.Update(bounds, location_type);

    REQUIRE(i.GetPosition() == pos);
  }

  // Sneeze and Symptomatic

  SECTION("Sneeze if kAsymptomatic and pass sneeze check") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kSymptomatic and pass sneeze check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kDying and pass sneeze check") {
    Individual i(bounds, Individual::Status::kDying);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Sneeze if kRecovered and pass sneeze check") {
    Individual i(bounds, Individual::Status::kRecovered);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.IsSneezing());
  }

  SECTION("Not sneeze if kUninfected") {
    Individual i(bounds, Individual::Status::kUninfected);
    i.SetSpread(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE_FALSE(i.IsSneezing());
  }

  SECTION("Not sneeze if fail sneeze check") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(0, 0));
    i.Update(bounds, location_type);

    REQUIRE_FALSE(i.IsSneezing());
  }

  SECTION("Updates spread rate") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    float exp_rate = i.GetSpread().x + i.GetSpread().y;
    i.Update(bounds, location_type);

    REQUIRE(i.GetSpread().x == exp_rate);
  }

  SECTION("Updates spread rate to negative if become recovered") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds, location_type);
    float exp_rate = i.GetSpread().x;
    i.Update(bounds, location_type);

    REQUIRE(i.GetSpread().x < exp_rate);
  }

  SECTION("Increase state to kSymptomatic if cross threshold") {
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kSymptomatic);
  }

  SECTION("Does not decrease state to kSymptomatic if kDead") {
    Individual::Status status = Individual::Status::kDead;
    Individual i(bounds, status);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Does not decrease state to kSymptomatic if kDying") {
    srand(3);
    Individual::Status status = Individual::Status::kDying;
    Individual i(bounds, status);
    i.SetDeath(vec2(Configuration::kDyingThreshold.value + 0.1f, 0));
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Does not decrease state to kSymptomatic if kRecovered") {
    Individual::Status status = Individual::Status::kRecovered;
    Individual i(bounds, status);
    i.SetSpread(vec2(0, Configuration::kSymptomaticThreshold.value + 0.1f));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  // Recover or Die

  SECTION("Recover if pass recover check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetRecovery(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kRecovered);
  }

  SECTION("Die if pass death check") {
    Individual i(bounds, Individual::Status::kSymptomatic);
    i.SetRecovery(vec2());
    i.SetDeath(vec2(1, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kDead);
  }

  SECTION("No change if fail both checks") {
    Individual::Status status = Individual::Status::kAsymptomatic;
    Individual i(bounds, status);
    i.SetSpread(vec2());
    i.SetRecovery(vec2());
    i.SetDeath(vec2());
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == status);
  }

  SECTION("Dying if cross dying threshold") {
    srand(3);
    Individual i(bounds, Individual::Status::kAsymptomatic);
    i.SetSpread(vec2());
    i.SetRecovery(vec2());
    i.SetDeath(vec2(Configuration::kDyingThreshold.value + 0.1f, 0));
    i.Update(bounds, location_type);

    REQUIRE(i.GetStatus() == Individual::Status::kDying);
  }

  SECTION(
      "Updates recover and death rate by isolation factor if not dead or "
      "recovered") {
    Individual i(bounds, Individual::Status::kDying);
    float exp_recovery =
        i.GetRecovery().x +
        i.GetRecovery().y * Configuration::kIsolationRecoveryFactor.value;
    float exp_death =
        i.GetDeath().x +
        i.GetDeath().y * Configuration::kIsolationDeathFactor.value;
    i.Update(bounds, location_type);

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