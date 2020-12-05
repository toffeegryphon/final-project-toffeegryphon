#pragma once

#include <scene_manager.h>
#include <view.h>

namespace epidemic {

class End : public View {
 public:
  /**
   * Container for ending result
   */
  struct Data {
    size_t uninfected;
    size_t recovered;
    size_t dead;
  };

  // Constructors

  /**
   * Initializes End with parent manager and data
   *
   * @param manager Of this
   * @param data Of ending result
   */
  End(SceneManager* manager, const Data& data);

  // Lifecycle

  /**
   * Draws end game message and button to bring player back to Menu
   */
  void Draw() override;

 private:
  Data data_;
};

}  // namespace epidemic
