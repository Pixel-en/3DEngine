#include "Quad.h"
#include "Camera.h"

Quad::Quad()
{
	vertexNum_ = 0;
	indexNum_ = 0;

	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;

	pTexture_ = nullptr;
}

Quad::~Quad()
{
	Release();
}

HRESULT Quad::Initialize()
{
#if 0
	// 頂点情報
	XMVECTOR vertices[] =
	{
		XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),	// 四角形の頂点（左上）
		XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	// 四角形の頂点（右上）
		XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	// 四角形の頂点（右下）y
		XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),	// 四角形の頂点（左下）		

		XMVectorSet(0.0f, 1.5f, 0.0f, 0.0f),	//五角形の頂点(中上)


		XMVectorSet(0.0f, -1.5f, 0.0f, 0.0f),	//六角形の頂点(中下)

		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
	};
#endif
#if 0


	XMVECTOR vertices[] = {
		XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),	// 四角形の頂点（左上）
		XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f),	// 四角形の頂点（右上）
		XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f),	// 四角形の頂点（右下)		
		XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),	// 四角形の頂点（左下）	
		XMVectorSet(0.0f,-2.0f,0.0f,0.0f),
	};
#endif

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

	if (FAILED(LoadTexture())) {
		return E_FAIL;

	}

	return S_OK;

	/*

	HRESULT hr;

	VERTEX vertices[] = {
		{XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f)},	// 四角形の頂点（左上）
		{XMVectorSet( 1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.25f, 0.0f, 0.0f, 0.0f)},	// 四角形の頂点（右上）
		{XMVectorSet( 1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f)},	// 四角形の頂点（右下)		
		{XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.25f, 0.5f, 0.0f, 0.0f)},	// 四角形の頂点（左下）	
	};


	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);


	if (FAILED(hr)) {
		MessageBox(nullptr, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	//インデックス情報
	int index[] = { 1,3,2, 1,0,3 };
	//int index[] = { 0,2,3, 0,1,2, 3,2,4 };
	//int index[] = { 0,2,3, 0,1,2, 0,4,1, 3,2,5};
	//int index[] = { 0,2,3, 0,1,2, 0,1,6, 6,1,2, 6,2,3, 0,6,3};
	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
		return hr;
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	pTexture_ = new Texture;
	pTexture_->Load("Assets\\dice.png");

	// コンスタントバッファの作成
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
		return hr;
	return S_OK;

	*/
}

void Quad::Draw(Transform& transform)
{

	Direct3D::SetShader(SHADER_2D);
	transform.Calculation();

	PassDataToCB(transform);

	SetBufferToPipeline();

	Direct3D::pContext->DrawIndexed(index_.size(), 0, 0);

	/*

	//コンスタントバッファに渡す情報
	D3D11_MAPPED_SUBRESOURCE pdata;
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(worldMatrix * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.matW = XMMatrixTranspose(worldMatrix); //MATRIXの掛け算のやり方がDirectXと違うので転置をとる（なんそれ）

	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用


	Direct3D::pContext->DrawIndexed(6, 0, 0);

	*/
}

void Quad::Release()
{


	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Quad::InitVertexData()
{
	// 頂点情報
	vertices_ =
	{
	 {XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},// 四角形の頂点（左上）
	 {XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f,  0.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},// 四角形の頂点（右上）
	 {XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)},// 四角形の頂点（右下）
	 {XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f)} // 四角形の頂点（左下） };
	};
	vertexNum_ = vertices_.size();
}

HRESULT Quad::CreateVertexBuffer()
{
	HRESULT hr;

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX)*vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);


	if (FAILED(hr)) {
		MessageBox(nullptr, L"頂点バッファの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	return S_OK;
}

void Quad::InitIndexData()
{
	index_ = { 0,2,3, 0,1,2 };
	indexNum_ = index_.size();
}

HRESULT Quad::CreateIndexBuffer()
{
	HRESULT hr;
	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * indexNum_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index_.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"インデックスバッファの作成に失敗", NULL, MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Quad::CreateConstantBuffer()
{
	HRESULT hr;
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr)) {
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", NULL, MB_OK);
		return hr;
	}
	return S_OK;
}

HRESULT Quad::LoadTexture()
{
	pTexture_ = new Texture;
	HRESULT hr;

	
	if (FAILED(hr = pTexture_->Load("Assets\\oldphoto.png"))) {
		MessageBox(NULL, L"テクスチャの作成に失敗", NULL, MB_OK);
		return hr;
	}
	return S_OK;

}

void Quad::PassDataToCB(Transform& transform)
{
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix()); //view*projをカメラからとってくる
	cb.matW = XMMatrixTranspose(transform.GetNormalMatrix()); //MATRIXの掛け算のやり方がDirectXと違うので転置をとる（なんそれ）

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); // GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); // データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0); //再開
}

void Quad::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_); //頂点シェーダー用 
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_); //ピクセルシェーダー用


	//サンプラーとシェーダーリソースビューをシェーダにセット
	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	//Direct3D::pContext->DrawIndexed(indexNum_, 0, 0);
}
