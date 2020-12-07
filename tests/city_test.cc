#include <cinder/gl/gl.h>
#include <city.h>
#include <configuration.h>
#include <utils.h>

#include <catch2/catch.hpp>
#include <queue>

namespace epidemic {

using glm::vec2;
using std::make_pair;
using std::queue;
using std::string;
using std::to_string;
using std::unordered_map;
using utils::ToPointers;

TEST_CASE("City Constructor", "[city][constructor") {
  vec2 bounds(100, 100);
  City city(bounds);

  SECTION("Sets type to kCity") {
    REQUIRE(city.GetType() == Location::Type::kCity);
  }
}

TEST_CASE("City Update", "[city][lifecycle][update]") {
  vec2 bounds(100, 100);

  // TODO Generator or put in Utils
  vector<Individual> source{
      Individual(bounds, Individual::Status::kAsymptomatic),
      Individual(bounds, Individual::Status::kAsymptomatic), Individual(bounds),
      Individual(bounds), Individual(bounds)};
  vector<Individual*> individual_ptrs = ToPointers(&source);

  SECTION("Updates all individuals") {
    City city(bounds, individual_ptrs);

    vector<Individual*> original = city.GetIndividuals();
    unordered_map<size_t, string> og_pos;
    for (const Individual* individual : city.GetIndividuals()) {
      og_pos.insert(make_pair(individual->GetID(),
                              to_string(individual->GetPosition().x) +
                                  to_string(individual->GetPosition().y)));
    }
    city.Update();

    for (const Individual* individual : city.GetIndividuals()) {
      REQUIRE_FALSE(to_string(individual->GetPosition().x) +
                        to_string(individual->GetPosition().y) ==
                    og_pos[individual->GetID()]);
    }
  }

  SECTION(
      "Transmit if pass sneeze check between 2 individuals within spread "
      "radius") {
    vector<Individual> individuals{
        Individual(bounds, Individual::Status::kAsymptomatic),
        Individual(bounds)};
    individuals[0].SetSpread(vec2(1, 0));
    individuals[0].SetPosition(vec2(50, 50));

    queue<vec2> destinations;
    destinations.push(individuals[0].GetPosition());
    individuals[0].SetDestinations(destinations);

    individuals[1].SetPosition(vec2(55, 55));
    individuals[1].SetDestinations(destinations);
    individuals[1].SetHealthiness(0);

    City city(bounds, ToPointers(&individuals));
    city.Update();

    for (const Individual* individual : city.GetIndividuals()) {
      REQUIRE_FALSE(individual->GetStatus() == Individual::Status::kUninfected);
    }
  }

  SECTION("Transmission from single to many") {
    vector<Individual> individuals{
        Individual(bounds, Individual::Status::kAsymptomatic),
        Individual(bounds), Individual(bounds)};
    individuals[0].SetSpread(vec2(1, 0));
    individuals[0].SetPosition(vec2(50, 50));

    queue<vec2> destinations;
    destinations.push(individuals[0].GetPosition());
    individuals[0].SetDestinations(destinations);

    individuals[1].SetPosition(vec2(55, 55));
    individuals[1].SetDestinations(destinations);
    individuals[1].SetHealthiness(0);

    individuals[2].SetPosition(vec2(45, 45));
    individuals[2].SetDestinations(destinations);
    individuals[2].SetHealthiness(0);

    City city(bounds, ToPointers(&individuals));
    city.Update();

    for (const Individual* individual : city.GetIndividuals()) {
      REQUIRE_FALSE(individual->GetStatus() == Individual::Status::kUninfected);
    }
  }

  SECTION("Transmission separate clusters") {
    vector<Individual> individuals{
        Individual(bounds, Individual::Status::kAsymptomatic),
        Individual(bounds),
        Individual(bounds, Individual::Status::kAsymptomatic),
        Individual(bounds), Individual(bounds)};
    individuals[0].SetSpread(vec2(1, 0));
    individuals[0].SetPosition(vec2(50, 50));

    queue<vec2> d_1;
    d_1.push(individuals[0].GetPosition());
    individuals[0].SetDestinations(d_1);

    individuals[1].SetPosition(vec2(55, 55));
    individuals[1].SetDestinations(d_1);
    individuals[1].SetHealthiness(0);

    individuals[2].SetSpread(vec2(1, 0));
    individuals[2].SetPosition(vec2(5, 5));

    queue<vec2> d_2;
    d_2.push(individuals[2].GetPosition());
    individuals[0].SetDestinations(d_2);

    individuals[3].SetPosition(vec2(8, 8));
    individuals[3].SetDestinations(d_2);
    individuals[3].SetHealthiness(0);

    size_t i_4 = individuals[4].GetID();
    individuals[4].SetPosition(vec2(99, 99));
    individuals[4].SetDestinations(d_1);

    City city(bounds, ToPointers(&individuals));
    city.Update();

    for (const Individual* individual : city.GetIndividuals()) {
      if (individual->GetID() == i_4) {
        REQUIRE(individual->GetStatus() == Individual::Status::kUninfected);
      } else {
        REQUIRE_FALSE(individual->GetStatus() ==
                      Individual::Status::kUninfected);
      }
    }
  }

  SECTION("Not transmit if pass sneeze check but beyond spread radius") {
    vector<Individual> individuals{
        Individual(bounds, Individual::Status::kAsymptomatic),
        Individual(bounds)};
    individuals[0].SetSpread(vec2(1, 0));
    individuals[0].SetPosition(vec2(10, 10));

    queue<vec2> destinations;
    destinations.push(vec2());
    individuals[0].SetDestinations(destinations);

    size_t i_1 = individuals[1].GetID();
    individuals[1].SetPosition(vec2(90, 90));
    individuals[1].SetDestinations(destinations);
    individuals[1].SetHealthiness(0);

    City city(bounds, ToPointers(&individuals));
    city.Update();

    for (const Individual* individual : city.GetIndividuals()) {
      if (individual->GetID() == i_1) {
        REQUIRE(individual->GetStatus() == Individual::Status::kUninfected);
      }
    }
  }

  SECTION("Not transmit if within radius but fail sneeze check") {
    vector<Individual> individuals{
        Individual(bounds, Individual::Status::kAsymptomatic),
        Individual(bounds)};
    individuals[0].SetSpread(vec2(0, 0));
    individuals[0].SetPosition(vec2(50, 50));

    queue<vec2> destinations;
    destinations.push(individuals[0].GetPosition());
    individuals[0].SetDestinations(destinations);

    size_t i_1 = individuals[1].GetID();
    individuals[1].SetPosition(vec2(55, 55));
    individuals[1].SetDestinations(destinations);
    individuals[1].SetHealthiness(1);

    City city(bounds, ToPointers(&individuals));
    city.Update();

    for (const Individual* individual : city.GetIndividuals()) {
      if (individual->GetID() == i_1) {
        REQUIRE(individual->GetStatus() == Individual::Status::kUninfected);
      }
    }
  }
}

TEST_CASE("City Add", "[city][interaction][add]") {
  vec2 bounds(100, 100);

  // TODO Generator or put in Utils
  vector<Individual> source{
      Individual(bounds, Individual::Status::kAsymptomatic),
      Individual(bounds, Individual::Status::kAsymptomatic), Individual(bounds),
      Individual(bounds), Individual(bounds)};
  vector<Individual*> src_ptrs = ToPointers(&source);

  vector<Individual> individuals{Individual(bounds), Individual(bounds),
                                 Individual(bounds)};
  vector<Individual*> ind_ptrs = ToPointers(&individuals);

  SECTION("Adds empty vector of individuals to empty city") {
    City city(bounds);
    city.Add(vector<Individual*>());
    REQUIRE(city.GetIndividuals().empty());
  }

  SECTION("Adds filled vector of individuals to empty city") {
    City city(bounds);
    city.Add(ind_ptrs);
    REQUIRE(city.GetIndividuals() == ind_ptrs);
  }

  SECTION("Adds empty vector of individuals to filled city") {
    City city(bounds, src_ptrs);
    vector<Individual*> original = city.GetIndividuals();
    city.Add(vector<Individual*>());
    REQUIRE(city.GetIndividuals() == original);
  }

  SECTION("Adds filled vector of individuals to filled city") {
    City city(bounds, src_ptrs);
    vector<Individual*> expected = city.GetIndividuals();
    expected.insert(expected.end(), ind_ptrs.begin(), ind_ptrs.end());
    city.Add(ind_ptrs);
    REQUIRE(city.GetIndividuals() == expected);
  }
}

TEST_CASE("City ExtractIndividualsAt", "[city][interaction][extract]") {
  vec2 bounds(100, 100);

  // TODO Generator or put in Utils
  vector<Individual> src{Individual(bounds, Individual::Status::kAsymptomatic)};
  vector<Individual*> src_ptrs = ToPointers(&src);

  SECTION("Extracts individual exactly at position") {
    City city(bounds, src_ptrs);
    vector<Individual*> expected = city.GetIndividuals();
    REQUIRE(city.ExtractIndividualsAt(expected[0]->GetPosition()) == expected);
  }

  SECTION("Extracts individual within radius of position") {
    City city(bounds, src_ptrs);
    vector<Individual*> expected = city.GetIndividuals();
    vec2 position(expected[0]->GetPosition().x +
                      0.9 * cfg::kDefaultIndividualRadius,
                  expected[0]->GetPosition().y);
    REQUIRE(city.ExtractIndividualsAt(position) == expected);
  }

  SECTION("Does not extract individuals outside of radius of position") {
    City city(bounds, src_ptrs);
    vector<Individual*> expected = city.GetIndividuals();
    vec2 position(expected[0]->GetPosition().x +
                      1.1 * cfg::kDefaultIndividualRadius,
                  expected[0]->GetPosition().y);
    REQUIRE(city.ExtractIndividualsAt(position).empty());
  }

  SECTION("Does not extract individuals at radius of position") {
    City city(bounds, src_ptrs);
    vector<Individual*> expected = city.GetIndividuals();
    vec2 position(
        expected[0]->GetPosition().x + cfg::kDefaultIndividualRadius,
        expected[0]->GetPosition().y);
    REQUIRE(city.ExtractIndividualsAt(position).empty());
  }

  SECTION("Extracts multiple individuals within radius of position") {
    City city(bounds);
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(position);
    source[1].SetPosition(position);
    source[2].SetPosition(bounds);

    vector<Individual*> source_ptrs(ToPointers(&source));
    city.Add(source_ptrs);

    REQUIRE(city.ExtractIndividualsAt(position) ==
            vector<Individual*>{source_ptrs[0], source_ptrs[1]});
  }

  SECTION("Removes extracted individuals from city individuals") {
    City city(bounds);
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(bounds);
    source[1].SetPosition(position);
    source[2].SetPosition(position);

    vector<Individual*> source_ptrs(ToPointers(&source));
    city.Add(source_ptrs);

    city.ExtractIndividualsAt(position);
    REQUIRE(city.GetIndividuals() == vector<Individual*>{source_ptrs[0]});
  }

  SECTION("Clears if removing all individuals") {
    City city(bounds);
    vector<Individual> source(3, Individual(bounds));
    city.Add(ToPointers(&source));
    city.ExtractIndividualsAt(source[0].GetPosition());
    REQUIRE(city.GetIndividuals().empty());
  }
}

}  // namespace epidemic