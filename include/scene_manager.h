#pragma once

#include <view.h>
#include <queue>

namespace epidemic {

using std::queue;
using std::unique_ptr;

class SceneManager {
 public:
  SceneManager();

  bool HasScene() const;
  View& GetScene() const;
  void SetScene(unique_ptr<View>&& scene);

 private:
  // https://en.cppreference.com/w/cpp/memory/unique_ptr
  queue<unique_ptr<View>> scenes_;
};

}  // namespace epidemic