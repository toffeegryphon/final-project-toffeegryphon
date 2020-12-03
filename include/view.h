#pragma once

#include <cinder/app/App.h>

namespace epidemic {

using ci::app::MouseEvent;

// https://stackoverflow.com/questions/4757565/what-are-forward-declarations-in-c
class SceneManager;

class View {
 public:
  explicit View(SceneManager* manager);
  View(SceneManager* manager, bool with_im_gui);

  virtual void Setup();
  virtual void Update();
  void DrawExternal();
  virtual void MouseDown(MouseEvent event);
  virtual void MouseUp(MouseEvent event);
  virtual void MouseMove(MouseEvent event);
  virtual void MouseDrag(MouseEvent event);

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

  virtual void Draw();
};

}  // namespace epidemic