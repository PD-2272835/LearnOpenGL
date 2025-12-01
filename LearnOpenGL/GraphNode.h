#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Mesh.h"
#define MAX_CHILDREN 2

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	
};

class Node
{
public:
	Node(Mesh* mesh = nullptr, Transform initTransform = { glm::vec3(0.f), glm::quat(1.f, 0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)}) :
		dirty_(true)
	{
		local_ = glm::mat4(1.f);
		mesh_ = mesh;
		children_ = new Node*[MAX_CHILDREN];
		maxChildren_ = MAX_CHILDREN;
		childCount_ = 0; //terrible hack fix for my dogshit implementation of SetChild() as it starts by incrementing this value
		transform_ = initTransform;
	}

	//publicly accessible transformation modification methods
	//so that we can flag this object's transformation as dirty when it's modified
	void SetPosition(glm::vec3 newPosition); //Set this object's Position (vector3)
	void MovePosition(glm::vec3 translation); //Translate this object's current position by vector3(translation)

	void SetRotation(glm::quat newRotation); //Set this object's Orientation (quaternion)
	void Rotate(float angle, glm::vec3 axis); //Rotate this object by angle(degrees) around vector3(axis)

	void SetScale(glm::vec3 newScale); //Set the scale of this object (vector3)
	void Scale(glm::vec3 scaleModifier); //Scale this object's size by vector3(scaleModifier)


	void Render(glm::mat4 parentWorld, bool dirty); //unoptimized graph traversal for rendering

	//Methods for managing nodes in a graph
	void SetChild(Node* newChild);
	void SetParent(Node* newParent);
	void Destroy(bool destroyChildren);

private:

	Transform transform_;
	glm::mat4 CalculateLocalMatrix();

	void ResizeChildArray(short extraSpace);

	bool dirty_;
	glm::mat4 local_;
	glm::mat4 world_;
	Mesh* mesh_;

	Node** children_; //no way of accessing parent of this node
	short childCount_; //tracks the end of actually used pointers in the child array
	short maxChildren_; //size of the children array in elements

};

