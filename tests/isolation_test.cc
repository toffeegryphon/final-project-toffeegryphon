#include <configuration.h>
#include <isolation.h>
#include <utils.h>

#include <catch2/catch.hpp>
#include <unordered_set>

namespace epidemic {

using std::string;
using std::to_string;
using std::unordered_set;
using utils::ToPointers;

TEST_CASE("Isolation Constructor", "[isolation][constructor") {
  vec2 bounds(100, 100);
  Isolation isolation(bounds);

  SECTION("Sets type to kIsolation") {
    REQUIRE(isolation.GetType() == Location::Type::kIsolation);
  }
}

TEST_CASE("Isolation Add", "[isolation][interaction][add]") {
  vec2 bounds(100, 100);
  size_t capacity = 5;
  vector<Individual> individuals(3, Individual(bounds));
  vector<Individual*> ind_ptrs(ToPointers(&individuals));
  Isolation isolation(bounds, capacity);

  SECTION("Adds empty vector of individuals to empty isolation") {
    isolation.Add(vector<Individual*>());
    REQUIRE(isolation.GetIndividuals().empty());
  }

  SECTION("Adds filled vector of individuals to empty isolation") {
    isolation.Add(ind_ptrs);
    REQUIRE(isolation.GetIndividuals() == ind_ptrs);
  }

  SECTION("Adding to empty isolation sets non overlapping destinations") {
    isolation.Add(ind_ptrs);
    vector<Individual*> warded = isolation.GetIndividuals();
    unordered_set<string> positions;
    for (const Individual* individual : warded) {
      positions.insert(to_string(individual->GetDestinations().front().x) +
                       to_string(individual->GetDestinations().front().y));
    }
    REQUIRE(positions.size() == warded.size());
  }

  SECTION("Adding to empty isolation sets route mode to kDeplete") {
    isolation.Add(ind_ptrs);
    for (const Individual* individual : isolation.GetIndividuals()) {
      REQUIRE(individual->GetRouteMode() == Route::Mode::kDeplete);
    }
  }

  SECTION("Adds empty vector of individuals to filled isolation") {
    isolation.Add(ind_ptrs);
    vector<Individual*> original = isolation.GetIndividuals();
    isolation.Add(vector<Individual*>());
    REQUIRE(isolation.GetIndividuals() == original);
  }

  SECTION("Adds filled vector of individuals to filled isolation") {
    isolation.Add(vector<Individual*>{ind_ptrs[0]});
    isolation.Add(vector<Individual*>{ind_ptrs[1], ind_ptrs[2]});
    REQUIRE(isolation.GetIndividuals() == ind_ptrs);
  }

  SECTION("Adding to filled isolation sets non overlapping destinations") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(ind_ptrs);
    isolation.Add(src_ptrs);
    vector<Individual*> warded = isolation.GetIndividuals();
    unordered_set<string> positions;
    for (const Individual* individual : warded) {
      positions.insert(to_string(individual->GetDestinations().front().x) +
                       to_string(individual->GetDestinations().front().y));
    }
    REQUIRE(positions.size() == warded.size());
  }

  SECTION(
      "Adding to filled isolation sets route mode to kDeplete only for "
      "admitted") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(ind_ptrs);
    vector<Individual*> rejected = isolation.Add(src_ptrs);
    for (const Individual* individual : isolation.GetIndividuals()) {
      REQUIRE(individual->GetRouteMode() == Route::Mode::kDeplete);
    }
    for (const Individual* individual : rejected) {
      REQUIRE(individual->GetRouteMode() == Route::Mode::kContinuous);
    }
  }

  SECTION("Adding less than capacity count returns empty vector") {
    REQUIRE(isolation.Add(ind_ptrs).empty());
  }

  SECTION("Adding exactly capacity count adds all") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(src_ptrs);
    REQUIRE(isolation.GetIndividuals() == src_ptrs);
  }

  SECTION("Adding exactly capacity count returns empty vector") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    REQUIRE(isolation.Add(src_ptrs).empty());
  }

  SECTION("Adding over capacity adds until capacity reached") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(ind_ptrs);
    isolation.Add(src_ptrs);
    vector<Individual*> expected = ind_ptrs;
    expected.insert(expected.end(), src_ptrs.begin(), src_ptrs.begin() + 2);
    REQUIRE(isolation.GetIndividuals() == expected);
  }

  SECTION("Adding exactly capacity count returns vector containing not added") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(ind_ptrs);
    vector<Individual*> expected(src_ptrs.begin() + 2, src_ptrs.end());
    REQUIRE(isolation.Add(src_ptrs) == expected);
  }

  SECTION("Adding when already at capacity does not add") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(src_ptrs);
    isolation.Add(ind_ptrs);
    REQUIRE(isolation.GetIndividuals() == src_ptrs);
  }

  SECTION("Adding when at capacity returns vector containing not added") {
    vector<Individual> source(capacity, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(src_ptrs);
    REQUIRE(isolation.Add(ind_ptrs) == ind_ptrs);
  }
}

