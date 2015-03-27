#include "Game.h"
#include <GameObject.h>
#include <SDL.h>
#include <math.h>
#include <SDL_mixer.h> 
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <InputManager.h>

#include <string>
#include "Cube.h"
#include "../Player.h"
#include "../Enemy.h"
#include <Cameras/Camera.h>
#include <Cameras/PerspectiveCamera.h>
#include <Cameras/OrthographicCamera.h>

// Initializing our static member pointer.
GameEngine* GameEngine::_instance = nullptr;

GameEngine* GameEngine::CreateInstance()
{
  if (_instance == nullptr)
  {
    _instance = new Game();
  }
  return _instance;
}

Game::Game() : GameEngine()
{

}

Game::~Game()
{
}

void Game::InitializeImpl()
{
  SDL_SetWindowTitle(_window, "Game");
  _lives = 3;
  _tilesColored = 0;
  _levelNumber = 1;
  float nearPlane = 0.01f;
  float farPlane = 100.0f;
  Vector4 position(3.5f, 2.5f, 2.5f, 0.0f);
  Vector4 lookAt = Vector4::Normalize(Vector4::Difference(Vector4(0.0f, 0.0f, 0.0f, 0.0f), position));
  Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

  //_camera = new PerspectiveCamera(100.0f, 1.0f, nearPlane, farPlane, position, lookAt, up);
  _camera = new OrthographicCamera(-10.0f, 10.0f, 10.0f, -10.0f, nearPlane, farPlane, position, lookAt, up);

  int _gridWidth = 5;
  int _gridHeight = 5;

  //Creating a new cube to be added
  for (int gridX = 0; gridX < _gridWidth; gridX++)
  {
	  for (int gridZ = 0; gridZ < _gridHeight; gridZ++)
	  {
	  
		  Cube *newCube = new Cube(Vector3(0.0f, 0.0f, 0.0f));

		  //World coordinates
		  float worldX = gridX;
		  float worldY = (-gridX + gridZ);
		  float worldZ = -gridZ;

		  //Setting position based on world coordinates
		  newCube->GetTransform().position = Vector3(worldX, worldY, worldZ);

		  _objects.push_back(newCube);
	  }
  }
  _enemyNumber = _levelNumber;
  _enemyList = new Enemy*[_enemyNumber];
  for (int i = 0; i < _enemyNumber; i++)
	  _enemyList[i] = nullptr;

  _player = new Player(Vector3(0.0f, 1.0f, 0.0f));
  _objects.push_back(_player);
 // _objects.push_back(new Cube(Vector3(0.0f, 2.0f, 0.0f)));
  //_objects.push_back(new Cube(Vector3(1.0f, 1.0f, 0.0f)));
 // _objects.push_back(new Cube(Vector3(1.0f, 0.0f, 1.0f)));
  //_objects.push_back(new Cube(Vector3(0.0f, 1.0f, 1.0f)));

  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Initialize(_graphicsObject);
  }
  spawnTimer.Start();
  _index = 0;
}

