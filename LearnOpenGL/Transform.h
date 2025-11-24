#pragma once
class Transform
{
public:
	static Transform Origin();

	Transform Combine(Transform& other);
};

