# Example

This folder contains examples DGM usage. If you are building DGM from source, targets for examples are part of the global DGM solution. If you have a release of DGM, you can configure a solution for these examples using following commands (cmake3 is needed):

```
mkdir vsbuild
cd vsbuild
cmake ..
```

You will get solution called dgm-examples.

## Running an example

Please always run examples from within Visual Studio. You can do this by right-clicking the example target and select it as startup project. Then you can use the debugger button to launch the example in question.

## Documentation for each example

### 01_polar_cartesian

This demonstrates conversions between cartesian and polar coordinates. Move the mouse around the screen. Green circle should be positioned under mouse and white line should point in direction of the mouse.

### 02_controller

This example demonstrates how to use dgm::Controller. Use WASD to move the car and click the X button on the window to close the app.

### 03_app_state

This example uses the dgm::AppState. It shows how to implement simple app state. Once again use WASD to move the car and Escape to close the app.

### 04_particle_system

Example implements very simple particle system with no advanced features. It will point in the direction of the mouse.

### 05_game_fifteen

Simple implementation of game fifteen. Click the tiles to move them to neighbouring empty space. You can press R to reset the board and Escape to close the app. This example does not showcase anything particular from DGM, but it shows you can use cfg::Args to parse the command line parameters.

### 06_animation

This one shows how dgm::Animation works. Hold Left or Right arrow to play running animation.

### 07_collisions

Demonstration of basic collisions between rectangle and circle. Use arrows to move the circle. It will turn red whenever it collides with blue rectangle.

### 08_buffer_mesh

Demonstration of dgm::Buffer and collisions between dgm::Circle/dgm::Rect with dgm::Mesh.

### 09_resource_manager

How to load resources with resource manager. It also demonstrates how single database of animation states can be shared between objects in dgm::Buffer.

### 10_tileset

This example showcases how tileset based maps can be rendered using dgm::Tileset and how collisions can be added to it and how to use LevelD to store level data.

### 11_advanced_particle_system

This example shows more complex particle systems, those that require to inherit from implicit data types, require texturing and such. Press P to start the simulation and Space to remove some blocks to further observe effect of falling sand particle system.

### 12_simple_lights

How to illuminate tiles of dgm::TileMap. This example allows you to move a green circle through textured maze rendered via TileMap, and surroundings of the player are illuminated by setting the transparent channel of each tile against black void. Use WASD to move and close the app by clicking close button.