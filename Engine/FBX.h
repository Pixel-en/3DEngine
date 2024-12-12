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
	//�}�e���A��
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;	//���ʔ��ˌW��
		XMFLOAT4 shininess;	//���ʔ��˂�Parameter
		XMFLOAT4 ambient;	//���Z�̔��ˌW��)
		XMFLOAT2 factor;
	};


	struct CONSTBUFFER_MODEL
	{
		XMMATRIX	matWVP;		//�X�N���[���ϊ��}�g���N�X
		XMMATRIX    matW;		//���[���h�ϊ��}�g���N�X
		XMMATRIX	matNormal;	//�@���̃��[���h�ϊ��p�}�g���N�X
		XMFLOAT4	diffuseColor;	//RGB�̊g�U���ˌW��(�F)
		//XMFLOAT4	globalLightvec;	//�����ʒu
		XMFLOAT2	diffuseFactor;	//�g�U���̔��ˌW��
		XMFLOAT4	ambientColor;
		XMFLOAT4    specularColor;
		XMFLOAT4	shininess;
		int			isTextured;	//�e�N�X�`�����\���Ă��邩�ǂ���
	};

	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer** pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	std::vector<MATERIAL> pMaterialList_;	

	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�

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

