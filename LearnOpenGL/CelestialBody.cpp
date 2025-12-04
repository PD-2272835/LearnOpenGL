#include "CelestialBody.h"



void CelestialBody::Initialize(glm::vec3 initialVelocity, glm::vec3 startingPosition, float initialMass)
{
	velocity = initialVelocity;
	SetPosition(startingPosition);
	mass = initialMass;
}

void CelestialBody::ProcessPhysics(float deltaTime)
{
	UpdateVelocity(parent_->GetChildren(), parent_->GetChildCount(), deltaTime);
	UpdatePosition(deltaTime);

	for (int i = 0; i < childCount_; i++)
	{
		children_[i]->ProcessPhysics(deltaTime);
	}
}

void CelestialBody::UpdateVelocity(Node** allBodies, short bodyCount, float timeStep)
{
	for (int i = 0; i < bodyCount; i++)
	{
		if (allBodies[i] != this)
		{
			glm::vec3 vectorDifference(allBodies[i]->GetPosition() - transform_.position);
			float sqrDistance = glm::dot(vectorDifference, vectorDifference);
			glm::vec3 forceDirection(glm::normalize(vectorDifference));

			glm::vec3 acceleration(forceDirection * GravitationalConstant * (mass / sqrDistance));
			velocity += acceleration * timeStep;
			//std::cout << this << velocity.x << velocity.y << velocity.z << 
		}
	}
}

void CelestialBody::UpdatePosition(float timeStep)
{
	//std::cout << velocity.x << velocity.y << velocity.z << "\n";
	MovePosition(velocity * timeStep);
}