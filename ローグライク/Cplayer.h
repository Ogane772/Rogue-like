#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "C3DObj.h"
#include <d3dx9.h>
#include "collision.h"
#define ATTACK_END (55)//攻撃の待機時間
#define NOATTACK_END (15) //空振り攻撃の待機時間

#define PLAYER_RADIUS	 (2.6f)				// 当たり判定半径
#define PLAYER_ENERADIUS (5.0f)				// エネミー専用当たり判定
#define PLAYER_RECOVERY (100)				// 自然回復 = MAXHP / PLAYER_RECOVERY
#define PLAYER_MAXHP (500)
#define PLAYER_STR (6)
#define PLAYER_HEAL (m_MaxHp / 5000)	//歩いたときの回復量
#define WALK_COUNT (9) //ウォークカウント基礎値
//0.25 19 
//0.5 9
class CPlayer :public C3DObj
{

public:

	CPlayer();
	~CPlayer();


	static void Player_SetPos(int z, int x);		// 階層ごとにプレイヤーの初期位置を決める
	void Update(void);
	void Draw(void);
	void Finalize(void);	//	終了処理

	static CPlayer *PlayerCreate(void);
	static const Sphere* Player_GetCollision(void) { return &m_Colision; }
	static const Sphere* Player_GetEnemyCollision(void) { return &m_EnemyColision; }
	static void Player_NextTurn(void);
	bool Damage(int str);
	bool Get_DrawCheck(void) { return alive; }

	static C3DObj *Get_Player(void);
	// マップ二次元配列用

	static bool goladder;	// 梯子
protected:

	bool m_DrawCheck;			//	描画フラグ
private:
	void Initialize(void);	//	初期化
	void Player_Move(void);
	void Player_MoveChenge(int MoveType);			//	移動処理の準備
	void Player_lefttopMove(void);
	void Player_righttopMove(void);
	void Player_leftbottomMove(void);
	void Player_rightbottomMove(void);
	void Player_GamepadMove(void);			//	ゲームパッドの移動処理
	void AngleChange(float Angle);	//	方向変換
	void ControllerAngleChange(int index);	//	コントローラーの方向変換
	void Player_Act(void);
	void Player_Destroy(void);
	void Player_Check(void);//プレイヤーの当たり判定
	bool JoyDevice_IsTrigger(int nKey);//コントローラーのトリガー処理
	int m_PadDirection;
	enum
	{
		DIRE_TOP,		//　上
		DIRE_RIGHT,		//	右
		DIRE_BOTTOM,		//　下
		DIRE_LEFT,		//　左
		DIRE_TOP_RIGHT,	//	右上
		DIRE_TOP_LEFT,	//	左上
		DIRE_BOTTOM_RIGHT,//	右下
		DIRE_BOTTOM_LEFT,	//	左下

		DIRE_N,

	};//　方向


	static CPlayer *m_pPlayer;

	int m_DrawCount;
	bool right_trigger = false;
	bool left_trigger = false;
	bool right_on;//右スティック入力確認

	static Sphere m_Colision;		// 当たり判定
	
	int charatype;		// キャラクターの種類

	
	D3DXVECTOR3 walkpos;	// 移動する場合の目的地セット

	D3DXVECTOR3 vecplayer;
	D3DXVECTOR3 vec2player;	// 斜め用

	static D3DXVECTOR3 start_m_Position;//開始時の位置
	static int start_m_Mapz;
	static int start_m_Mapx;
};



#endif // !

