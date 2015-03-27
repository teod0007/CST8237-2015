
#include "Enemy.h"

Enemy::Enemy()
{
	_transform.position = Vector3(0.0f, 0.0f, 0.0f);
}
Enemy::Enemy(Vector3 position)
{
	_transform.position = position;
}


void Enemy::Initialize(Graphics *graphics)
{
	srand(time(NULL));

	int position = rand() % 3;

	switch (position)
	{
	case 0: _transform.position = Vector3(0.0f, 15.0f, -4.0f);
			break;
	case 1:_transform.position = Vector3(0.0f, 14.0f, -3.0f);
			break;
	case 2:_transform.position = Vector3(1.0f, 14.0f, -4.0f);
			break;
	}

	_enemyCube = new Cube(_transform.position);
	_enemyCube->GetTransform().scale = Vector3(0.6f, 0.6f, 0.6f);
	_enemyCube->Initialize(graphics);
	_enemyCube->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	_moveSpeed = 2.0f;

	
	Fall(Vector3(_transform.position.x, _transform.position.y - 10, _transform.position.z));
	timer.Start();

}

void Enemy::Update(float dt)
{
	timer.Update();

	if (_state == IDLE)
	{
		if (timer.GetElapsedTime() >= 3)
		{
			timer.Stop();
			srand(time(NULL));

			int direction = rand() % 2;

			switch (direction)
			{
				case 0: Move(LEFT);
					break;
				case 1: Move(DOWN);
					break;
				default:
					break;
			}

			timer.Start();
		}
	}

	if (_state == FALLING)
	{
		_transform.position.y -= dt * _moveSpeed;
		if (_transform.position.y <= -10)
		{
			_state = DEAD;
		}else
		if (_transform.position.y <= _destY)
		{
			_transform.position.y = _destY;
			_state = IDLE;
		}

	}
	else
		if (_state == MOVING)
		{


		if (_movementDirection == UP)
		{
			_transform.position.y += dt * _moveSpeed;
			_transform.position.x -= dt * _moveSpeed;
			//_transform.position.y++;
			//_transform.position.x--;
			if (_transform.position.y >= _destY || _transform.position.x <= _destX)
			{
				_transform.position.y = _destY;
				_transform.position.x = _destX;
				_state = IDLE;
			}
		}
		if (_movementDirection == DOWN)
		{
			_transform.position.y -= dt * _moveSpeed;
			_transform.position.x += dt * _moveSpeed;
			//_transform.position.y--;
			//_transform.position.x++;
			if (_transform.position.y <= _destY || _transform.position.x >= _destX)
			{
				_transform.position.y = _destY;
				_transform.position.x = _destX;
				_state = IDLE;
			}
		}
		if (_movementDirection == RIGHT)
		{
			_transform.position.y += dt * _moveSpeed;
			_transform.position.z -= dt * _moveSpeed;
			//_transform.position.y++;
			//_transform.position.z--;
			if (_transform.position.y >= _destY || _transform.position.z <= _destZ)
			{
				_transform.position.y = _destY;
				_transform.position.z = _destZ;
				_state = IDLE;
			}
		}
		if (_movementDirection == LEFT)
		{
			_transform.position.y -= dt * _moveSpeed;
			_transform.position.z += dt * _moveSpeed;
			//_transform.position.y--;
			//_transform.position.z++;
			if (_transform.position.y <= _destY || _transform.position.z >= _destZ)
			{
				_transform.position.y = _destY;
				_transform.position.z = _destZ;
				_state = IDLE;
			}
		}

		}
	_enemyCube->GetTransform().position = _transform.position;

}
void Enemy::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{
	_enemyCube->Draw(graphics, relativeTo, dt);
}

void Enemy::Move(Direction d)
{
	if (_state == IDLE)
	{
		_state = MOVING;
		_movementDirection = d;
		if (d == UP)
		{
			_destX = _transform.position.x - 1;
			_destY = _transform.position.y + 1;
			//_transform.position.y++;
			//_transform.position.x--;
		}
		if (d == DOWN)
		{
			_destX = _transform.position.x + 1;
			_destY = _transform.position.y - 1;
			//_transform.position.y--;
			//_transform.position.x++;
		}
		if (d == RIGHT)
		{
			_destZ = _transform.position.z - 1;
			_destY = _transform.position.y + 1;
			//_transform.position.y++;
			//_transform.position.z--;
		}
		if (d == LEFT)
		{
			_destZ = _transform.position.z + 1;
			_destY = _transform.position.y - 1;
			//_transform.position.y--;
			//_transform.position.z++;
		}
	}

}


void Enemy::Fall(Vector3 destination)
{
	_state = FALLING;
	_destX = destination.x;
	_destY = destination.y;
	_destZ = destination.z;
}

EntityState& Enemy::GetState()
{
	return _state;
}

void Enemy::ResetPosition()
{
	srand(time(NULL));

	int position = rand() % 3;

	switch (position)
	{
	case 0: _transform.position = Vector3(0.0f, 15.0f, -4.0f);
		break;
	case 1:_transform.position = Vector3(0.0f, 14.0f, -3.0f);
		break;
	case 2:_transform.position = Vector3(1.0f, 14.0f, -4.0f);
		break;
	}

	Fall(Vector3(_transform.position.x, _transform.position.y - 10, _transform.position.z));
}