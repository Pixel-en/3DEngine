#pragma once
#include <d3d11.h>
#include <assert.h>	//製作者用のデバッグ
#include <DirectXMath.h>

enum SHADER_TYPE {
	SHADER_2D,
	SHADER_3D,
	SHADER_CRT,
	SHADER_MAX
};

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ


namespace Direct3D
{

	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	extern DirectX::XMINT2 WindowSize;

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderCRT();

	void SetShader(SHADER_TYPE type);

	//描画開始
	void BeginDraw();

	//描画終了
	HRESULT EndDraw();

	//解放
	void Release();

	void SetGlobalLightVec(DirectX::XMFLOAT4 lv);
	DirectX::XMFLOAT4 GetGlovalLightVec();
};