void Game::UpdateImpl(float dt)
{
	std::string title = "Awesome Game ---- Lives: " + std::to_string(_lives) + "  Level: " + std::to_string(_enemyNumber);

	SDL_SetWindowTitle(_window, title.c_str());
  //SDL_Event evt;
  //SDL_PollEvent(&evt);
  InputManager::GetInstance()->Update(dt);
  spawnTimer.Update();

  if (_tilesColored == 25)
	  NextLevel();

  for (int i = 0; i < _enemyNumber; i++)
  {
	  if ((_enemyList[i] == nullptr) && spawnTimer.GetElapsedTime() >= 3)
	  {
		  _enemyList[i] = new Enemy();
		  _enemyList[i]->Initialize(_graphicsObject);
		  _objects.push_back(_enemyList[i]);
		  spawnTimer.Stop();
		  spawnTimer.Start();
	  }
		  
  }

  if (_player->GetState() == DEAD)
  {
	  _lives--;
	  if(_lives < 0) Reset();
	  _player->GetTransform().position = Vector3(0.0f,1.0f,0.0f);
	  _index = 0;
	  _player->GetState() = IDLE;
  }

  if (InputManager::GetInstance()->GetKeyState(SDLK_t, SDL_KEYUP) == true)
  {
	  
	  _index = GetPathIndex(UP);
	  _player->Move(UP);
  }
  if (InputManager::GetInstance()->GetKeyState(SDLK_h, SDL_KEYUP) == true)
  {
	  
	  _index = GetPathIndex(DOWN);
	  _player->Move(DOWN);
	  
  }

  if (InputManager::GetInstance()->GetKeyState(SDLK_g, SDL_KEYUP) == true)
  {
	  
	  _index = GetPathIndex(LEFT);
	  _player->Move(LEFT);
  }

  if (InputManager::GetInstance()->GetKeyState(SDLK_y, SDL_KEYUP) == true)
  {
	  
	  _index = GetPathIndex(RIGHT);
	  _player->Move(RIGHT);
  }


  if (_index != -1)
  {
	  if (dynamic_cast<Cube*>(_objects[_index])->IsColored() == false)
	  {
		  dynamic_cast<Cube*>(_objects[_index])->SetColor(0.0f, 1.0f, 0.0f, 1.0f);
		  dynamic_cast<Cube*>(_objects[_index])->IsColored() = true;
		  _tilesColored++;
	  }
  }
  else
  {
	  if (_player->GetState() == IDLE)
		_player->Fall(Vector3(0,-15,0));
  }

  if (InputManager::GetInstance()->GetKeyState(SDLK_UP, SDL_KEYUP) == true)
  {
    Vector4 position = _camera->GetPosition();
    position.y += 1.0f;

    Vector4 lookAt = Vector4::Normalize(Vector4::Difference(Vector4(1.0f, 0.0f, 0.0f, 0.0f), position));

    _camera->SetPosition(position);
    _camera->SetLookAtVector(lookAt);
  }
  else if (InputManager::GetInstance()->GetKeyState(SDLK_DOWN, SDL_KEYUP) == true)
  {
    Vector4 position = _camera->GetPosition();
    position.y -= 1.0f;

    Vector4 lookAt = Vector4::Normalize(Vector4::Difference(Vector4(1.0f, 0.0f, 0.0f, 0.0f), position));

    _camera->SetPosition(position);
    _camera->SetLookAtVector(lookAt);
  }

  bool isOnCube = false;

  //For each enemy
  for (int i = 0; i < _enemyNumber; i++)
  {		
	  //If exists
	  if (_enemyList[i] != nullptr)
	  {
		  //If same place as the player, player dies
		  if (_player->GetTransform().position.x == _enemyList[i]->GetTransform().position.x && _player->GetTransform().position.y == _enemyList[i]->GetTransform().position.y && _player->GetTransform().position.z == _enemyList[i]->GetTransform().position.z)
		  {
			  _player->GetState() = DEAD;
		  }


	  //The following code will check if the enemies are on the grid
	  if ((_enemyList[i]->GetState() == IDLE || _enemyList[i]->GetState() == DEAD))
	  {
		  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
		  {
			  Vector3 objPos = (*itr)->GetTransform().position;
			  if (Cube* cube = dynamic_cast<Cube*>(*itr))
			  {
				  if (objPos.x == _enemyList[i]->GetTransform().position.x  && objPos.y == _enemyList[i]->GetTransform().position.y - 1 && objPos.z == _enemyList[i]->GetTransform().position.z)
				  {
					  isOnCube = true;
				  }
				  
			  }

		  }
		  if (isOnCube == false)
		  {
			  if (_enemyList[i]->GetState() == DEAD)
			  {
				  _enemyList[i]->ResetPosition();
			  }else
			  _enemyList[i]->Fall(Vector3(0, -15, 0));
			  
				  
		  }
		  else isOnCube = false;
	  }
	  }
  }
  


  for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
  {
    (*itr)->Update(dt);
  }
}

void Game::DrawImpl(Graphics *graphics, float dt)
{
  std::vector<GameObject *> renderOrder = _objects;
  //CalculateDrawOrder(renderOrder);

  glPushMatrix();

  CalculateCameraViewpoint();

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++)
  {
    (*itr)->Draw(graphics, _camera->GetProjectionMatrix(), dt);
  }

  glPopMatrix();
}

