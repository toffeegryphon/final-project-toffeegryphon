#include <route.h>
#include <utils.h>

namespace epidemic {

using utils::GetRandomPosition;
using glm::length;
using std::min;

// Public Constants

const pair<size_t, size_t> Route::kQueueSize{1, 5};

Route::Route(const vec2& bounds) : position_(GetRandomPosition(bounds)) {
  FillDestinations(bounds);
}

// Lifecycle
vec2 Route::Update(float speed, const vec2& bounds) {
  if (destinations_.size() == kQueueSize.first) {
    FillDestinations(bounds);
  }

  const vec2& destination = destinations_.front();
  vec2 to_travel = destination - position_;
  float travel_ratio = min(1.0f, speed / length(to_travel));
  position_ += to_travel * travel_ratio; // TODO Possibly can condense to 1 line

  if (position_ == destination) {
    destinations_.pop();
  }

  return position_;
}

// Getters & Setters

const vec2& Route::GetPosition() const {
  return position_;
}

const queue<vec2>& Route::GetDestinations() const {
  return destinations_;
}

// Private

void Route::FillDestinations(const vec2& bounds) {
  for (size_t i = destinations_.size(); i < kQueueSize.second; ++i) {
    destinations_.push(GetRandomPosition(bounds));
  }
}

}  // namespace epidemic