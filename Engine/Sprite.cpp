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

	//頂点情報
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

	//コンスタントバッファに渡す情報
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
	{XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// 四角形の頂点（左上）
	{XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)},	// 四角形の頂点（右上）
	{XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f)},	// 四角形の頂点（右下)		
	{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)},	// 四角形の頂点（左下）	
	};

	vertexNum_ = vertices_.size();
}

HRESULT Sprite::CreateVertexBuffer()
{
	HRESULT hr;

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX2D) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();	//配列のアドレスを入れる
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr)) {
		MessageBox(nullptr, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	return S_OK;

}

void Sprite::InitIndexData()
{
	indices_ = { 0,2,3, 0,1,2 };

	//インデックス数
	indexNum_ = indices_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_; //自分のクラスに合わせる
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices_.data(); //自分のクラスに合わせる
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"インデックスバッファの作成に失敗しました", L"エラー", MB_OK);
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
		MessageBox(NULL, L"コンスタントバッファの作成に失敗",L"エラー", MB_OK);
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
		MessageBox(NULL, L"テクスチャの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void Sprite::PassDataToCB(DirectX::XMMATRIX worldMatrix)
{
	CONSTANT_BUFFER2D cb;
	cb.matW = XMMatrixTranspose(worldMatrix); //MATRIXの掛け算のやり方がDirectXと違うので転置をとる（なんそれ）
	cb.winSize = Direct3D::WindowSize;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開


}

void Sprite::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX2D);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);
	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	//サンプラーとシェーダーリソースビューをシェーダにセット
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);
}
