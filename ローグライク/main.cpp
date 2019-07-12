//=============================================================================
//
// ゲームループ　[main.cpp]
// Author : WATARU SODA
// Date	  : 2018/09/20
//
//=============================================================================

//=============================================================================
//	インクルードファイル
//=============================================================================
#define NOMINMAX
/*#include <Windows.h>
#undef min
#undef max*/

#include <d3dx9.h>
#include "common.h"
#include "scene.h"
#include "input.h"
#include "system_timer.h"
#include "debug_font.h"
#include "gamepad.h"
#include "CObject.h"
#include "CTrap.h"
#include "CWepon.h"
#include "fade.h"
#include "sound.h"
#include "C2DObj.h"

#include "C3DObj.h"
#include "Cplayer.h"
#include "CLight.h"
#include "userInterface.h"
#include "CEnemy.h"
#include "CCamera.h"
#include "save_load.h"
//=============================================================================
//	定数定義
//=============================================================================
#define CLASS_NAME		("GameWindow")
#define WINDOW_CAPTION	("タイニーラビリンス")
#define WINDOW_STYLE	(WS_OVERLAPPEDWINDOW - WS_MAXIMIZEBOX - WS_THICKFRAME)	// ウィンドウスタイル


//	#defineは()を付けとく


//=============================================================================
//	プロトタイプ宣言
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//　ゲームの初期化関数
static bool Begin(HINSTANCE hInstance, HWND hWnd);
//　ゲームの更新関数
static void Update(void);
//　ゲームの描画関数
static void Draw(void);
//　ゲームの修了処理
static void End(void);

//　static付ける

//=============================================================================
//	グローバル変数宣言
//=============================================================================

static  HWND hWnd;
#if defined(_DEBUG) || defined(DEBUG)
static int g_FrameCount = 0;	// フレームカウンター
static int g_BaseFrame = 0;		// FPS計測の基となるフレームカウント
static double g_BaseTime = 0;	// FPS計測の基となる時間
static float g_FPS = 0.0f;		// FPS
#endif // _DEBUG || DEBUG
								//static double g_
								// LPDIRECT3DTEXTURE9 g_pTexture;	//	テクスチャのアドレス帳
								//　static付ける
