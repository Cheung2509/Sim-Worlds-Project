#ifndef _BOID_MANAGER_H_
#define _BOID_MANAGER_H_

#include <vector>

#include "gameobject.h"
#include "Boid.h"

class BoidManager : public GameObject
{
public:
	//Constructer and Destructer
	BoidManager(string _filename, ID3D11Device * _pd3dDevice, IEffectFactory * _EF);
	~BoidManager();
	
	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	//Getters
	int getNumberOfBoids() { return m_boids.size(); };
	bool getIsEmpty() { return m_boids.empty(); };

	void addBoids();	//Function to push new boid into vector;
	void removeBoids();	//Function to pop a boid in vector

	void calculateAvoid();
	void calculateAlign();
	void calculateCohesion();

	void addOrRemoveCallback();

	int m_numberOfBoids;	//Number of boids
protected:
	std::vector<std::unique_ptr<Boid>> m_boids;	//Vector of boids

	std::shared_ptr<CMOGO> m_model;
};
#endif // !_BOID_MANAGER_H_
