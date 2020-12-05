#pragma once

#include <scene_manager.h>
#include <view.h>

namespace epidemic {

class Menu : public View {
 public:
  /**
   * Initializes Menu with parent manager
   * @param manager Of this
   */
  explicit Menu(SceneManager* manager);

  /**
   * Draws start button and Configuration options from
   * Configuration::kProperties
   */
  void Draw() override;
};

}  // namespace epidemic
