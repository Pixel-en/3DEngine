#pragma once
#include <DirectXMath.h>
#include "Direct3D.h"
#include "Texture.h"
#include <vector>
#include "Transform.h"

using std::vector;
using namespace DirectX;

struct CONSTANT_BUFFER2D
{
	XMMATRIX	matW;

	XMINT2 winSize;
};

//頂点情報
struct VERTEX2D
{
	XMVECTOR position;
	XMVECTOR uv;
};

class Sprite
{
private:
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;		//テクスチャへのポインタ

	uint64_t vertexNum_;		//頂点数
	vector<VERTEX2D> vertices_;	//頂点情報
	uint64_t indexNum_;			//インデックス数
	vector<int> indices_;		//インデックス情報

public:

	Sprite();
	~Sprite();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();

private:

	//--------Initialize関数から呼ばれる関数--------

	void InitVertexData();			//頂点情報の準備
	HRESULT CreateVertexBuffer();	//頂点バッファを作成

	void InitIndexData();			//インデックス情報を準備
	HRESULT CreateIndexBuffer();	//インデックスバッファを作成

	HRESULT CreateConstantBuffer();	//コンスタントバッファを作成

	HRESULT LoadTexture(std::string fineName);			//テクスチャをロード


	//--------Draw関数から呼ばれる関数--------

	void PassDataToCB(DirectX::XMMATRIX worldMatrix);	//コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline();							//各バッファをパイプラインにセット

};