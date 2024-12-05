#pragma once
#include "Engine/GameObject.h"

class Stage:public GameObject
{
	int hModel_[3];
	int hlightmodel;
	Transform trans[4];
	Transform lightpos;
	int hFloor_;

public:
	Stage(GameObject* parent);
	~Stage();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

