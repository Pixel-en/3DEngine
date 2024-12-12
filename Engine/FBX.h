#pragma once
#include <DirectXMath.h>
#include <fbxsdk.h>
#include "Direct3D.h"
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
		XMFLOAT4 specular;	//鏡面反射係数
		XMFLOAT4 shininess;	//鏡面反射のParameter
		XMFLOAT4 ambient;	//環境校の反射係数)
		XMFLOAT2 factor;
	};


	struct CONSTBUFFER_MODEL
	{
		XMMATRIX	matWVP;		//スクリーン変換マトリクス
		XMMATRIX    matW;		//ワールド変換マトリクス
		XMMATRIX	matNormal;	//法線のワールド変換用マトリクス
		XMFLOAT4	diffuseColor;	//RGBの拡散反射係数(色)
		//XMFLOAT4	globalLightvec;	//光源位置
		XMFLOAT2	diffuseFactor;	//拡散光の反射係数
		XMFLOAT4	ambientColor;
		XMFLOAT4    specularColor;
		XMFLOAT4	shininess;
		int			isTextured;	//テクスチャが貼ってあるかどうか
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

