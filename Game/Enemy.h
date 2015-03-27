#pragma once

#include <GameObject.h>
#include "src\Cube.h"
#include <Timer.h>
#include <time.h>
#include <stdlib.h>

class Cube;
class Timer;

class Enemy : public GameObject
{
public:
	Enemy();
	Enemy(Vector3 position);

	void Initialize(Graphics *graphics);
	void Update(float dt);
	void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);
	void Move(Direction d);
	void Fall(Vector3 destination);
	EntityState& GetState();
	void ResetPosition();
protected:
	//void *_vertexPointer;

	Cube *_enemyCube;
	EntityState _state; //Used to set the current state of the entity
	Direction _movementDirection; //If the state is "MOVING", which direction it is moving
	Timer timer; //Timer used for "AI" purposes

	float _moveSpeed;
	float _destX, _destY, _destZ; //Where it is going if in movement
};