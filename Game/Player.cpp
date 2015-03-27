
#include "Player.h"

Player::Player()
{
	_transform.position = Vector3(0.0f,0.0f,0.0f);
}
Player::Player(Vector3 position)
{
	_transform.position = position;
}


void Player::Initialize(Graphics *graphics)
{
	_playerCube = new Cube(_transform.position);
	_playerCube->GetTransform().scale = Vector3(0.6f, 0.6f, 0.6f);
	_playerCube->Initialize(graphics);
	_playerCube->SetColor(0.0f,0.0f,1.0f,1.0f);
	_state = IDLE;
	_moveSpeed = 2.0f;

}

void Player::Update(float dt)
{
	
	if (_state == FALLING)
	{
		_transform.position.y -= dt * _moveSpeed;
		if (_transform.position.y <= -10)
		{
			_state = DEAD;
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
	_playerCube->GetTransform().position = _transform.position;

}

void Player::Draw(Graphics *graphics, Matrix4x4 relativeTo, float dt)
{
	_playerCube->Draw(graphics, relativeTo, dt);
}

void Player::Move(Direction d)
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

void Player::Fall(Vector3 destination)
{
	_state = FALLING;
	_destX = destination.x;
	_destY = destination.y;
	_destZ = destination.z;
}

EntityState& Player::GetState()
{
	return _state;
}