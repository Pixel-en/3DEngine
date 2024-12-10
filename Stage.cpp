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
	trans[3] = transform_;
	trans[3].position_.y -= 1;
	lightpos.position_ = { Direct3D::GetGlovalLightVec().x,Direct3D::GetGlovalLightVec().y,Direct3D::GetGlovalLightVec().z };
	hFloor_ = 0;
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	hlightmodel = Model::Load("Assets\\Sphere.fbx");
	hFloor_ = Model::Load("Assets\\Torus.fbx");

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

	Model::SetTransform(hFloor_, trans[3]);
	Model::Draw(hFloor_);
	Model::SetTransform(hlightmodel, lightpos);
	Model::Draw(hlightmodel);
}

void Stage::Release()
{
}
