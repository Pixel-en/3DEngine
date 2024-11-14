#include "Transform.h"

Transform::Transform()
    :position_({0,0,0}),
    rotate_({0,0,0}),
    scale_({ 1, 1, 1 }),
    matTranslate_(XMMatrixIdentity()),
    matRotate_(XMMatrixIdentity()),
    matScale_(XMMatrixIdentity())
{
    //float x = XMVectorGetX(position_);
    //float y = XMVectorGetY(position_);
    //float z = XMVectorGetZ(position_);
    //float w = XMVectorGetW(position_);
}

Transform::~Transform()
{
}

void Transform::Calculation()
{
    //SRTの順番で掛けるとワールド変換になる

    //移動行列の計算
    matTranslate_ = XMMatrixTranslation(position_.x, position_.y, position_.z);

    //回転行列の計算   Z*X*Yの順番固定
    //digree->radians
    XMMATRIX rx = XMMatrixRotationX(rotate_.x);
    XMMATRIX ry = XMMatrixRotationY(rotate_.y);
    XMMATRIX rz = XMMatrixRotationZ(rotate_.z);
    matRotate_ = rz * rx * ry;

    //拡大行列の計算
    matScale_ = XMMatrixScaling(scale_.x,scale_.y,scale_.z);


}

XMMATRIX Transform::GetWorldMatrix()
{
    //return matScale_ * matRotate_ * matTranslate_;
    if (pParent_ != nullptr) {
        return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }
    else
    {
        return matScale_ * matRotate_ * matTranslate_;
    }
}

XMMATRIX Transform::GetNormalMatrix()
{
    //平行移動は無視　回転はかける　スケールは元に戻す
    return (matRotate_ * XMMatrixInverse(nullptr, matScale_));
}
