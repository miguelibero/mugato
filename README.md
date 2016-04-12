MUGATO
====

![Mugato Creature from Star Trek](http://vignette4.wikia.nocookie.net/memoryalpha/images/5/5e/Mugato.jpg/revision/latest/scale-to-width-down/203?cb=20090202055240&path-prefix=en)

Mugato is a C++11 scene graph on top of the [Gorn renderer](https://github.com/miguelibero/gorn).

Its main goals are:

* entity/component architecture
* support for Linux, Android, iOS and Windows (WIP)

Some functionalities that the library supports:

* octree children container
* sprites
* GUI: text labels, buttons
* Spine skeletal animations
* animations
* lighting

## Example

```c++
auto& ctx = getContext();

ctx.getQueue().addCamera()
    .withProjection(glm::ortho(0.0f, 480.0f, 0.0f, 320.0f));

auto scene = ctx.getScenes().push();

// show FPS and draw calls
scene->addComponent<RenderInfoComponent>();

// load a background sprite to cover the whole screen
auto& bg = scene->addComponent<SpriteComponent>("background.png");
bg.getSprite().setResizeMode(SpriteResizeMode::Exact);

// load a Spine animation
auto spineboy = scene->addChild();
spineboy->getTransform().setScale(0.3f);
spineboy->getTransform().setPosition(glm::vec2(400.0f, 10.0f));
auto& skel = spineboy->addComponent<SpineSkeletonComponent>("spineboy");
skel.getSkeleton().setAnimation("walk");
```