static double g_StaticFrameTime = 0.0f; // フレーム固定用計測時間
static BYTE KeyTbl[256];
//=============================================================================
//	メイン
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance, LPSTR lpCmdline, int nCmdShow)
{
	// 使用しない一時変数を明示
	UNREFERENCED_PARAMETER(hprevInstance);
	UNREFERENCED_PARAMETER(lpCmdline);

	// ウィンドウクラス構造体の設定
	// ウィンドウクラス構造体の設定
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;                          // ウィンドウプロシージャの指定
	wc.lpszClassName = CLASS_NAME;                     // クラス名の設定
	wc.hInstance = hInstance;                          // インスタンスハンドルの指定
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);          // マウスカーソルを指定
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1); // ウインドウのクライアント領域の背景色を設定
													   //アイコン指定
	wc.hIconSm = NULL;
	// クラス登録
	RegisterClassEx(&wc);


	// ウィンドウスタイル
	DWORD window_style = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	// 基本矩形座標
	RECT window_rect = { 0, 0, (LONG)WINDOW_WIDTH, (LONG)WINDOW_HEIGHT };

	// 指定したクライアント領域を確保するために新たな矩形座標を計算
	AdjustWindowRect(&window_rect, window_style, FALSE);
	int window_width = 0;
	int window_height = 0;
	// 新たなWindowの矩形座標から幅と高さを算出
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;

	int screen_width = 0;
	int screen_hight = 0;
	// プライマリモニターの画面解像度取得
	//　window座標 プライマリモニターの画面解像度取得
	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_hight = GetSystemMetrics(SM_CYSCREEN);

	// デスクトップの真ん中にウィンドウが生成されるように座標を計算
	// ※ただし万が一、デスクトップよりウィンドウが大きい場合は左上に表示
	int window_x = (((screen_width - window_width) / 2 > (0)) ? ((screen_width - window_width) / 2) : (0));  //　max(a,b) a>bならa a<bならb
	int window_y = (screen_hight / 2) - (window_height / 2);

	// ウィンドウの生成
	HWND hWnd = CreateWindow(
		CLASS_NAME,     // ウィンドウクラス
		WINDOW_CAPTION, // ウィンドウテキスト
		window_style,   // ウィンドウスタイル
		-8,       // ウィンドウ座標x
		window_y,       // ウィンドウ座標y
		window_width,   // ウィンドウの幅
		window_height,  // ウィンドウの高さ
		NULL,           // 親ウィンドウハンドル
		NULL,           // メニューハンドル
		hInstance,      // インスタンスハンドル
		NULL            // 追加のアプリケーションデータ
	);

	if (hWnd == NULL) {
		// ウィンドウハンドルが何らかの理由で生成出来なかった
		return -1;
	}

	// 指定のウィンドウハンドルのウィンドウを指定の方法で表示
	ShowWindow(hWnd, nCmdShow);


	// ゲームの初期化　(Direct3Dの初期化)
	if (!Begin(hInstance, hWnd))
	{
		return -1;
	}



	//	ゲームループ
	MSG msg = {};
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	//	ゲームループ　戻値：メッセージがあったら0以外　GetMessageと同じ　PM_REMOVEは気にしない
		{
			TranslateMessage(&msg);		//	キー入力メッセージだけ加工	順番①
			DispatchMessage(&msg);		//　メッセージをウィンドウプロシージャに送る	順番②
		}
		else {
			// 現在のシステム時間を取得
			double time = SystemTimer_GetTime();

			if (time - g_StaticFrameTime < 1.0 / 60.0) {
				// 1 / 60 秒経っていなかったら空回り
				Sleep(0);
			}
			else {
				// フレーム固定用の計測時間を更新する
				g_StaticFrameTime = time;

				// ゲームの更新
				Update();
				// ゲームの描画
				Draw();
			}
		}
	} while (msg.message != WM_QUIT);

	//　ゲームの終了処理(Direct3Dの終了処理)
	End();

	return (int)msg.wParam;
}

