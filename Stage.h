#pragma once
#include "Engine/GameObject.h"
#include <DirectXMath.h>

struct CONSTBUFFER_STAGE {
	XMFLOAT4 lightPosition;	//�����ʒu
	XMFLOAT4 eyePosition;	//���_�̈ʒu
};

class Stage:public GameObject
{
	int hModel_[3];
	int hlightmodel;
	Transform trans[4];
	Transform lightpos;
	int hFloor_;

	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	void InitConstantBuffer();

public:
	Stage(GameObject* parent);
	~Stage();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

