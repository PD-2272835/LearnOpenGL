#pragma once
#include "Mesh.h"
#define MAX_CHILDREN 2

class Node
{
public:
	Node(Mesh* mesh = nullptr) :
		_dirty(true)
	{
		_local = glm::mat4(1.f);
		_mesh = mesh;
		_children = new Node*[MAX_CHILDREN];
		_maxChildren = MAX_CHILDREN;
		_childCount = 0;
	}

	~Node();

	void Render(glm::mat4 parentWorld, bool dirty);

	//Methods for managing nodes in a graph
	void SetChild(Node &newChild);
	void SetParent(Node &newParent);

	void Delete();


private:
	bool _dirty;
	glm::mat4 _local;
	glm::mat4 _world;
	Mesh* _mesh;

	Node** _children;
	short _childCount; //current number of children
	short _maxChildren; //maximum number of children permitted by the size of the array
};

