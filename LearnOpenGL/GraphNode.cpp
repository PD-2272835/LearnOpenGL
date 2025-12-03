#include "GraphNode.h"



void Node::Render(glm::mat4 parentWorld, bool dirty)
{
	dirty_ |= dirty;
	if (dirty)
	{
		local_ = CalculateLocalMatrix();
		world_ = parentWorld * local_;
		dirty_ = false;
	}

	if (mesh_) mesh_->Render(world_);

	for (int i = 0; i < childCount_; i++)
	{
		children_[i]->Render(world_, dirty);
	}
}


void Node::Parent(Node* newParent)
{
	SetParent(newParent);
	newParent->SetChild(this);
}

void Node::RemoveChild(Node* targetChild)
{
	Node** newArray = new Node * [maxChildren_];
	for (int i = 0; i < maxChildren_; i++)
	{
		if (children_[i] = targetChild)
		{
			childCount_--;
			continue;
		}
		newArray[i] = children_[i];
	}
	delete[] children_;
	children_ = newArray;
	delete[] newArray;
}

void Node::AddChild(Node* newChild)
{
	newChild->SetParent(this);
	SetChild(newChild);
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
	std::cout << "wrote new child: " << newChild << " to child array\n";
}

void Node::SetParent(Node* newParent)
{
	if (parent_) //ptr evaluates to boolean (nullptr is false)
	{
		parent_->RemoveChild(this);
	}
	parent_ = newParent;
}


Node** Node::GetChildren()
{
	return children_;
}

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

//not very good unoptimized traversal implementation of PRAPI stuff
//REFACTOR THIS
void Node::ProcessPhysics()
{
	for (int i = 0; i < childCount_; i++)
	{
		children_[i]->ProcessPhysics();
	}
}


//-----------------------------------Transform modifiers-----------------------------------
void Node::SetPosition(glm::vec3 newPosition)
{
	transform_.position = newPosition;
	dirty_ = true;
}

void Node::MovePosition(glm::vec3 translation)
{
	transform_.position += translation;
	dirty_ = true;
}

void Node::SetRotation(glm::quat newOrientation)
{
	transform_.rotation = newOrientation;
	dirty_ = true;
}

void Node::Rotate(float angle, glm::vec3 axis)
{
	transform_.rotation = glm::rotate(transform_.rotation, glm::radians(angle), axis);
	dirty_ = true;
}

void Node::SetScale(glm::vec3 newScale)
{
	transform_.scale = newScale;
	dirty_ = true;
}

void Node::Scale(glm::vec3 scaleModifier)
{
	transform_.scale += scaleModifier;
	dirty_ = true;
}



glm::mat4 Node::CalculateLocalMatrix()
{

	glm::mat4 Matrix(1.f);
	Matrix = glm::translate(Matrix, transform_.position);
	Matrix *= glm::mat4_cast(transform_.rotation);
	Matrix = glm::scale(Matrix, transform_.scale);

	//return translate * rotate * scale;
	std::cout << "calculated local matrix for " << this << "\n";
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