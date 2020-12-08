#pragma once

#include <cinder/gl/gl.h>

namespace epidemic {

using glm::vec2;
using std::vector;

class Individual;

class Location {
 public:
  /**
   * Location Types
   */
  enum Type { kCity, kIsolation };

  /**
   * Container for size and offset of a Location
   */
  struct Data {
    vec2 size;
    vec2 offset;
  };

  // Constructors

  /**
   * Initializes Location to type with size of bounds
   *
   * @param type Of this
   * @param bounds Of this
   */
  explicit Location(Type type, const vec2& bounds);

  // Interaction
  // TODO May need to test here too

  /**
   * Adds individuals to this, and returns an empty vector of Individual
   * pointers
   *
   * @param individuals To be added to this
   * @return Empty vector of Individual pointers
   */
  virtual vector<Individual*> Add(const vector<Individual*>& individuals);

  /**
   * Removes and returns all Individuals within the radius (of an Individual) of
   * position
   *
   * @param position To remove Individuals from
   * @return Individuals removed
   */
  virtual vector<Individual*> ExtractIndividualsAt(const vec2& position);

  // Lifecycle

  /**
   * Virtual update hook to be overridden and called by parent View
   */
  virtual void Update() {};

  /**
   * Draws this with a background and all the Individuals within this at offset
   *
   * @param offset From origin to draw this at
   */
  virtual void Draw(const vec2& offset) const;

  // Getters & Setters

  virtual Type GetType() const;
  virtual const vec2& GetBounds() const;
  virtual const vector<Individual*>& GetIndividuals() const;

 protected:
  Type type_;
  vec2 bounds_;
  vector<Individual*> individuals_;
  size_t frame_ = 0;
};

}  // namespace epidemic
