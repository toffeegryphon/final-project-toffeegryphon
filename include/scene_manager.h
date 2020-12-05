#pragma once

#include <view.h>

#include <queue>

namespace epidemic {

using std::queue;
using std::unique_ptr;

class SceneManager {
 public:
  /**
   * Initializes default SceneManager with no view
   */
  SceneManager();

  /**
   * Checks if this currently contains at least one View
   *
   * @return True if this contains at least one View
   */
  bool HasScene() const;

  /**
   * Returns non-const reference to first View
   *
   * @return First View
   */
  View& GetScene() const;

  /**
   * Clears all Views in this and sets scene as the new View
   *
   * @param scene To be set as the new first View
   */
  void SetScene(unique_ptr<View>&& scene);

 private:
  // https://en.cppreference.com/w/cpp/memory/unique_ptr
  queue<unique_ptr<View>> scenes_;
};

}  // namespace epidemic