# Animation Class

## Table of Contents


## Introduction

Animation object let's you easily animate your sprites. Each animation contains
a database of states, basically a bunch of named `Clip` objects and has a sprite
bound to it. With each call to `Update()`, `Animation` will increment its
internal counters, which may result into change of frame (call to `sf::Sprite::setTextureRect()`).

## Prerequisities

The most basic prerequisity for animating is to have all your animations in a 
single tileset image which can then be described using `Clip` objects. Once you
have this, you can load an image as a texture and bind it to your sprite. Then,
bind the sprite to the animation:

```c++
dgm::Animation anim;
sf::Sprite sprite;
sf::Texture texture;
// ... load texture;
sprite.setTexture(texture);
anim.setSprite(&sprite);
```

Now we have to build the database of animation states. Let's consider following
image:

[Image here]

There are two animation states: rolling while being white and rolling while being
red. The image is 512x256 and each frame is 128x128. Let's build our database:

```c++
dgm::AnimationData database;
dgm::Clip clip;

clip.LoadFromParameters({128, 128}, sf::IntRect(0, 0, 512, 128));
database.AddState("RollingWhite", clip);

clip.LoadFromParameters({128, 128}, sf::IntRect(0, 128, 512, 128));
database.AddState("RollingRed", clip);

animation.LoadFromMemory(&database);
```

At this point we can easily set speed of animation and start it:
```c++
animation.SetSpeed(4); // 4 frames per sec
animation.SetState("RollingRed");
```