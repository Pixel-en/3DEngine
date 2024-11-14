
#include <Windows.h>
#include <stdlib.h>
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/RootJob.h"
#include "Engine/Input.h"
#include "Engine/Model.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")

const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //ウィンドウクラス名
const wchar_t* APP_NAME = L"サンプルゲーム";

RootJob* pRootjob = nullptr;	//ルートジョブ(すべての親)



//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{

	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);              //この構造体のサイズ
	wc.hInstance = hInstance;                    //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;           //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                    //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;          //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_SHIELD);     //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_NO);       //マウスカーソル
	wc.lpszMenuName = NULL;						 //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
	RegisterClassEx(&wc);  //クラスを登録

	//ウィンドウサイズの計算
	//(表示領域をWINDOW WIDTHxWINDOW HEIGHTに指定するための計算)
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		APP_NAME,     //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,                 //ウィンドウ幅
		winH,                 //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);
	//Direct3D初期化
	if (FAILED(Direct3D::Initialize(winW, winH, hWnd)))
		return 0;

	if (FAILED(Input::Initialize(hWnd)))
		return 0;

	Camera::Initialize();

	pRootjob = new RootJob(nullptr);
	pRootjob->Initialize();

	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{


		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



		//メッセージなし
		else
		{
			timeBeginPeriod(1);
			static DWORD countFps = 0;
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();
			static DWORD lastUpdateTime = nowTime;
			timeEndPeriod(1);

			if (nowTime - startTime >= 1000) {

				std::wstring str;

				wsprintf(str.data(), L"%u", countFps);
				SetWindowTextW(hWnd, str.c_str());
				countFps = 0;
				startTime = nowTime;
			}
			
			//1/60以上経過してないのでスルー
			if (nowTime - lastUpdateTime <= 1000.0f / 60.0f)
				continue;

			//1/60秒以上経過しているので更新
			lastUpdateTime = nowTime;

			countFps++;

			Input::Update();

			Camera::Update();

			pRootjob->UpdateSub();

			Direct3D::BeginDraw();

			//ルートジョブのDraw
			pRootjob->DrawSub();
			

			Direct3D::EndDraw();

		}
	}
	Model::Release();

	pRootjob->ReleaseSub();
	Input::Release();
	//解放処理
	Direct3D::Release();

	return 0;
}

//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:	//ウィンドウが閉じられたイベント
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
