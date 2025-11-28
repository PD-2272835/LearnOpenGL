#include "GraphNode.h"

Node::~Node()
{
	delete[] _children; //vaguely less dubious?
}

void Node::Destroy(bool destroyChildren)
{
	if (destroyChildren)
	{
		for (int i = 0; i < _childCount; i++)
		{
			_children[i]->Destroy(destroyChildren);
		}
	}
	delete this;
}


void Node::Render(glm::mat4 parentWorld, bool dirty)
{

	dirty |= _dirty;
	if (dirty)
	{
		_world = parentWorld * _local;
	}

	if (_mesh) _mesh->Render(_world);

	for (int i = 0; i < _childCount; i++)
	{
		_children[i]->Render(_world, dirty);
	}
}


void Node::SetChild(Node* newChild)
{
	//resize the children array with an extra buffer element in case of multiple SetChild calls, 
	//hopefully reducing the number of times freestore will need to be managed in this way
	if (_childCount + 1 >= _maxChildren)
	{
		ResizeChildArray(2);
		_maxChildren + 2;
	}
	//add the new child to children
	_childCount++;
	_children[_childCount] = newChild;
	std::cout << "wrote new child\n";
}

void Node::SetParent(Node* newParent)
{
	newParent->SetChild(this);
}



void Node::ResizeChildArray(short extraSpace)
{
	Node** newArray = new Node* [_childCount + extraSpace];
	
	std::cout << "boutta copy\n";
	std::copy(_children, _children + _childCount, newArray); //there's something dubious here, I can feel it
	std::cout << "copied\n";
	//mayhaps a dubious?
	delete[] _children;
	std::cout << "deleted old array\n";
	_children = newArray;
	std::cout << "wrote new array to current\n";
	_maxChildren += extraSpace;
}