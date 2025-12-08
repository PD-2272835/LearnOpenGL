#pragma once
#include "GraphNode.h"

const float GravitationalConstant = 0.01f;
class CelestialBody : public Node
{
public:
	CelestialBody(Mesh* mesh = nullptr, Transform initTransform = { glm::vec3(0.f), glm::quat(1.f, 0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f) })
	{
		dirty_ = true;
		local_ = glm::mat4(1.f);
		mesh_ = mesh;
		children_ = new Node * [MAX_CHILDREN];
		maxChildren_ = MAX_CHILDREN;
		childCount_ = 0;
		transform_ = initTransform;
		parent_ = nullptr;
		Initialize(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f);
	}

    void Initialize(glm::vec3 initialVelocity, glm::vec3 startingPosition, float mass);

    void ProcessPhysics(float deltaTime) override;
    void UpdateVelocity(Node** allBodies, short bodyCount, float timeStep); 
    void UpdatePosition(float timeStep);

    glm::vec3 velocity;
    float mass;
};

