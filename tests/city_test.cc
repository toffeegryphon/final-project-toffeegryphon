#include <cinder/gl/gl.h>
#include <city.h>
#include <configuration.h>

#include <catch2/catch.hpp>
#include <queue>

namespace epidemic {

using glm::vec2;
using std::find_if;
using std::queue;

TEST_CASE("City Constructor", "[city][constructor") {
  vec2 bounds(100, 100);
  City city(bounds);

  SECTION("Sets type to kCity") {
    REQUIRE(city.GetType() == Location::Type::kCity);
  }

  SECTION("Creates city with population size") {
    REQUIRE(city.GetIndividuals().size() ==
            Configuration::kDefaultPopulationSize);
  }

  SECTION("Creates city with correct number of sick") {
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

TEST_CASE("City Update", "[city][lifecycle][update]") {
  vec2 bounds(100, 100);

  SECTION("Updates all individuals") {
    City city(bounds, 5, 2);

    vector<Individual> original = city.GetIndividuals();
    city.Update();
    for (const Individual& individual : city.GetIndividuals()) {
      const Individual& og =
          *find_if(original.begin(), original.end(),
                   [individual](const Individual& og_ind) {
                     return og_ind.GetID() == individual.GetID();
                   });
      REQUIRE(og.GetPosition() != individual.GetPosition());
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

    City city(bounds, individuals);
    city.Update();

    for (const Individual& individual : city.GetIndividuals()) {
      REQUIRE_FALSE(individual.GetStatus() == Individual::Status::kUninfected);
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

    City city(bounds, individuals);
    city.Update();

    for (const Individual& individual : city.GetIndividuals()) {
      REQUIRE_FALSE(individual.GetStatus() == Individual::Status::kUninfected);
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

    City city(bounds, individuals);
    city.Update();

    for (const Individual& individual : city.GetIndividuals()) {
      if (individual.GetID() == i_4) {
        REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
      } else {
        REQUIRE_FALSE(individual.GetStatus() ==
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

    City city(bounds, individuals);
    city.Update();

    for (const Individual& individual : city.GetIndividuals()) {
      if (individual.GetID() == i_1) {
        REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
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

    City city(bounds, individuals);
    city.Update();

    for (const Individual& individual : city.GetIndividuals()) {
      if (individual.GetID() == i_1) {
        REQUIRE(individual.GetStatus() == Individual::Status::kUninfected);
      }
    }
  }
}

TEST_CASE("City Add", "[city][interaction][add]") {
  vec2 bounds(100, 100);
  vector<Individual> individuals{Individual(bounds), Individual(bounds),
                                 Individual(bounds)};

  SECTION("Adds empty vector of individuals to empty city") {
    City city(bounds, 0, 0);
    city.Add(vector<Individual>{});
    REQUIRE(city.GetIndividuals().empty());
  }

  SECTION("Adds filled vector of individuals to empty city") {
    City city(bounds, 0, 0);
    city.Add(individuals);
    REQUIRE(city.GetIndividuals() == individuals);
  }

  SECTION("Adds empty vector of individuals to filled city") {
    City city(bounds, 5, 1);
    vector<Individual> original = city.GetIndividuals();
    city.Add(vector<Individual>{});
    REQUIRE(city.GetIndividuals() == original);
  }

  SECTION("Adds filled vector of individuals to filled city") {
    City city(bounds, 5, 1);
    vector<Individual> expected = city.GetIndividuals();
    expected.insert(expected.end(), individuals.begin(), individuals.end());
    city.Add(individuals);
    REQUIRE(city.GetIndividuals() == expected);
  }
}

TEST_CASE("City ExtractIndividualsAt", "[city][interaction][extract]") {
  vec2 bounds(100, 100);

  SECTION("Extracts individual exactly at position") {
    City city(bounds, 1, 1);
    vector<Individual> expected = city.GetIndividuals();
    REQUIRE(city.ExtractIndividualsAt(expected[0].GetPosition()) == expected);
  }

  SECTION("Extracts individual within radius of position") {
    City city(bounds, 1, 1);
    vector<Individual> expected = city.GetIndividuals();
    vec2 position(expected[0].GetPosition().x +
                      0.9 * Configuration::kDefaultIndividualRadius,
                  expected[0].GetPosition().y);
    REQUIRE(city.ExtractIndividualsAt(position) == expected);
  }

  SECTION("Does not extract individuals outside of radius of position") {
    City city(bounds, 1, 1);
    vector<Individual> expected = city.GetIndividuals();
    vec2 position(expected[0].GetPosition().x +
                      1.1 * Configuration::kDefaultIndividualRadius,
                  expected[0].GetPosition().y);
    REQUIRE(city.ExtractIndividualsAt(position).empty());
  }

  SECTION("Does not extract individuals at radius of position") {
    City city(bounds, 1, 1);
    vector<Individual> expected = city.GetIndividuals();
    vec2 position(
        expected[0].GetPosition().x + Configuration::kDefaultIndividualRadius,
        expected[0].GetPosition().y);
    REQUIRE(city.ExtractIndividualsAt(position).empty());
  }

  SECTION("Extracts multiple individuals within radius of position") {
    City city(bounds, 0, 0);
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(position);
    source[1].SetPosition(position);
    source[2].SetPosition(bounds);
    city.Add(source);

    REQUIRE(city.ExtractIndividualsAt(position) ==
            vector<Individual>{source[0], source[1]});
  }

  SECTION("Removes extracted individuals from city individuals") {
    City city(bounds, 0, 0);
    vector<Individual> source(3, Individual(bounds));
    vec2 position(5, 5);
    source[0].SetPosition(bounds);
    source[1].SetPosition(position);
    source[2].SetPosition(position);
    city.Add(source);

    city.ExtractIndividualsAt(position);
    REQUIRE(city.GetIndividuals() == vector<Individual>{source[0]});
  }
}

}  // namespace epidemic