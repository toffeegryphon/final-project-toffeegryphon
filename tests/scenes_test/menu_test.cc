#include <cinder/app/MouseEvent.h>
#include <scene_manager.h>
#include <scenes/menu.h>

#include <catch2/catch.hpp>

namespace epidemic {

using ci::app::MouseEvent;
using std::make_unique;

TEST_CASE("Menu MouseDown", "[menu][ui]") {
  SceneManager manager;
  manager.SetScene(make_unique<Menu>(&manager));
  size_t id_menu = manager.GetScene().GetID();

  SECTION("Sets scene to Game on MouseDownEvent") {
    manager.GetScene().MouseDown(MouseEvent());
    REQUIRE_FALSE(manager.GetScene().GetID() == id_menu);
  }
}

}  // namespace epidemic