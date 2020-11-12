#pragma once

#include <cinder/gl/gl.h>
#include <individual.h>

namespace epidemic {

using epidemic::Individual;
using glm::vec2;
using std::vector;

class Location {
 public:
  Location(const vec2& bounds);

  void Update();
  void Draw(const vec2& offset) const;

 private:
  vec2 bounds_;
  vector<Individual> individuals_;
};

}  // namespace epidemic
