#include <scene_manager.h>
#include <view.h>

#include <catch2/catch.hpp>

namespace epidemic {

using std::logic_error;
using std::make_unique;

TEST_CASE("SceneManager HasScene") {
  SceneManager manager;
  SECTION("Returns true if has scene") {
    manager.SetScene(make_unique<View>(&manager));
    REQUIRE(manager.HasScene());
  }

  SECTION("Returns false if no scene") {
    REQUIRE_FALSE(manager.HasScene());
  }
}

TEST_CASE("SceneManager GetScene") {
  SceneManager manager;
  SECTION("Returns scene if has scene") {
    View view(&manager);
    manager.SetScene(make_unique<View>(view));
    REQUIRE(manager.GetScene() == view);
  }

  SECTION("Throws logic_error if no scene") {
    REQUIRE_THROWS_AS(manager.GetScene(), logic_error);
  }
}

TEST_CASE("SceneManager SetScene") {
  SceneManager manager;

  SECTION("Sets scene to empty manager") {
    View view(&manager);
    manager.SetScene(make_unique<View>(view));
    REQUIRE(manager.GetScene() == view);
  }

  SECTION("Sets scene to manager with scene") {
    View v_1(&manager), v_2(&manager);
    manager.SetScene(make_unique<View>(v_1));
    manager.SetScene(make_unique<View>(v_2));
    REQUIRE(manager.GetScene() == v_2);
  }
}

}  // namespace epidemic
