#pragma once

#include <scene_manager.h>
#include <view.h>

namespace epidemic {

class Menu : public View {
 public:
  /**
   * Initializes Menu with parent manager
   *
   * @param manager Of this
   */
  explicit Menu(SceneManager* manager);

  /**
   * Draws start button and cfg options from cfg::kProperties
   */
  void Draw() override;

 private:
  /**
   * Draws each Property as editable settings
   */
  void DrawSettings();
};

}  // namespace epidemic
