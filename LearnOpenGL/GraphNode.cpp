#include "GraphNode.h"

Node::~Node()
{
	for (int i = 0; i < _childCount; i++)
	{
		_children[0][i].Delete();
	}
	delete[] _children;
}

void Node::Delete()
{
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
		_children[0][i].Render(_world, dirty);
	}
}

void Node::SetChild(Node &newChild)
{
	//resize the children array with an extra buffer element in case of multiple SetChild calls, 
	//hopefully reducing the number of times freestore will need to be managed in this way
	if (_childCount + 1 >= _maxChildren)
	{
		Node** newArray = new Node*[_childCount + 2];
		std::copy(_children[0][0], _children[0][_childCount], newArray[0][0]); //there's something dubious here, I can feel it

		std::copy(_children, _children + _childCount, newArray);

		//there's something dubious here *too*, I can feel it
		delete[] _children[0];
		delete[] _children;

		_children = newArray;
	}
	//add the new child to children
	_childCount++;
	_children[0][_childCount] = newChild;
}

void Node::SetParent(Node &newParent)
{
	newParent.SetChild(*this);
}