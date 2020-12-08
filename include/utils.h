#pragma once

#include <cinder/gl/gl.h>
#include <configuration.h>
#include <individual.h>
#include <location.h>

namespace epidemic {

namespace utils {

using glm::vec2;

// Generation

/**
 * Generates random float between 0.0f and 1.0f, inclusive
 *
 * @return Random float between 0.0f and 1.0f, inclusive
 */
float GetRandom();

/**
 * Generates random vec2 position within bounds
 *
 * @param bounds Vec2 to generate random vec2 within
 * @return Random vec2 within bounds
 */
vec2 GetRandomPosition(const vec2& bounds);

/**
 * Generates random float within range from start
 *
 * @param start_range Vec2 representation of starting value, range from start
 * @return Random float within range from start
 */
float GetRandomInRange(const vec2& start_range);

/**
 * Returns random float between 0.0f and end
 *
 * @param end Maximum value of float to be generated
 * @return Random float between 0.0f and end
 */
float GetRandomInRange(float end);

/**
 * Returns random float representing healthiness within range provided by cfg
 *
 * @return Random float healthiness
 */
float GetRandomHealthiness();

/**
 * Returns random vec2 of chance, rate of change (ROC) with chance within
 * chance_range and ROC within roc_range
 *
 * @param chance_range Vec2 to generate chance within
 * @param roc_range Vec2 to generate ROC within
 * @return Random vec2 representing chance, ROC
 */
vec2 GetRandomChanceROC(const vec2& chance_range, const vec2& roc_range);

/**
 * Returns random vec2 of spread chance, rate of change (ROC) within ranges
 * provided by cfg
 *
 * @return Random vec2 representing spread chance, ROC
 */
vec2 GetRandomSpread();

/**
 * Returns random vec2 of recovery chance, rate of change (ROC) within ranges
 * provided by cfg
 *
 * @return Random vec2 representing recovery chance, ROC
 */
vec2 GetRandomRecovery();

/**
 * Returns random vec2 of death chance, rate of change (ROC) within ranges
 * provided by cfg
 *
 * @return Random vec2 representing death chance, ROC
 */
vec2 GetRandomDeath();

// Calculation & Comparison

/**
 * Calculates x distance between positions of first to second
 *
 * @param first Individual to calculate x distance from
 * @param second Individual to calculate x distance to
 * @return X distance between positions of 2 Individuals
 */
float DistanceX(const Individual& first, const Individual& second);

/**
 * Returns true if the distance from first to second is less than or equal to 0
 *
 * @param first Individual to calculate x distance from
 * @param second Individual to calculate x distance to
 * @return True if the distance from first to second is less than or equal to 0
 */
bool CompareX(const Individual* first, const Individual* second);

/**
 * Returns true if the position is within the offset bounds in data
 *
 * @param position To be checked
 * @param data Of location to obtain offset bounds from
 * @return True if position is within the offset bounds in data
 */
bool IsInLocation(const vec2& position, const Location::Data& data);

/**
 * Converts a vector of Individuals to a vector of Individual pointers
 *
 * @param source To convert from
 * @return Vector of Individual pointers from source
 */
vector<Individual*> ToPointers(vector<Individual>* source);

}  // namespace utils

}  // namespace epidemic