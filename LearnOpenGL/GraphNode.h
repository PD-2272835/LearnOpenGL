#pragma once
#include "Transform.h"
#define MAX_CHILDREN 3

class Node
{
public:
	Node()
	: _local(Transform::Origin()),
	  _dirty(true)
	{}

private:
	bool _dirty;
	Transform _local;
	Transform _world;
	//Mesh _mesh

	Node* _children[MAX_CHILDREN];
	int _childCount;
};

