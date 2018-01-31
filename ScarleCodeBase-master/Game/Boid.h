#ifndef  _BOID_H_
#define _BOID_H_

#include "CMOGO.h"

#include <vector>

class Boid : public GameObject
{
public:
	//Constructor and Destructor;
	Boid(std::shared_ptr<CMOGO> model);
	~Boid();

	//Tick and Draw functions
	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData* _DD) override;

	//Calculations for Boids Rules
	void calculateAlign(std::vector<std::unique_ptr<Boid>>& boids);
	void calculateCohesion(std::vector<std::unique_ptr<Boid>>& boids);
	void calculateSeperation(std::vector<std::unique_ptr<Boid>>& boids);

	//Getters
	Vector3 getAlign() { return m_align; };
	Vector3 getCohesion() { return m_cohesion; };
	Vector3 getAvoid() { return m_avoid; };

	void boundaries();
protected:
	//Vector for rules
	Vector3 m_align;
	Vector3 m_cohesion;
	Vector3 m_avoid;

	int m_boundaryX;
	int m_boundaryY;
	int m_boundaryZ;

	//Distance for rules to apply
	static float m_cohDistance;
	static float m_avoidDistance;
	static float m_alignDistance;

	static float m_avoidWeight;
	static float m_cohWeight;
	static float m_alignWeight;

	static float m_maxSpeed;
	static float m_maxForce;

	std::shared_ptr<CMOGO> m_model = nullptr;
};
#endif // ! _BOID_H_
