#include <CinderImGui.h>
#include <utils.h>

namespace epidemic {

namespace utils {

using std::rand;

static vector<Individual::Status> statuses{
    Individual::Status::kUninfected,  Individual::Status::kAsymptomatic,
    Individual::Status::kSymptomatic, Individual::Status::kRecovering,
    Individual::Status::kRecovered,   Individual::Status::kDying,
    Individual::Status::kDead};

float GetRandom() {
  return static_cast<float>(rand()) / RAND_MAX;
}

vec2 GetRandomPosition(const vec2& bounds) {
  return vec2{GetRandom() * bounds.x, GetRandom() * bounds.y};
}

float GetRandomInRange(const vec2& start_range) {
  return start_range.x + GetRandom() * start_range.y;
}

float GetRandomInRange(float end) {
  return GetRandomInRange(vec2{0, end});
}

float GetRandomHealthiness() {
  return GetRandomInRange(cfg::kDefaultHealthinessRange);
}

vec2 GetRandomChanceROC(const vec2& chance_range, const vec2& roc_range) {
  return vec2{GetRandomInRange(chance_range), GetRandomInRange(roc_range)};
}

vec2 GetRandomSpread() {
  return GetRandomChanceROC(cfg::kSpreadChanceRange.value,
                            cfg::kSpreadInfectedROCRange.value);
}

vec2 GetRandomRecovery() {
  return GetRandomChanceROC(cfg::kRecoveryChanceRange.value,
                            cfg::kRecoveryROCRange.value);
}

vec2 GetRandomDeath() {
  return GetRandomChanceROC(cfg::kDeathChanceRange.value,
                            cfg::kDeathROCRange.value);
}

Color GetColor(Individual::Status status) {
  Color color;
  switch (status) {
    case Individual::Status::kUninfected:
    case Individual::Status::kAsymptomatic:
      color = Color(0, 0, 1);
      break;
    case Individual::Status::kSymptomatic:
      color = Color(1, 1, 0);
      break;
    case Individual::Status::kDying:
      color = Color(1, 0, 0);
      break;
    case Individual::Status::kDead:
      color = Color(1, 1, 1);
      break;
    case Individual::Status::kRecovering:
      color = Color(0, 1, 1);
      break;
    case Individual::Status::kRecovered:
      color = Color(0, 1, 0);
      break;
  }
  return color;
}

string GetLabel(Individual::Status status) {
  string label;
  switch (status) {
    case Individual::Status::kUninfected:
      label = "Uninfected";
      break;
    case Individual::Status::kAsymptomatic:
      label = "Asymptomatic";
      break;
    case Individual::Status::kSymptomatic:
      label = "Symptomatic";
      break;
    case Individual::Status::kDying:
      label = "Dying (still spreads)";
      break;
    case Individual::Status::kDead:
      label = "Dead (no longer spreads)";
      break;
    case Individual::Status::kRecovering:
      label = "Recovering (still spreads)";
      break;
    case Individual::Status::kRecovered:
      label = "Recovered (no longer spreads)";
      break;
  }
  return label;
}

void DrawIndividualInfo() {
  ImGui::SetNextWindowPos(vec2(0, cfg::kWindowSize.y - cfg::kInfoMargins));
  ImGui::SetNextWindowSize(vec2(cfg::kWindowSize.x, cfg::kInfoMargins));
  ImGui::Begin("individual_info", nullptr,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoResize);

  ImVec2 center(cfg::kInfoMargins,
                cfg::kWindowSize.y - cfg::kInfoMargins / 2.0f);
  ImU32 white = ImGui::GetColorU32(ImVec4(1, 1, 1, 1));

  for (Individual::Status status : statuses) {
    ci::Color color = GetColor(status);
    ImU32 u32 =
        ImGui::ColorConvertFloat4ToU32(ImVec4(color.r, color.g, color.b, 1));
    string label = GetLabel(status);
    ImGui::GetWindowDrawList()->AddCircleFilled(
        center, cfg::kInfoIndividualRadius, u32);
    center.x += cfg::kInfoIndividualRadius * 1.5f;
    ImGui::GetWindowDrawList()->AddText(center, white, label.c_str());

    center.x += ImGui::CalcTextSize(label.c_str()).x +
                cfg::kInfoIndividualRadius * 1.5f;
  }

  ImGui::End();
}

float DistanceX(const Individual& first, const Individual& second) {
  return fabs(first.GetPosition().x - second.GetPosition().x);
}

bool CompareX(const Individual* first, const Individual* second) {
  return first->GetPosition().x < second->GetPosition().x;
}

bool IsInLocation(const vec2& position, const Location::Data& data) {
  return (
      data.offset.x < position.x && position.x < data.offset.x + data.size.x &&
      data.offset.y < position.y && position.y < data.offset.y + data.size.y);
}

vector<Individual*> ToPointers(vector<Individual>* source) {
  vector<Individual*> individual_ptrs;
  individual_ptrs.reserve(source->size());
  for (Individual& individual : *source) {
    individual_ptrs.push_back(&individual);
  }
  return individual_ptrs;
}

}  // namespace utils

}  // namespace epidemic
