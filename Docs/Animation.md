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

## State Database

Now we have to build the database of animation states. Let's consider following
image:

![Sprite](rolling.png)

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

## Using

At this point we can easily set speed of animation and start it:
```c++
animation.SetSpeed(4); // 4 frames per sec
animation.SetState("RollingRed");
```

Now you can use the `dgm::Time` class for updating the animation object and after
some time (250ms in this case), sprite will change its appearance.

## Writing Database

Database of animation states can be written outside of your program, in a text
file and then load into the animation object. The syntax of the file is following:
```
frameOffset=0:0
frameSize=64:64

begin
	stateName=RollingWhite
	frameSize=128:128
	boundaries=0:0:256:128
end

begin
	stateName=RollingRed
	frames=7
	boundaries=0:0:256:128
end
```

Beginning section of the file specifies globally valid settings - `frameOffset` 
and `frameSize`. Those values defined here will apply to all states, unless you 
redefine that value in the state definition. Also, if you don't specify offset
at all, it will default to (0, 0).

After the header section there is a bunch of begin-end blocks, each defining
single state. You have to define state name and boundaries. Optionally, you can
specify number of frames, size of the frames and their offset.

Syntax of every vector is `X_COORD:Y_COORD`, boundaries specify values for `sf::IntRect`
with syntax `LEFT_OFFSET:TOP_OFFSET:WIDTH:HEIGHT`.

File defined according to rules written can be loaded directly into `Animation`
object, or it can be loaded into `AnimationData` object and then you can use this
to initialize multiple `Animation` objects, sharing the same memory for state
database. This approach is used when loaded with `ResourceManager`.