#include "Sprite.h"
#include "Camera.h"
#include <filesystem>


Sprite::Sprite()
{
	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;

	pTexture_ = nullptr;

	vertexNum_ = 0;
	indexNum_ = 0;

}

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Load(std::string fileName)
{

	HRESULT hr;

	//���_���
	InitVertexData();
	if (FAILED(CreateVertexBuffer())) {
		return E_FAIL;
	}

	InitIndexData();
	if (FAILED(CreateIndexBuffer())) {
		return E_FAIL;
	}

	if (FAILED(CreateConstantBuffer())) {
		return E_FAIL;
	}

	if (FAILED(LoadTexture(fileName))) {
		return E_FAIL;
	}

	return S_OK;
}

void Sprite::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_TYPE::SHADER_2D);

	transform.Calculation();

	//�R���X�^���g�o�b�t�@�ɓn�����
	PassDataToCB(transform.GetWorldMatrix());

	SetBufferToPipeline();


	Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
}

void Sprite::Release()
{
	SAFE_DELETE(pTexture_);

	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::InitVertexData()
{
	vertices_ = {
	{XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// �l�p�`�̒��_�i����j
	{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)},	// �l�p�`�̒��_�i�E��j
	{XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f)},	// �l�p�`�̒��_�i�E��)		
	{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)},	// �l�p�`�̒��_�i�����j	
	};

	vertexNum_ = vertices_.size();
}

HRESULT Sprite::CreateVertexBuffer()
{
	HRESULT hr;

	// ���_�f�[�^�p�o�b�t�@�̐ݒ�
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX2D) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();	//�z��̃A�h���X������
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"���_�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}

	return S_OK;

}

void Sprite::InitIndexData()
{
	indices_ = { 0,2,3, 0,1,2 };

	//�C���f�b�N�X��
	indexNum_ = indices_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_; //�����̃N���X�ɍ��킹��
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices_.data(); //�����̃N���X�ɍ��킹��
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�C���f�b�N�X�o�b�t�@�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Sprite::CreateConstantBuffer()
{

	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER2D);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;


	HRESULT hr;


	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		MessageBox(NULL, L"�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s",L"�G���[", MB_OK);
		return hr;
	}
}

HRESULT Sprite::LoadTexture(std::string fileName)
{
	namespace fs = std::filesystem;

	pTexture_ = new Texture;
	assert(fs::is_regular_file(fileName));


	HRESULT hr;
	hr = pTexture_->Load(fileName);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�e�N�X�`���̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}
	return S_OK;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX worldMatrix)
{
	CONSTANT_BUFFER2D cb;
	cb.matW = XMMatrixTranspose(worldMatrix); //MATRIX�̊|���Z�̂�����DirectX�ƈႤ�̂œ]�u���Ƃ�i�Ȃ񂻂�j
	cb.winSize = Direct3D::WindowSize;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPU����̃f�[�^�A�N�Z�X���~�߂�
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// �f�[�^��l�𑗂�

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//�ĊJ


}

void Sprite::SetBufferToPipeline()
{
	//���_�o�b�t�@
	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//�R���X�^���g�o�b�t�@
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//���_�V�F�[�_�[�p	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//�s�N�Z���V�F�[�_�[�p

	//�T���v���[�ƃV�F�[�_�[���\�[�X�r���[���V�F�[�_�ɃZ�b�g
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
}