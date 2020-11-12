#include <location.h>

namespace epidemic {

epidemic::Location::Location(const vec2& bounds) : bounds_(bounds) {
}

void Location::Update() {
}

void Location::Draw(const vec2& offset) const {
  for (const Individual& individual : individuals_) {
    individual.Draw(offset);
  }
}

}  // namespace epidemic
