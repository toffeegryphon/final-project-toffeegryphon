#pragma once

#include <scene_manager.h>
#include <view.h>

namespace epidemic {

class Win : public View {
 public:
  struct Data {
    size_t uninfected;
    size_t recovered;
    size_t dead;
  };

  Win(SceneManager* manager, const Data& data);

  // Lifecycle

  void Setup() override;
  void Update() override;
  void Draw() override;
  void MouseDown(MouseEvent event) override;

 private:
  Data data_;
};

}  // namespace epidemic
