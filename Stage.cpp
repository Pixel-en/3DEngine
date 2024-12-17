#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"


void Stage::InitConstantBuffer()
{	
	//Quad�ƈꏏ
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTBUFFER_STAGE);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
	}
}

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage"),pConstantBuffer_(nullptr)
{
	lightpos.position_ = { Direct3D::GetGlovalLightVec().x,Direct3D::GetGlovalLightVec().y,Direct3D::GetGlovalLightVec().z };

}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hModel_[0] = Model::Load("Assets\\Torus_TP.fbx");
	hModel_[1] = Model::Load("Assets\\Torus_TL.fbx");
	hModel_[2] = Model::Load("Assets\\Torus_P.fbx");
	hModel_[3] = Model::Load("Assets\\Torus_L.fbx");

	hlightmodel = Model::Load("Assets\\Sphere.fbx");

	for (int i = 0; i < 4; i++) {
		trans[i] = transform_;
		trans[i].rotate_.x = 90;
		trans[i].position_.z += 2;
	}
	trans[0].position_.x -= 5;
	trans[1].position_.x -= 2;
	trans[2].position_.x += 2;
	trans[3].position_.x += 5;

	this->InitConstantBuffer();
}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;

	if (Input::IsKey(DIK_LEFT))
		lightpos.position_.x -= 0.1f;
	if (Input::IsKey(DIK_RIGHT))
		lightpos.position_.x += 0.1f;
	if (Input::IsKey(DIK_UP))
		lightpos.position_.z += 0.1f;
	if (Input::IsKey(DIK_DOWN))
		lightpos.position_.z -= 0.1f;
	if(Input::IsKey(DIK_W))
		lightpos.position_.y += 0.1f;
	if (Input::IsKey(DIK_S))
		lightpos.position_.y -= 0.1f;
	XMFLOAT4 temp = { lightpos.position_.x,lightpos.position_.y,lightpos.position_.z,Direct3D::GetGlovalLightVec().w };
	Direct3D::SetGlobalLightVec(temp);

	//�R���X�^���g�o�b�t�@�̐ݒ�ƁA�V�F�[�_�[�ւ̃R���X�^���g�o�b�t�@�̃Z�b�g������
	CONSTBUFFER_STAGE cb;
	cb.lightPosition = Direct3D::GetGlovalLightVec();
	XMStoreFloat4(&cb.eyePosition, Camera::GetPosition());

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//�ĊJ

	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

}

void Stage::Draw()
{
	for (int i = 0; i < 4; i++) {
		Model::SetTransform(hModel_[i], trans[i]);
		Model::Draw(hModel_[i]);
	}

	Model::SetTransform(hlightmodel, lightpos);
	Model::Draw(hlightmodel);
}

void Stage::Release()
{
}
