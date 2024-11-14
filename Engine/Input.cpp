#include "Input.h"

namespace Input
{
	//キーボード
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256];    //前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8 pMouDevice = nullptr;
	DIMOUSESTATE mouseState;
	DIMOUSESTATE prevmouseState;

	XMVECTOR mousePosition;

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr=S_OK;

		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);

		if (FAILED(hr)) {
			MessageBox(NULL, L"ダイレクトインプットの作成に失敗しました", L"エラー", MB_OK);
		}

		//キーボードの準備
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		
		if (FAILED(hr)) {
			MessageBox(NULL, L"キーボードの作成に失敗しました", L"エラー", MB_OK);
		}

		//マウスの準備
		hr = pDInput->CreateDevice(GUID_SysMouse, &pMouDevice, nullptr);
		pMouDevice->SetDataFormat(&c_dfDIMouse);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		if (FAILED(hr)) {
			MessageBox(NULL, L"マウスの作成に失敗しました", L"エラー", MB_OK);
		}

		return hr;
	}

	void Update()
	{
		memcpy(&prevKeyState, &keyState, sizeof(keyState));

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);	//ここで状態が入る


		memcpy(&prevmouseState, &mouseState, sizeof(mouseState));
		pMouDevice->Acquire();
		pMouDevice->GetDeviceState(sizeof(mouseState),&mouseState);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)	//ビットの演算子してるヨ
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		if (IsKey(keyCode) && keyState[keyCode] != prevKeyState[keyCode])
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (!IsKey(keyCode) && keyState[keyCode] != prevKeyState[keyCode])
		{
			return true;
		}
		return false;
	}

	//マウス
	bool IsMouse(int buttonCode)
	{
		if (mouseState.rgbButtons[buttonCode] & 0x80)	//ビットの演算子してるヨ
		{
			return true;
		}
		return false;
	}

	bool IsMouseDown(int buttonCode)
	{
		if (IsMouse(buttonCode) && mouseState.rgbButtons[buttonCode] != prevmouseState.rgbButtons[buttonCode])
		{
			return true;
		}
		return false;
	}

	bool IsMouseUp(int buttonCode)
	{
		if (!IsMouse(buttonCode) && mouseState.rgbButtons[buttonCode] != prevmouseState.rgbButtons[buttonCode])
		{
			return true;
		}
		return false;
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pMouDevice);
	}

}