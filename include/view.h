#pragma once

#include <cinder/app/App.h>

namespace epidemic {

using ci::app::MouseEvent;

// https://stackoverflow.com/questions/4757565/what-are-forward-declarations-in-c
class SceneManager;

class View {
 public:
  explicit View(SceneManager* manager);

  virtual void Setup();
  virtual void Update();
  virtual void Draw();
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
  static size_t kNextID;
  static size_t GetNextID();

  size_t id_;
};

}  // namespace epidemic