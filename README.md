# Tower Defense
2D TD game in C++ with the SFML library.


![td.png](td.png)

## TODO

* ~~Tower cost~~
* ~~Base getting destroyed~~
* ~~Game\Menu State~~
* ~~Waves (Loaded from file etc)~~
* ~~Everything should use node~~
* ~~Projectiles~~
* Buttons
* Level Selection UI
* Game Over UI
* Flying Enemies
* First 5 levels
* Update graphics again
* Add more enemy types
* Load enemy types from file

### Code Standards

Argument in class methods should just be the name, use this-> to differentiate between them. E.g `this->health = health`.

### Code Structure

The entry point is in `main.cpp` but it just calls and uses `TD` found in `td.hpp\td.cpp` which inherits from `Game` found in `game.hpp\game.cpp`