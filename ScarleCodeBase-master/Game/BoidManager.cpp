#include "BoidManager.h"
#include "GameData.h"
#include "DrawData.h"

#include <thread>

BoidManager::BoidManager(string _filename, ID3D11Device * _pd3dDevice, IEffectFactory * _EF)
{
	m_numberOfBoids = 500;

	m_model = std::make_shared<CMOGO>(_filename, _pd3dDevice, _EF);

	for (int i = 0; i <= m_numberOfBoids; i++)
	{
		addBoids();
	}
}

BoidManager::~BoidManager()
{
	m_boids.clear();
}

void BoidManager::Tick(GameData* _GD)
{
	if (!m_boids.empty())
	{
		std::thread alignThread(&BoidManager::calculateAlign, this);
		std::thread cohesionThread(&BoidManager::calculateCohesion, this);
		std::thread avoidThread(&BoidManager::calculateAvoid, this);


		alignThread.join();
		cohesionThread.join();
		avoidThread.join();

		for (auto& iter : m_boids)
		{
			iter->Tick(_GD);
		}
	}

	addOrRemoveCallback();
}

void BoidManager::Draw(DrawData* _DD)
{
	//If there are boids in the vector
	if (!m_boids.empty())
	{
		//Cycle through all the boids draw function
		for (auto& iter : m_boids)
		{
			iter->Draw(_DD);
		}
	}
}

void BoidManager::addBoids()
{
	std::unique_ptr<Boid> boid = std::make_unique<Boid>(m_model);
	boid->SetScale(0.1);
	m_boids.push_back(std::move(boid));
}

void BoidManager::removeBoids()
{
	if (!m_boids.empty())
	{
		m_boids.pop_back();
	}
}

void BoidManager::calculateAvoid()
{
	for (auto& iter : m_boids)
	{
		iter->calculateSeperation(m_boids);
	}
}

void BoidManager::calculateAlign()
{
	for (auto& iter : m_boids)
	{
		iter->calculateAlign(m_boids);
	}
}

void BoidManager::calculateCohesion()
{
	for (auto& iter : m_boids)
	{
		iter->calculateCohesion(m_boids);
	}
}

void BoidManager::addOrRemoveCallback()
{
	while (m_numberOfBoids != m_boids.size())
	{
		if (m_boids.size() > m_numberOfBoids)
		{
			m_boids.pop_back();
		}
		else if (m_boids.size() < m_numberOfBoids)
		{
			addBoids();
		}
	}
}


