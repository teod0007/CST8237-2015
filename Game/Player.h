#pragma once

#include <GameObject.h>
#include "src\Cube.h"

class Cube;

class Player : public GameObject
{
public:
	Player();
	Player(Vector3 position);

	void Initialize(Graphics *graphics);
	void Update(float dt);
	void Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt);
	void Move(Direction d);
	void Fall(Vector3 destination);
	EntityState& GetState();

protected:
	//void *_vertexPointer;

	Cube *_playerCube;
	EntityState _state;//Used to set the current state of the entity
	Direction _movementDirection;//If the state is "MOVING", which direction it is moving
	float _moveSpeed;
	float _destX, _destY, _destZ;//Where it is going if in movement
};