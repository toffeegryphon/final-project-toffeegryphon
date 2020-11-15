#include <configuration.h>
#include <isolation.h>

#include <catch2/catch.hpp>

namespace epidemic {

TEST_CASE("Isolation Constructor", "[isolation][constructor") {
  vec2 bounds(100, 100);
  Isolation isolation(bounds);

  SECTION("Sets type to kIsolation") {
    REQUIRE(isolation.GetType() == Location::Type::kIsolation);
  }
}

TEST_CASE("Isolation Add", "[isolation][interaction][add]") {
  vec2 bounds(100, 100);
  vector<Individual> individuals{Individual(bounds), Individual(bounds),
                                 Individual(bounds)};
  Isolation isolation(bounds);

  SECTION("Adds empty vector of individuals to empty isolation") {
    isolation.Add(vector<Individual>{});
    REQUIRE(isolation.GetIndividuals().empty());
  }

  SECTION("Adds filled vector of individuals to empty isolation") {
    isolation.Add(individuals);
    REQUIRE(isolation.GetIndividuals() == individuals);
  }

  SECTION("Adds empty vector of individuals to filled isolation") {
    isolation.Add(individuals);
    vector<Individual> original = isolation.GetIndividuals();
    isolation.Add(vector<Individual>{});
    REQUIRE(isolation.GetIndividuals() == original);
  }

  SECTION("Adds filled vector of individuals to filled isolation") {
    isolation.Add(vector<Individual>{individuals[0]});
    isolation.Add(vector<Individual>{individuals[1], individuals[2]});
    REQUIRE(isolation.GetIndividuals() == individuals);
  }
}

TEST_CASE("Isolation ExtractIndividualsAt",
          "[isolation][interaction][extract]") {
  vec2 bounds(100, 100);
  vector<Individual> individuals{Individual(bounds), Individual(bounds),
                                 Individual(bounds)};
  Isolation isolation(bounds);

  SECTION("Extracts individual exactly at position") {
    isolation.Add(individuals);
    REQUIRE(isolation.ExtractIndividualsAt(individuals[0].GetPosition()) ==
            vector<Individual>{individuals[0]});
  }

  SECTION("Extracts individual within radius of position") {
    isolation.Add(individuals);
    vec2 position(individuals[0].GetPosition().x +
                      0.9 * Configuration::kDefaultIndividualRadius,
                  individuals[0].GetPosition().y);
    REQUIRE(isolation.ExtractIndividualsAt(position) ==
            vector<Individual>{individuals[0]});
  }

  SECTION("Does not extract individuals outside of radius of position") {
    isolation.Add(individuals);
    vec2 position(individuals[0].GetPosition().x +
                      1.1 * Configuration::kDefaultIndividualRadius,
                  individuals[0].GetPosition().y);
    REQUIRE(isolation.ExtractIndividualsAt(position).empty());
  }

  SECTION("Does not extract individuals at radius of position") {
    isolation.Add(individuals);
    vec2 position(individuals[0].GetPosition().x +
                      Configuration::kDefaultIndividualRadius,
                  individuals[0].GetPosition().y);
    REQUIRE(isolation.ExtractIndividualsAt(position).empty());
  }

  SECTION("Extracts multiple individuals within radius of position") {
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(position);
    source[1].SetPosition(position);
    source[2].SetPosition(bounds);
    isolation.Add(source);

    REQUIRE(isolation.ExtractIndividualsAt(position) ==
            vector<Individual>{source[0], source[1]});
  }

  SECTION("Removes extracted individuals from isolation individuals") {
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(position);
    source[1].SetPosition(position);
    source[2].SetPosition(bounds);
    isolation.Add(source);

    isolation.ExtractIndividualsAt(position);
    REQUIRE(isolation.GetIndividuals() == vector<Individual>{source[2]});
  }
}

}  // namespace epidemic