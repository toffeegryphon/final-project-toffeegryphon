#pragma once

#include <scene_manager.h>
#include <view.h>

namespace epidemic {

class Menu : public View {
 public:
  explicit Menu(SceneManager* scene_manager);
  void Update() override;
  void Draw() override;
  void MouseDown(MouseEvent event) override;
};

}  // namespace epidemic
