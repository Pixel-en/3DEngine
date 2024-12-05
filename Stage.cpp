#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent,"Stage")
{
	trans[0] = transform_;
	trans[1] = transform_;
	trans[2] = transform_;
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
	hModel_[0] = Model::Load("Assets\\Sphere_0.3.fbx");
	hModel_[1] = Model::Load("Assets\\Sphere.fbx");
	hModel_[2] = Model::Load("Assets\\Sphere_1.0.fbx");
	hlightmodel = Model::Load("Assets\\Sphere.fbx");
	hFloor_ =	 Model::Load("Assets\\floor.fbx");
	trans[0].position_.x -= 3;
	trans[2].position_.x += 3;

}

void Stage::Update()
{
	transform_.rotate_.y += 0.5f;
	for (int i = 0; i < 3; i++) {
		trans[i].rotate_.y = transform_.rotate_.y;
	}
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
}

void Stage::Draw()
{
	for (int i = 0; i < 3; i++) {
		Model::SetTransform(hModel_[i], trans[i]);
		Model::Draw(hModel_[i]);
	}

	Model::SetTransform(hFloor_, trans[3]);
	Model::Draw(hFloor_);
	Model::SetTransform(hlightmodel, lightpos);
	Model::Draw(hlightmodel);
}

void Stage::Release()
{
}
