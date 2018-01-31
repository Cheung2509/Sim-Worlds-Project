#include "Boid.h"
#include "GameData.h"
#include "DrawData.h"

#include <random>

float Boid::m_alignDistance = 60;
float Boid::m_avoidDistance = 10;
float Boid::m_cohDistance = 60;

float Boid::m_avoidWeight = 1;
float Boid::m_cohWeight = 1;
float Boid::m_alignWeight = 1;

float Boid::m_maxSpeed = 5;
float Boid::m_maxForce = 2;

Boid::Boid(std::shared_ptr<CMOGO> model)
{
	m_model = model;

	//Random number distribution stuff
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<> randomNumber( -100, 100);

	int new_x = randomNumber(gen);
	int new_y = randomNumber(gen);
	int new_z = randomNumber(gen);

	m_pos = Vector3(new_x, new_y, new_z);

	SetPhysicsOn(true);

	m_boundaryX = 200;
	m_boundaryY = 200;
	m_boundaryZ = 200;
}

Boid::~Boid()
{

}

void Boid::Tick(GameData * _GD)
{
	//Set the boids to poing to direction needed
	Vector3 direction = Vector3::Zero;
	direction.Normalize();
	direction = Vector3::Transform(direction, m_worldMat) - m_pos;
	Matrix scaleMat = Matrix::CreateScale(m_scale);
	Matrix rotTransMat = Matrix::CreateWorld(m_pos, direction, Vector3::Up);

	//Add force to boid
	m_acc += m_align + m_cohesion + m_avoid;

	//Calling function to check whether the boid is in the boindary
	boundaries();

	GameObject::Tick(_GD);

	m_worldMat = m_fudge * scaleMat * rotTransMat;
}

void Boid::Draw(DrawData * _DD)
{
	m_model->SetPos(this->GetPos());
	m_model->SetWorld(this->m_worldMat);

	m_model->Draw(_DD);
}

void Boid::calculateAlign(std::vector<std::unique_ptr<Boid>>& boids)
{
	Vector3 _align = Vector3::Zero;
	int count = 0;

	for (auto& n : boids)
	{
		//If neighbors is not the boid
		if (n.get() != this)
		{
			float distance = Vector3::DistanceSquared(m_pos, n->GetPos());

			if (distance < (m_alignDistance * m_alignDistance))
			{
				_align += n->GetVel();
				++count;
			}
		}
	}

	if (count > 0)
	{
		_align /= count;

		_align.Normalize();
		_align *= m_maxSpeed;

		Vector3 steer = _align - m_vel;
		steer.Clamp(Vector3(-m_maxForce, -m_maxForce, -m_maxForce), Vector3(m_maxForce, m_maxForce, m_maxForce));

		m_align = steer;
	}
}

void Boid::calculateCohesion(std::vector<std::unique_ptr<Boid>>& boids)
{
	int count = 0;
	Vector3 _coh = Vector3::Zero;

	for (auto& n : boids)
	{
		if (n.get() != this)
		{
			float distance = Vector3::DistanceSquared(m_pos, n->GetPos());

			if (distance < (m_cohDistance * m_cohDistance))
			{
				_coh += n->GetPos();
				count++;
			}
		}
	}

	if (count > 0)
	{
		_coh /= count;

		Vector3 desired = _coh - m_pos;
		desired.Normalize();
		desired *= m_maxSpeed;

		desired.Clamp(Vector3(-m_maxForce, -m_maxForce, -m_maxForce), Vector3(m_maxForce, m_maxForce, m_maxForce));

		desired * m_cohWeight;

		m_cohesion = desired;
	}
}

void Boid::calculateSeperation(std::vector<std::unique_ptr<Boid>>& boids)
{
	Vector3 _avoid = Vector3::Zero;
	int count = 0;

	for (auto& n : boids)
	{
		//If neighbors is not the boid
		if (n.get() != this)
		{
			float distance = Vector3::DistanceSquared(m_pos, n->GetPos());
			//Check distance of boid
			if (distance < (m_avoidDistance * m_avoidDistance))
			{
				//calculating difference difference between neighbor
				Vector3  diff = m_pos - n->GetPos();
				diff.Normalize();
				diff /= distance;

				//Adding difference
				_avoid += diff;
				count++;
			}
		}
	}

	if (count > 0)
	{
		_avoid /= (float)count;
		
	//calculate the magnitude of the vector
	float magnitude = sqrt((_avoid.x* _avoid.x) + (_avoid.y * _avoid.y) + (_avoid.z * _avoid.z));

	if (magnitude > 0)
	{
		_avoid.Normalize();
		_avoid *= m_maxSpeed;
	}

	_avoid *= m_avoidWeight;

	m_avoid = _avoid;
	}
}

void Boid::boundaries()
{
	if (m_pos.x > m_boundaryX)
	{
		m_pos.x = m_boundaryX * -1;
	}
	else if (m_pos.x < m_boundaryX * -1)
	{
		m_pos.x = m_boundaryX;
	}

	if (m_pos.z > m_boundaryZ)
	{
		m_pos.z = m_boundaryZ * -1;
	}
	
	if (m_pos.z < m_boundaryZ * -1)
	{
		m_pos.z = m_boundaryZ;
	}

	if (m_pos.y > m_boundaryY)
	{
		m_pos.y = m_boundaryY * -1;
	}

	if (m_pos.y < m_boundaryY * -1)
	{
		m_pos.y = m_boundaryY;
	}
}
