#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

struct CONSTBUFFER_STAGE {
	XMFLOAT4 lightPosition;	//光源位置
	XMFLOAT4 eyePosition;	//視点の位置
};

class Stage:public GameObject
{
	int hModel_[4];
	int hlightmodel;
	Transform trans[4];
	Transform lightpos;

	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	void InitConstantBuffer();

public:
	Stage(GameObject* parent);
	~Stage();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

