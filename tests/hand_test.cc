#include <hand.h>

#include <catch2/catch.hpp>

namespace epidemic {

TEST_CASE("Hand Add", "[hand][add]") {
  vec2 bounds(100, 100);
  Hand hand;
  vector<Individual> source{Individual(bounds), Individual(bounds),
                            Individual(bounds)};

  SECTION("Adds single to empty hand") {
    const Individual& i = source[0];
    hand.Add(i);
    REQUIRE(hand.GetIndividuals() == vector<Individual>{i});
  }

  SECTION("Adds single to filled hand") {
    for (const Individual& individual : source) {
      hand.Add(individual);
    }

    REQUIRE(hand.GetIndividuals() == source);
  }

  SECTION("Adds empty vector to empty hand") {
    hand.Add(vector<Individual>());
    REQUIRE(hand.GetIndividuals().empty());
  }

  SECTION("Adds filled vector to empty hand") {
    hand.Add(source);
    REQUIRE(hand.GetIndividuals() == source);
  }

  SECTION("Adds empty vector to filled hand") {
    hand.Add(source);
    hand.Add(vector<Individual>());
    REQUIRE(hand.GetIndividuals() == source);
  }

  SECTION("Adds filled vector to filled hand") {
    hand.Add(source[0]);
    hand.Add(vector<Individual>{source[1], source[2]});
    REQUIRE(hand.GetIndividuals() == source);
  }
}

TEST_CASE("Hand Release", "[hand][release]") {
  vec2 bounds(100, 100);
  Hand hand;
  vector<Individual> source{Individual(bounds), Individual(bounds),
                            Individual(bounds)};

  hand.Add(source);

  SECTION("Returns contained individuals") {
    REQUIRE(hand.Release() == source);
  }

  SECTION("Clears contained individuals") {
    hand.Release();
    REQUIRE(hand.GetIndividuals().empty());
  }
}

TEST_CASE("Hand Update", "[hand][lifecycle][update]") {
  vec2 bounds(100, 100);
  Hand hand;
  vector<Individual> source{Individual(bounds), Individual(bounds),
                            Individual(bounds)};
  hand.Add(source);
  SECTION("Sets individuals position to new") {
    hand.Update(bounds);
    for (const Individual& individual : hand.GetIndividuals()) {
      REQUIRE(individual.GetPosition() == bounds);
    }
  }
}

}  // namespace epidemic