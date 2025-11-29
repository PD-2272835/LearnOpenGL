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
		transform = initTransform;
	}

	//publicly accessible transformation used to recalculate model matrix in local space
	Transform transform;

	void Render(glm::mat4 parentWorld, bool dirty);

	//Methods for managing nodes in a graph
	void SetChild(Node* newChild);
	void SetParent(Node* newParent);
	void Destroy(bool destroyChildren);

private:
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

