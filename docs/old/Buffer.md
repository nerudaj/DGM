# Buffer Class

## Table of Contents
1. [Introduction](#introduction)
2. [Usage](#usage)
    1. [Initialization](#initialization)
    2. [Adding objects](#adding-objects)
    3. [Updating objects](#updating-objects)

## Introduction

Buffer is a storage object designed for use-case where speed is important. Buffer 
can insert and delete items in a constant time. Due to this, order of items within
storage can randomly change and the storage size is limited and cannot be resized
without discarding all the data first.

## Usage

### Initialization

Let's say we have a class named `Projectile`. We desperadely need a storage for 
projectiles. Those will be created and deleted in large quantities, but we don't 
care about their order since they all will be updated in a single frame.

We can create a `Buffer` of those items like this:
```c++
dgm::Buffer<Projectile> projectiles;

projectiles.Init(256);
for (int i = projectiles.Begin(), i < projectiles.Capacity(); i++) {
    projectiles[i].myInitMethodHere();
}
```

This syntax will create a storage object, initialize it for 256 objects and then
initialize each object (loading sprites, registering callbacks and so on). Remember, 
all 256 projectile objects are initialized in a memory on `Buffer::Init()`, so 
initializing all those objects right after that call is recommended.

### Adding objects

In the previous section, we've created 256 objects, but from the game logic perspective,
these objects don't exist. This is because `Buffer` has two properties: `capacity` 
and `size`. First tells us how many objects can we have and second tells us how 
many we already have.

So how to affect value of size? Easy:

```c++
if (projectiles.Add()) {
    projectiles.GetLast().Spawn(...);
}
```

By calling `Buffer::Add()`, we've requested the `Buffer` object to give us an object.
If add returns true, it means that capacity was not reached and we got that object.
It's index in that exact moment will be `Buffer::End() - 1`, or we can simply
grab it by reference by calling `Buffer::GetLast()`. Now it's wise to call any
custom spawn method and that projectile just became part of our game.

### Updating objects

But we need to update, draw and remove those objects, so let's address it now. In
our update logic, we'll need this loop:

```c++
for (int i = projectiles.Begin(), i < projectiles.End(); i++) {
    projectiles[i].Update();
}
```

Now assume that `Projectile::Update()` returns boolean value and if it returns false,
we should delete that projectile:

```c++
for (int i = projectiles.Begin(), i < projectiles.End(); i++) {
    if (not projectiles[i].Update()) {
        projectiles.Remove(i);
        i--;
    }
}
```

This might be confusing at start, but it's the same principle that would apply
with std::vector::erase() method used in a loop.