#include <hand.h>
#include <utils.h>

#include <catch2/catch.hpp>

namespace epidemic {

using utils::ToPointers;

TEST_CASE("Hand Add", "[hand][add]") {
  vec2 bounds(100, 100);
  Hand hand;
  vector<Individual> source{Individual(bounds), Individual(bounds),
                            Individual(bounds)};
  vector<Individual*> src_ptrs(ToPointers(&source));

  SECTION("Adds single to empty hand") {
    Individual* i = src_ptrs[0];
    hand.Add(i);
    REQUIRE(hand.GetIndividuals() == vector<Individual*>{i});
  }

  SECTION("Adds single to filled hand") {
    for (Individual* individual : src_ptrs) {
      hand.Add(individual);
    }

    REQUIRE(hand.GetIndividuals() == src_ptrs);
  }

  SECTION("Adds empty vector to empty hand") {
    hand.Add(vector<Individual*>());
    REQUIRE(hand.GetIndividuals().empty());
  }

  SECTION("Adds filled vector to empty hand") {
    hand.Add(src_ptrs);
    REQUIRE(hand.GetIndividuals() == src_ptrs);
  }

  SECTION("Adds empty vector to filled hand") {
    hand.Add(src_ptrs);
    hand.Add(vector<Individual*>());
    REQUIRE(hand.GetIndividuals() == src_ptrs);
  }

  SECTION("Adds filled vector to filled hand") {
    hand.Add(src_ptrs[0]);
    hand.Add(vector<Individual*>{src_ptrs[1], src_ptrs[2]});
    REQUIRE(hand.GetIndividuals() == src_ptrs);
  }
}

TEST_CASE("Hand Release", "[hand][release]") {
  vec2 bounds(100, 100);
  Hand hand;
  vector<Individual> source{Individual(bounds), Individual(bounds),
                            Individual(bounds)};
  vector<Individual*> src_ptrs(ToPointers(&source));

  hand.Add(src_ptrs);

  SECTION("Returns contained individuals") {
    REQUIRE(hand.Release() == src_ptrs);
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
  vector<Individual*> src_ptrs(ToPointers(&source));
  hand.Add(src_ptrs);

  SECTION("Sets individuals position to new") {
    hand.Update(bounds);
    for (const Individual* individual : hand.GetIndividuals()) {
      REQUIRE(individual->GetPosition() == bounds);
    }
  }
}

}  // namespace epidemic