// ウィンドウプロシージャ(コールバック関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0); // WM_CLOSEメッセージの送信
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了してよろしいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd); // 指定のウィンドウにWM_DESTROYメッセージを送る
		}
		return 0;	// DefWindowProc関数にメッセージを流さず終了することによって何もなかったことにする

	case WM_DESTROY:	// ウィンドウの破棄メッセージ
		PostQuitMessage(0); // WM_QUITメッセージの送信
		return 0;
	};
	// 通常メッセージ処理はこの関数に任せる
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//　ゲームの初期化関数
bool Begin(HINSTANCE hInstance, HWND hWnd)
{
	//　Direct3Dの初期化
	if (!CGameObj::Device_Initialize(hWnd))
	{
		return false;
	}
	/*
	if (!InitSound(hWnd))
	{
		return false;
	}*/
	// キーボードの初期化
	if (!Keyboard_Initialize(hInstance, hWnd))
	{
		return false;
	}
	//ゲームパッドの初期化
	if (!Gamepad_Initialize(hInstance, hWnd))
	{
		return false;
	}


	// スプライトの初期化
	C2DObj::Sprite_Initialize();
	// テクスチャの初期化
	C2DObj::Texture_Load();

	// サウンドの初期化
	InitSound(hWnd);

	// フェードの初期化
	Fade_Initialize();

	//セーブデータのロード
	Load();

	//	システムタイマーの初期化
	SystemTimer_Initialize();
	g_BaseFrame = 0;
	g_BaseTime = 0;

	// システムタイマーの起動
	SystemTimer_Start();

#if defined(_DEBUG) || defined(DEBUG)
	// デバックフォントモジュールの初期化
	DebugFont_Initialize();
	//DebugPrimitive_Initialize();
	// FPS計測用変数群の初期化
	g_FrameCount = g_BaseFrame = 0;
	g_BaseTime = SystemTimer_GetTime();
	g_FPS = 0.0f;
#endif // _DEBUG || DEBUG

	// フレーム固定用計測時間
	//g_StaticFrameTime = SystemTimer_GetTime();

	//CSVの読み込み
	CObject::ObjectDataInit();
	CWepon::WeponDataInit();
 	CEnemy::EnemyDataInit();
	CTrap::TrapDataInit();

	CUserinterface::UI_Initialize();
											//CCamera::Camera_Create();				//	カメラ生成					//	地面生成																									//CMeshField_Cylinder::MeshField_Cylinder_Create(CTexture::TEX_FLORR, 10.0f, SYLINDERSIZE, 20, 1,true);	//	内カベ生成


	//　シーンの初期化
	Scene_Initialize(SCENE_INDEX_TITLE);

	return true;
}
//　ゲームの更新関数
void Update(void)
{
	// キーボードの更新
	Keyboard_Update();
	// シーンの更新
	Scene_Update();
	// フェードの更新
	Fade_Update();


	//　計測
#if defined(_DEBUG) || defined(DEBUG)

	// 現在のシステム時間を取得
	double time = SystemTimer_GetTime();

	// 前回のFPS計測時間から規定時間経っていたらFPS計測
	if (time - g_BaseTime > 1.0)
	{
	// FPS計算(前回からの経過フレーム数÷経過時間)
	g_FPS = (float)((g_FrameCount - g_BaseFrame )/ (time - g_BaseTime));

	// FPS計測の基となる変数の更新
	g_BaseTime = time;
	g_BaseFrame = g_FrameCount;

	}

	// フレームカウントの更新
	g_FrameCount++;
#endif // _DEBUG || DEBUG
}
//　ゲームの描画関数
void Draw(void)
{

	if (!CGameObj::m_pD3DDevice)
	{
		return;
	}

	// 画面のクリア
	CGameObj::m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR(/*0xffafa690*/0xff454341), 1.0f, 0);		//画面クリア
																																	/*
																																	D3DCLEAR_TARGET		色のクリア
																																	D3DCLEAR_ZBUFFER	ステンシルクリア
																																	D3DCOLOR_RGBA		何色でクリアするか

																																	第四引数　0.0fが一番手前1.0fが一番奥
																																	*/

																																	//　描画処理開始
	CGameObj::m_pD3DDevice->BeginScene();
#if defined(_DEBUG) || defined(DEBUG)
	//DebugPrimitive_BatchBegin();
#endif // _DEBUG || DEBUG

	// シーンの描画
	Scene_Draw();
	// フェードの描画
	Fade_Draw();

#if defined(_DEBUG) || defined(DEBUG)
	// FPSのデバック表示
	DebugFont_Draw(8, 8, "FPS : %.2f", g_FPS);
	//DebugPrimitive_BatchRun();
#endif // _DEBUG || DEBUG



	//　描画処理終了
	CGameObj::m_pD3DDevice->EndScene();
	// バックバッファをフリップ（タイミングはD3DPRESENT_PARAMETERSの設定による）
	CGameObj::m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	Scene_Check();		//	シーンのチェック
}
//　ゲームの修了処理
void End(void)
{
	//DebugPrimitive_Fainalize();
#if defined(_DEBUG) || defined(DEBUG)
	// デバックフォントモジュールの終了処理
	//DebugPrimitive_Fainalize();
	//DebugFont_Finalize();
#endif // _DEBUG || DEBUG

	// シーン終了処理
	Scene_Finalize();

	CUserinterface::UI_Finalize();

	C3DObj::DeleteAll();			//	3Dオブジェクト全消去
	CGameObj::DeleteAll2D();			//	2Dオブジェクト全消去
	C3DObj::Model_Finalize();
	// サウンド終了処理
	UninitSound();

	// テクスチャの解放
	//Texture_Release();
	C2DObj::Texture_Release();

	// スプライトの開放
	//Sprite_Finalize();
	C2DObj::Sprite_Finalize();
	DebugFont_Finalize();
	// キーボード終了処理
	Keyboard_Finalize();
	//ゲームパッド終了処理
	Gamepad_Finalize();
	// デバイスの開放
	CGameObj::Device_Finalize();
	_CrtDumpMemoryLeaks();

}