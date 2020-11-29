#pragma once

#include <cinder/gl/gl.h>

namespace epidemic {

using glm::vec2;
using std::vector;

class Individual;

class Location {
 public:
  enum Type {
    kCity, kIsolation, kEmergency
  };

  struct Data {
    vec2 size;
    vec2 offset;
  };

  // Constructors
  explicit Location(Type type, const vec2& bounds);

  // Interaction
  // TODO May need to test here too
  virtual vector<Individual*> Add(const vector<Individual*>& individuals);
  virtual vector<Individual*> ExtractIndividualsAt(const vec2& position);

  // Lifecycle
  virtual void Update();
  virtual void Draw(const vec2& offset) const;

  // Getters & Setters
  virtual Type GetType() const;
  virtual const vec2& GetBounds() const;
  virtual const vector<Individual*>& GetIndividuals() const;

 protected:
  Type type_;
  vec2 bounds_;
  vector<Individual*> individuals_;
};

}  // namespace epidemic
