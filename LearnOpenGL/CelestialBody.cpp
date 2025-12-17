#include "CelestialBody.h"



void CelestialBody::Initialize(glm::vec3 initialVelocity, glm::vec3 startingPosition, float initialMass)
{
	velocity = initialVelocity;
	SetPosition(startingPosition);
	mass = initialMass;
}

void CelestialBody::ProcessPhysics(float deltaTime)
{
	UpdateVelocity(parent_->GetChildren(), parent_->GetChildCount(), deltaTime); //too coupled - but works for this application
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
			glm::vec3 vectorDifference(allBodies[i]->GetPosition() - transform_.position); //difference in position
			float sqrDistance = glm::dot(vectorDifference, vectorDifference); //r^^2
			glm::vec3 forceDirection(glm::normalize(vectorDifference)); //direction to move the force in
			glm::vec3 acceleration(forceDirection * GravitationalConstant * (mass / sqrDistance)); //calculate acceleration from G*(mass/r^^2)
			velocity += acceleration * timeStep; //decouple the acceleration from the framerate
		}
	}
}

void CelestialBody::UpdatePosition(float timeStep)
{
	MovePosition(velocity * timeStep); //update the position of this object by the current velocity
}