void Game::CalculateDrawOrder(std::vector<GameObject *>& drawOrder)
{
  // SUPER HACK GARBAGE ALGO.
  drawOrder.clear();

  auto objectsCopy = _objects;
  auto farthestEntry = objectsCopy.begin();
  while (objectsCopy.size() > 0)
  {
    bool entryFound = true;
    for (auto itr = objectsCopy.begin(); itr != objectsCopy.end(); itr++)
    {
      if (farthestEntry != itr)
      {
        if ((*itr)->GetTransform().position.y < (*farthestEntry)->GetTransform().position.y)
        {
          entryFound = false;
          farthestEntry = itr;
          break;
        }
      }
    }

    if (entryFound)
    {
      GameObject *farthest = *farthestEntry;

      drawOrder.push_back(farthest);
      objectsCopy.erase(farthestEntry);
      farthestEntry = objectsCopy.begin();
    }
  }
}

void Game::CalculateCameraViewpoint()
{
  Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
  Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
  Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);

  Vector3 cameraVector(_camera->GetLookAtVector().x, _camera->GetLookAtVector().y, _camera->GetLookAtVector().z);
  Vector3 lookAtVector(0.0f, 0.0f, -1.0f);

  Vector3 cross = Vector3::Normalize(Vector3::Cross(cameraVector, lookAtVector));
  float dot = MathUtils::ToDegrees(Vector3::Dot(lookAtVector, cameraVector));

  glRotatef(cross.x * dot, 1.0f, 0.0f, 0.0f);
  glRotatef(cross.y * dot, 0.0f, 1.0f, 0.0f);
  glRotatef(cross.z * dot, 0.0f, 0.0f, 1.0f);

  glTranslatef(-_camera->GetPosition().x, -_camera->GetPosition().y, -_camera->GetPosition().z);
}

//This will calculate if the player will be on a cube or not, in the next movement
int Game::GetPathIndex(Direction direction)
{
	int index = 0;
	float x = 0, y = 0, z = 0;

	if (direction == LEFT)
	{
		y = -2;
		z = 1;
	}
	else if (direction == RIGHT)
	{
		z = -1;
		// y = -2;
	}
	else if (direction == UP)
	{
		//	y = 1;
		x = -1;
	}
	else if (direction == DOWN)
	{
		x = 1;
		y = -2;
	}
	Vector3 playerPos = _player->GetTransform().position;
	for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
	{
		Vector3 objPos = (*itr)->GetTransform().position;
		if (Cube* cube = dynamic_cast<Cube*>(*itr))
		{
			if (objPos.x == playerPos.x + x && objPos.y == playerPos.y + y && objPos.z == playerPos.z + z)
			{
				return index;
			}
		}
		index++;
	}
	return -1;
}

void Game::Reset()
{
	_lives = 3;
	
	_tilesColored = 0;
	for (int i = 0; i < _enemyNumber; i++)
	{
		if (_enemyList[i] != nullptr)
		{
			_objects.pop_back();
			delete _enemyList[i];
			_enemyList[i] = nullptr;
		}
			
	}
	_levelNumber = _enemyNumber = 1;
	delete _enemyList;
	_enemyList = new Enemy*[_enemyNumber];
	for (int i = 0; i < _enemyNumber; i++)
	{
		_enemyList[i] = nullptr;
	}
	for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
	{
		Vector3 objPos = (*itr)->GetTransform().position;
		if (Cube* cube = dynamic_cast<Cube*>(*itr))
		{
			dynamic_cast<Cube*>(*itr)->SetColor(1.0f,0.0f,0.0f,1.0f);
			dynamic_cast<Cube*>(*itr)->IsColored() = false;
		}
	}
	
}

void Game::NextLevel()
{
	_lives = 3;
	_tilesColored = 0;
	for (int i = 0; i < _enemyNumber; i++)
	{
		if (_enemyList[i] != nullptr)
		{
			_objects.pop_back();
			delete _enemyList[i];
			_enemyList[i] = nullptr;
		}

	}
	delete _enemyList;
	_enemyList = new Enemy*[_enemyNumber + 1];
	for (int i = 0; i < _enemyNumber + 1; i++)
	{
		_enemyList[i] = nullptr;
	}
	for (auto itr = _objects.begin(); itr != _objects.end(); itr++)
	{
		Vector3 objPos = (*itr)->GetTransform().position;
		if (Cube* cube = dynamic_cast<Cube*>(*itr))
		{
			dynamic_cast<Cube*>(*itr)->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
			dynamic_cast<Cube*>(*itr)->IsColored() = false;
		}
	}

	_levelNumber = ++_enemyNumber;

}