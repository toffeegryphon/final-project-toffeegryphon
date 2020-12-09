#pragma once

#include <cinder/app/App.h>

namespace epidemic {

using ci::app::KeyEvent;
using ci::app::MouseEvent;

// https://stackoverflow.com/questions/4757565/what-are-forward-declarations-in-c
class SceneManager;

class View {
 public:
  // Constructors

  /**
   * Initializes View with SceneManager pointer and by default without ImGui
   * active
   *
   * @param manager SceneManager to manage this View
   */
  explicit View(SceneManager* manager);

  /**
   * Initializes View with SceneManager pointer and ImGui active if
   * with_im_gui is true
   *
   * @param manager SceneManager to manage this View
   * @param with_im_gui Whether to set ImGui active in drawing this View
   */
  View(SceneManager* manager, bool with_im_gui);

  // Lifecycle

  /**
   * Optional setup hook to be called by Cinder, only called for the very first
   * View
   */
  virtual void Setup(){};

  /**
   * Update hook to be called by Cinder
   */
  virtual void Update(){};

  /**
   * Draw hook that clears screen and calls overridden Draw method, to be called
   * by Cinder
   */
  void DrawExternal();

  /**
   * Optional MouseDown hook to be called by Cinder when a mouse down event is
   * triggered
   *
   * @param event Mouse down MouseEvent
   */
  virtual void MouseDown(MouseEvent event){};

  /**
   * Optional MouseUp hook to be called by Cinder when a mouse up event is
   * triggered
   *
   * @param event Mouse up MouseEvent
   */
  virtual void MouseUp(MouseEvent event){};

  /**
   * Optional MouseMove hook to be called by Cinder when a mouse move event is
   * triggered
   *
   * @param event Mouse move MouseEvent
   */
  virtual void MouseMove(MouseEvent event){};

  /**
   * Optional MouseDrag hook to be called by Cinder when a mouse drag event is
   * triggered
   *
   * @param event Mouse drag MouseEvent
   */
  virtual void MouseDrag(MouseEvent event){};

  /**
   * Optional KeyDown hook to be called by Cinder when a key down event is
   * triggered
   *
   * @param event Key down KeyEvent
   */
  virtual void KeyDown(KeyEvent event){};

  /**
   * Returns unique ID of this View
   *
   * @return Unique ID of this View
   */
  size_t GetID() const;

  bool operator==(const View& rhs) const;
  bool operator!=(const View& rhs) const;

 protected:
  SceneManager* manager_;

 private:
  // ID
  static size_t next_id_;
  static size_t GetNextID();
  size_t id_;

  bool with_im_gui_;

  /**
   * Draw method to override to draw elements in this View, calling clear is not
   * required
   */
  virtual void Draw(){};
};

}  // namespace epidemic