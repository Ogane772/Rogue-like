//////////////////////////////////////////////////
////
////	ゲームオブジェクトクラス
////
//////////////////////////////////////////////////

#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <d3dx9.h>

#define MAX_GAMEOBJ (120)
#define TEXTURE_FILENAME_MAX (64)

class CGameObj
{
public:
	CGameObj();
	CGameObj(int type);
	virtual ~CGameObj();
	typedef enum
	{
		TYPE_LIGHT,  // ライト
		TYPE_CAMERA, //	カメラ
		TYPE_MESH,	 // メッシュフィールド
		TYPE_UI,	 //	UI

		TYPE_MAX

	} OBJTYPE;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Finalize(void) = 0;
	bool Get_Enable(void) { return m_Enable; }

	static void FrameCountReset(void);
	static int Get_FraemCount(void) { return m_FrameCount; }	//	フレームカウント取得
	static int Get_GameObjNum(void) { return m_GameObjNum; }	//	総ゲームオブジェクト取得
	static int Get_GameObjIndex(void) { return m_GameObjNum - 1; }	//	ゲームオブジェクトインデックス

	static void FrameCountUp(void) { m_FrameCount++; }	//  フレームカウントアップ
	static bool Device_Initialize(HWND hWnd);	//	デバイス初期化
	static void Device_Finalize(void);	//	デバイス破棄
	static LPDIRECT3DDEVICE9 m_pD3DDevice;	//	デバイス


	static void UpdateAll();	// 全オブジェクト更新
	static void DrawAll();		// 全オブジェクト描画
	static void DeleteAll2D();	// 全オブジェクト削除
	static CGameObj *Get(int type);				// インスタンス取得
												/*	static CGameObj *Get(int nIdx);				// インスタンス取得
												int Get_GameObjIndex() { return m_GameObjIndex;}	// ワークインデックス取得*/
	int Get_GameType(void) { return m_GameObjType; }		// 種類取得
	int Get_GameIndex(void) { return m_GameObjIndex; }
	static void DebugDraw(void);
protected:

	static int m_FrameCount;	//	フレームカウント　
	static int m_GameObjNum;	//	ゲームオブジェクト総数
	int m_TimeKeep = 0;
	bool m_Enable;
	static bool g_bIsFade;				//フェードフラグ
private:

	int m_GameObjIndex;
	int m_GameObjType;
	static CGameObj *pGameObj[MAX_GAMEOBJ];
	static LPDIRECT3D9 m_pD3D;

};

#endif // !1


