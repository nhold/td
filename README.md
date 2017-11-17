# Tower Defense
2D TD game in C++ with the SFML library.


![td.png](td.png)

## TODO

* ~~Tower cost~~
* ~~Base getting destroyed~~
* Game End state (Win\Lose\Retry)
* Waves (Loaded from file etc)
* Refactor the path follow logic
* Create actual A* from path logic
* Optimise tilemap
* ~~Everything should use node~~

### Code Standards

Argument in class methods should just be the name, use this-> to differentiate between them. E.g `this->health = health`.