#include <configuration.h>

namespace epidemic {
vec2 Configuration::kDefaultWindowSize{800, 800};

vec2 Configuration::kDefaultSpreadChanceRange{0.01, 0.3};
vec2 Configuration::kDefaultSpreadInfectedROCRange{0.0001, 0.0005};
vec2 Configuration::kDefaultSpreadRecoveredROCRange{-0.0005, -0.0001};

vec2 Configuration::kDefaultRecoveryChanceRange{0.00, 0.000001};
vec2 Configuration::kDefaultRecoveryROCRange{0.000001, 0.000001};

vec2 Configuration::kDefaultDeathChanceRange{0.0000001, 0.00001};
vec2 Configuration::kDefaultDeathROCRange{0.00000, 0.0000001};

vec2 Configuration::kDefaultHealthinessRange{-0.5, 0.3};
vec2 Configuration::kDefaultWanderlustRange{0.9, 3};
}  // namespace epidemic