//=============================================================================
//
// ゲームパッド入力処理 [gamepad.cpp]
//
//=============================================================================
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <d3dx9.h>
#include <stdio.h>
#include "gamepad.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_SPEED 6

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static bool initialize(HINSTANCE hInstance);
static void finalize(void);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3D9 pD3d = NULL;
LPDIRECT3DDEVICE9 pDevice = NULL;
LPDIRECTINPUT8 pDinput = NULL;
LPDIRECTINPUTDEVICE8 pJoyDevice = NULL;

//=============================================================================
// ゲームパッド入力処理の初期化
//=============================================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
	//複数列挙される場合、ユーザーに選択・確認させる
	//TCHAR szConfirm[MAX_PATH + 1];
	// 「DirectInputデバイス」オブジェクトの作成
	if (FAILED(pDinput->CreateDevice(pdidInstance->guidInstance,
		&pJoyDevice, NULL)))
	{
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

//=============================================================================
// ゲームパッド入力処理の終了処理
//=============================================================================
void Gamepad_Finalize(void)
{
	if (pDinput != NULL) {

		// DirectInputオブジェクトの開放
		pDinput->Release();
		pDinput = NULL;
	}
	if (pJoyDevice != NULL) {

		// DirectInputオブジェクトの開放
		pJoyDevice->Release();
		pJoyDevice = NULL;
	}
}

//BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,VOID* pContext )
//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{

	if (pdidoi->dwType & DIDFT_AXIS)
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(DIPROPRANGE);
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		diprg.diph.dwHow = DIPH_BYID;
		diprg.diph.dwObj = pdidoi->dwType;
		diprg.lMin = -MOVE_SPEED;
		diprg.lMax = MOVE_SPEED;

		if (FAILED(pJoyDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//=============================================================================
// ゲームパッドの初期化
//=============================================================================
HRESULT Gamepad_Initialize(HINSTANCE hInstance, HWND hWnd)
{
	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDinput, NULL)))
	{
		return E_FAIL;
	}
	//利用可能なゲームコントローラーの列挙関数を実行
	if (FAILED(pDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,
		NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (!pJoyDevice)
	{
		return E_FAIL;
	}
	// デバイスをジョイスティックに設定
	if (FAILED(pJoyDevice->SetDataFormat(&c_dfDIJoystick2)))
	{
		return E_FAIL;
	}
	// 協調レベルの設定
	if (FAILED(pJoyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return E_FAIL;
	}
	//アプリケーションで使用するコントローラーのプロパティを列挙して設定する
	if (FAILED(pJoyDevice->EnumObjects(EnumObjectsCallback,
		NULL, DIDFT_ALL)))
	{
		return E_FAIL;
	}
	// デバイスを「取得」する
	pJoyDevice->Acquire();

	return true;
}

LPDIRECTINPUTDEVICE8* JoyDevice_Get(void)
{
	return &pJoyDevice;
}

