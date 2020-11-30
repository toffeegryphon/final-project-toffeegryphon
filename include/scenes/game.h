#pragma once

#include <city.h>
#include <hand.h>
#include <isolation.h>
#include <location.h>
#include <scene_manager.h>
#include <view.h>

namespace epidemic {

using std::vector;

class Game : public View {
 public:
  explicit Game(SceneManager* manager);

  // Lifecycle

  void Setup() override;
  void Update() override;
  void Draw() override;
  void MouseDown(MouseEvent event) override;
  void MouseUp(MouseEvent event) override;
  void MouseDrag(MouseEvent event) override;
  void MouseMove(MouseEvent event) override;

  // Getters & Setters
  const vector<Individual>& GetIndividuals() const;
  const City& GetCity() const;
  const vector<pair<Isolation, Location::Data>>& GetIsolations() const;

 private:
  // Cannot be reordered
  vector<Individual> individuals_;

  Location::Data kCityData{vec2{500, 500}, vec2{20, 20}};
  City city_;

  // TODO Vector tie to each Isolation
  Location::Data kTemplateIsolationData{vec2{200, 100}, vec2{20, 20}};
  // TODO Maybe isolation manager
  vector<pair<Isolation, Location::Data>> isolations_;
  Hand hand_;

  void GenerateIndividuals();
  void GenerateIsolations();
  bool HasEnded();
};

}  // namespace epidemic