TEST_CASE("Isolation ExtractIndividualsAt",
          "[isolation][interaction][extract]") {
  vec2 bounds(100, 100);
  vector<Individual> individuals{Individual(bounds), Individual(bounds),
                                 Individual(bounds)};
  vector<Individual*> ind_ptrs(ToPointers(&individuals));
  Isolation isolation(bounds);

  SECTION("Extracts individual exactly at position") {
    isolation.Add(ind_ptrs);
    REQUIRE(isolation.ExtractIndividualsAt(ind_ptrs[1]->GetPosition()) ==
            vector<Individual*>{ind_ptrs[1]});
  }

  SECTION("Extracts individual within radius of position") {
    isolation.Add(ind_ptrs);
    vec2 position(ind_ptrs[0]->GetPosition().x +
                      0.9 * Configuration::kDefaultIndividualRadius,
                  ind_ptrs[0]->GetPosition().y);
    REQUIRE(isolation.ExtractIndividualsAt(position) ==
            vector<Individual*>{ind_ptrs[0]});
  }

  SECTION("Does not extract individuals outside of radius of position") {
    isolation.Add(ind_ptrs);
    vec2 position(ind_ptrs[0]->GetPosition().x +
                      1.1 * Configuration::kDefaultIndividualRadius,
                  ind_ptrs[0]->GetPosition().y);
    REQUIRE(isolation.ExtractIndividualsAt(position).empty());
  }

  SECTION("Does not extract individuals at radius of position") {
    isolation.Add(ind_ptrs);
    vec2 position(
        ind_ptrs[0]->GetPosition().x + Configuration::kDefaultIndividualRadius,
        ind_ptrs[0]->GetPosition().y);
    REQUIRE(isolation.ExtractIndividualsAt(position).empty());
  }

  SECTION("Extracts multiple individuals within radius of position") {
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(position);
    source[1].SetPosition(position);
    source[2].SetPosition(bounds);
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(src_ptrs);

    REQUIRE(isolation.ExtractIndividualsAt(position) ==
            vector<Individual*>{src_ptrs[0], src_ptrs[1]});
  }

  SECTION("Removes extracted individuals from isolation individuals") {
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(position);
    source[1].SetPosition(position);
    source[2].SetPosition(bounds);
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(src_ptrs);

    isolation.ExtractIndividualsAt(position);
    REQUIRE(isolation.GetIndividuals() == vector<Individual*>{src_ptrs[2]});
  }

  SECTION("Clears if removing all individuals") {
    vector<Individual> source(3, Individual(bounds));
    vector<Individual*> src_ptrs(ToPointers(&source));
    isolation.Add(src_ptrs);
    isolation.ExtractIndividualsAt(source[0].GetPosition());
    REQUIRE(isolation.GetIndividuals().empty());
  }

  SECTION("Empties destination queue for removed individuals only") {
    isolation.Add(ind_ptrs);
    vector<Individual*> removed =
        isolation.ExtractIndividualsAt(ind_ptrs[1]->GetPosition());
    REQUIRE(removed[0]->GetDestinations().empty());
    for (const Individual* individual : isolation.GetIndividuals()) {
      REQUIRE_FALSE(individual->GetDestinations().empty());
    }
  }

  SECTION("Sets route mode to kContinuous for removed individuals only") {
    isolation.Add(ind_ptrs);
    vector<Individual*> removed =
        isolation.ExtractIndividualsAt(ind_ptrs[1]->GetPosition());
    REQUIRE(removed[0]->GetRouteMode() == Route::Mode::kContinuous);
    for (const Individual* individual : isolation.GetIndividuals()) {
      REQUIRE(individual->GetRouteMode() == Route::Mode::kDeplete);
    }
  }
}

TEST_CASE("Isolation Update", "[isolation][lifecycle][update]") {
  vec2 bounds(100, 100);
  vector<Individual> individuals{
      Individual(bounds, Individual::Status::kAsymptomatic), Individual(bounds),
      Individual(bounds)};
  vector<Individual*> ind_ptrs(ToPointers(&individuals));
  Isolation isolation(bounds);

  SECTION("Updates position") {
    isolation.Add(ind_ptrs);
    isolation.Update();
    for (size_t i = 0; i < ind_ptrs.size(); ++i) {
      ind_ptrs[i]->Update(bounds, Location::Type::kIsolation);
      REQUIRE(isolation.GetIndividuals()[i] == ind_ptrs[i]);
    }
  }

  SECTION("Checks to pass to all if one sneezes") {
    ind_ptrs[0]->SetSpread(vec2(1, 0));
    ind_ptrs[1]->SetSpread(vec2(0, 0));
    ind_ptrs[1]->SetHealthiness(0);
    ind_ptrs[2]->SetSpread(vec2(0, 0));
    ind_ptrs[2]->SetHealthiness(0);
    isolation.Add(ind_ptrs);
    isolation.Update();
    for (const Individual* individual : isolation.GetIndividuals()) {
      REQUIRE_FALSE(individual->GetStatus() == Individual::Status::kUninfected);
    }
  }

  SECTION("Updates recovery chance with isolation multiplier") {
    ind_ptrs[0]->SetRecovery(vec2(0, 0.2));
    isolation.Add(ind_ptrs);
    float expected_recovery =
        ind_ptrs[0]->GetRecovery().x +
        ind_ptrs[0]->GetRecovery().y * Configuration::kIsolationRecoveryFactor;
    isolation.Update();
    REQUIRE(isolation.GetIndividuals()[0]->GetRecovery().x ==
            expected_recovery);
  }

  SECTION("Updates death chance with isolation multiplier") {
    ind_ptrs[0]->SetDeath(vec2(0, 0.2));
    isolation.Add(ind_ptrs);
    float expected_death =
        ind_ptrs[0]->GetDeath().x +
        ind_ptrs[0]->GetDeath().y * Configuration::kIsolationDeathFactor;
    isolation.Update();
    REQUIRE(isolation.GetIndividuals()[0]->GetDeath().x == expected_death);
  }
}

}  // namespace epidemic