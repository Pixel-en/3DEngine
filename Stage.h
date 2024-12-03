#pragma once
#include "Engine/GameObject.h"

class Stage:public GameObject
{
	int hModel_[3];
	Transform trans[4];
	XMFLOAT4 lightpos;
	int hFloor_;

public:
	Stage(GameObject* parent);
	~Stage();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

