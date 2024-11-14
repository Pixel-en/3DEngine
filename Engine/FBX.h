#pragma once
#include <DirectXMath.h>
#include <fbxsdk.h>
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"
#include "filesystem"

#pragma comment(lib, "LibFbxSDK-MD.lib")
//#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")


class FBX
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};


	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;		//ワールドビュー
		XMMATRIX	matNormal;	//法線ベクトル
		XMFLOAT4	diffuseColor;
		int		isTextured;	//テクスチャが貼ってあるかどうか
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer** pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	std::vector<MATERIAL> pMaterialList_;	

	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数

	void InitVertex(fbxsdk::FbxMesh* mesh);
	void InitIndex(fbxsdk::FbxMesh* mesh);
	void InitConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);

	std::vector<int> indexcount;

public:

	FBX();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();

};

