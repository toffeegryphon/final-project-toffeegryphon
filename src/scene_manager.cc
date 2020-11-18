#include <scene_manager.h>

namespace epidemic {

using std::logic_error;

SceneManager::SceneManager() = default;

bool SceneManager::HasScene() const {
  return !scenes_.empty();
}

View& SceneManager::GetScene() const {
  if (!HasScene()) {
    throw logic_error("Manager has no scenes. Please check HasScene() first.");
  }
  return *scenes_.front();
}

void SceneManager::SetScene(unique_ptr<View>&& scene) {
  scenes_ = queue<unique_ptr<View>>();
  scenes_.push(move(scene));
}

}  // namespace epidemic