#pragma once

#include <cinder/gl/gl.h>
#include <configuration.h>
#include <individual.h>

namespace epidemic {

namespace utils {

using glm::vec2;

// TODO Figure a way to test Random maybe by fixing seed
float GetRandom();

vec2 GetRandomPosition(const vec2& bounds);

float GetRandomInRange(const vec2& start_range);

float GetRandomInRange(float end);

float GetRandomHealthiness();

float GetRandomWanderlust();

vec2 GetRandomChanceROC(const vec2& chance_range, const vec2& roc_range);

vec2 GetRandomSpread();

// TODO Take healthiness, has_recovered
vec2 GetRandomRecovery();

vec2 GetRandomDeath();

float DistanceX(const Individual& first, const Individual& second);

bool CompareX(const Individual& first, const Individual& second);

}  // namespace utils

}  // namespace epidemic