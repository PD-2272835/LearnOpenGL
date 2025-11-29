#include "GraphNode.h"



void Node::Destroy(bool destroyChildren = true)
{
	if (destroyChildren)
	{
		for (int i = 0; i < childCount_; i++)
		{
			std::cout << children_[i] << " i: " << i << "\n";
			children_[i]->Destroy(destroyChildren);
		}
	}
	delete[] children_;
	this->~Node();
}


void Node::Render(glm::mat4 parentWorld, bool dirty)
{
	dirty_ |= dirty;
	if (dirty)
	{
		local_ = CalculateLocalMatrix();
		world_ = parentWorld * local_;
	}

	if (mesh_) mesh_->Render(world_);

	for (int i = 0; i < childCount_; i++)
	{
		children_[i]->Render(world_, dirty);
	}
}


void Node::SetChild(Node* newChild)
{
	//resize the children array with an extra buffer element in case of multiple SetChild calls, 
	//hopefully reducing the number of times freestore will need to be managed in this way
	if (childCount_ + 1 >= maxChildren_)
	{
		ResizeChildArray(2);
	}

	//add the new child to children
	children_[childCount_] = newChild;
	childCount_++;
	std::cout << "wrote new child: " << newChild << "\n";
}

void Node::SetParent(Node* newParent)
{
	newParent->SetChild(this);
}


glm::mat4 Node::CalculateLocalMatrix()
{

	glm::mat4 Matrix(1.f);
	Matrix = glm::translate(Matrix, transform.position);
	Matrix *= glm::mat4_cast(transform.rotation);
	Matrix = glm::scale(Matrix, transform.scale);

	//return translate * rotate * scale;
	return Matrix;
}


void Node::ResizeChildArray(short extraSpace)
{
	Node** newArray = new Node* [childCount_ + extraSpace];
	
	std::copy(children_, children_ + childCount_, newArray); //there's something dubious here, I can feel it

	delete[] children_;

	maxChildren_ += extraSpace;
	children_ = new Node* [maxChildren_];
	
	for (int i=0; i < maxChildren_ - childCount_; i++)
	{
		children_[i] = nullptr;
	}

	std::copy(newArray, newArray + maxChildren_, children_);
	delete[] newArray;
}