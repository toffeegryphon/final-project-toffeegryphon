#pragma once

#include <city.h>
#include <hand.h>
#include <isolation.h>
#include <location.h>
#include <scene_manager.h>
#include <scenes/end.h>
#include <view.h>

namespace epidemic {

using std::vector;

class Game : public View {
 public:
  explicit Game(SceneManager* manager);

  // Lifecycle

  /**
   * Updates all Locations in this if game has yet to end, otherwise set View to
   * End
   */
  void Update() override;

  /**
   * Draws all Locations
   */
  void Draw() override;

  /**
   * Picks individuals in Location at position of event into Hand
   * @param event Mouse down MouseEvent
   */
  void MouseDown(MouseEvent event) override;

  /**
   * Drops all individuals in hand into Location that contains position of
   * event, returning all overflow to City
   *
   * @param event Mouse up MouseEvent
   */
  void MouseUp(MouseEvent event) override;

  /**
   * Updates position of hand to position of event
   *
   * @param event Mouse drag MouseEvent
   */
  void MouseDrag(MouseEvent event) override;

  /**
   * Updates position of hand to position of event
   *
   * @param event Mouse move MouseEvent
   */
  void MouseMove(MouseEvent event) override;

  /**
   * Keyboard commands
   *
   * @param event Key down KeyEvent
   */
  void KeyDown(KeyEvent event) override;

  // Getters & Setters
  const vector<Individual>& GetIndividuals() const;
  const City& GetCity() const;
  const vector<pair<Isolation, Location::Data>>& GetIsolations() const;
  const Hand& GetHand() const;

 private:
  // Cannot be reordered
  vector<Individual> individuals_;

  City city_;

  Location::Data kTemplateIsolationData{vec2{200, 100}, vec2{20, 20}};
  vector<pair<Isolation, Location::Data>> isolations_;
  Hand hand_;

  /**
   * Generates a total of cfg::kPopulationSize individuals with
   * cfg::kStartingSickCount sick
   */
  void GenerateIndividuals();

  /**
   * Generates a total of cfg::kIsolationCount Isolation wards according to
   * template with associated data
   */
  void GenerateIsolations();

  /**
   * Returns true if no more individuals with infected statuses remain
   * (kAsymptomatic, kSymptomatic, kDying, kRecovering)
   *
   * @return True if no more infected individuals remain
   */
  bool HasEnded();

  /**
   * Calculates End Data for game
   *
   * @return End Data for game
   */
  End::Data CalculateEndData();
};

}  // namespace epidemic