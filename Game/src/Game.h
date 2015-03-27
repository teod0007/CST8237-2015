#pragma once

#include <GameEngine.h>

union SDL_Event;
class Graphics;
class Camera;
class Player;
class Enemy;

class Game: public GameEngine
{
  friend class GameEngine;

public:
  ~Game();

protected:
  Game();

  void InitializeImpl();
  void UpdateImpl(float dt);
  void DrawImpl(Graphics *graphics, float dt);
  //Resets the game
  void Reset();
  //Changes to next level (number of enemies will increase)
  void NextLevel();
  void CalculateDrawOrder(std::vector<GameObject *>& drawOrder);
  void CalculateCameraViewpoint();
  int GetPathIndex(Direction direction);

  int _enemyNumber;
  int _levelNumber;
  int _tilesColored;
  int _lives;
  int _index;
  Timer spawnTimer;
  Camera *_camera;
  Player *_player;
  Enemy **_enemyList;
};