#include <route.h>
#include <utils.h>

namespace epidemic {

using glm::length;
using utils::GetRandomPosition;

// Public Constants

const pair<size_t, size_t> Route::kQueueSize{1, 5};

// Constructors

Route::Route(const vec2& bounds) : Route(bounds, GetRandomPosition(bounds)) {
}

Route::Route(const vec2& bounds, const vec2& position)
    : position_(position), mode_(kContinuous) {
  FillDestinations(bounds);
}

// Lifecycle
void Route::Update(float speed, const vec2& bounds) {
  switch (mode_) {
    case kContinuous:
      MoveTowards(speed, destinations_.front());
      if (destinations_.size() <= kQueueSize.first) {
        FillDestinations(bounds);
      }
      break;
    case kDeplete:
      if (!destinations_.empty()) {
        MoveTowards(speed, destinations_.front());
      }
      break;
  }
}

// Getters & Setters

Route::Mode Route::GetMode() const {
  return mode_;
}

void Route::SetMode(Route::Mode mode) {
  mode_ = mode;
}

const vec2& Route::GetPosition() const {
  return position_;
}

void Route::SetPosition(const vec2& position) {
  position_ = position;
}

const queue<vec2>& Route::GetDestinations() const {
  return destinations_;
}

void Route::SetDestinations(const queue<vec2>& destinations) {
  destinations_ = destinations;
}

// Private

void Route::FillDestinations(const vec2& bounds) {
  // TODO Implement wanderlust here
  for (size_t i = destinations_.size(); i < kQueueSize.second; ++i) {
    destinations_.push(GetRandomPosition(bounds));
  }
}

void Route::MoveTowards(float speed, const vec2& destination) {
  vec2 to_travel = destination - position_;
  float travel_ratio = speed / length(to_travel);
  position_ =
      (travel_ratio >= 1) ? destination : position_ + to_travel * travel_ratio;

  // TODO Implement pos and revert to old method (min of 1 and ratio)
  if (position_ == destination) {
    destinations_.pop();
  }
}

}  // namespace epidemic