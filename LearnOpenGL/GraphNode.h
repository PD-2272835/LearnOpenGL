#pragma once
#include "Transform.h"
#include "Mesh.h"
#define MAX_CHILDREN 2

class Node
{
public:
	Node()
	: _local(),
	  _dirty(true)
	{}

private:
	bool _dirty;
	Transform _local;
	Transform* _world;
	Mesh* _mesh;

	Node* _children[MAX_CHILDREN];
	int _childCount;
};

