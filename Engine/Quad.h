#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

using namespace DirectX;

struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matW;
};

//頂点情報
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;
};

class Quad
{
protected:
	int vertexNum_;
	std::vector<VERTEX> vertices_;
	int indexNum_;
	std::vector<int> index_;


	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;

public:

	Quad();
	virtual ~Quad();
	HRESULT Initialize();
	void Draw(Transform& transform);
	void Release();

private:
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();
	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture();

	//--------Draw関数から呼ばれる関数---------
	void PassDataToCB(Transform& transform);
	void SetBufferToPipeline();
};