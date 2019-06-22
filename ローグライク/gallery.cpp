#include "scene.h"
#include "fade.h"
#include "input.h"
#include "CTexture.h"
#include "common.h"
#include "bilboard.h"
#include "stage.h"
#include "sound.h"
#include "title_menu.h"
#include "C3DObj.h"
#include "gallery.h"
#include "userinterface.h"
#include "save_load.h"
#define JIXTUSEKI_ADDX (-150)
/*======================================================================
グローバル変数
======================================================================*/
//static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;
static int tFlag = 0;
C2DObj *pGallery;
static int cursor = 0;
static bool tipsflag = false;//データ集開いてるときtrue
static bool galleryflag = false;//ギャラリー開いてるときtrue
C3DObj::NormalModelData CGallery::Gallery_model[GALLERY_MODELMAX];

static D3DXMATRIX mtxWorld;			//	ワールド変換用行列
static D3DXMATRIX mtxTranslation;	//	移動行列
static D3DXMATRIX mtxRotation;		//	移動行列
static D3DXMATRIX mtxScaling;		//	移動行列
static D3DXVECTOR3 Position;
static D3DXVECTOR3 Rotation;			//  角度
static D3DXVECTOR3 Scaling;

static D3DXVECTOR3 m_At;		//	見てる場所	注視点
static float m_AtLength = 0.0; // atまでの長さ
static D3DXVECTOR3 Front;		//	フロントベクトル
static D3DXVECTOR3 Right;	//	ライトベクトル
static D3DXVECTOR3 Up;			//	アップベクトル

static D3DXMATRIX m_mtxView;	//	ビュー変換用行列
static D3DXVECTOR3 CameraPos;	//	カメラ座標


enum menu_command
{
	GALLERY_IROIRO,
	GALLERY_JIXTUSEKI,
	GALLERY_BACK,
};

void CGallery::Gallery_Initialize(void)
{
	Gallery_CameraInitialize();
	pGallery = new C2DObj;
	g_bIsFade = false;
	tFlag++;
	Fade_Start(false, 10, 0, 0, 0);
	cursor = 0;
	tipsflag = false;
	galleryflag = false;
	Gallery_ModelInitialize();
}

void CGallery::Gallery_CameraInitialize(void)
{
	CameraPos = D3DXVECTOR3(0.0f, 0.0f, -6.0f);

	Front = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXVec3Normalize(&Front, &Front);

	Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 外積
	//D3DXVec3Cross(&g_right, &g_front, &g_up);
	// 逆かも
	D3DXVec3Cross(&Right, &Up, &Front);
	D3DXVec3Normalize(&Right, &Right);

	D3DXVec3Cross(&Up, &Front, &Right);
	D3DXVec3Normalize(&Up, &Up);
	m_AtLength = 0.1f;
	m_At = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void CGallery::Gallery_ModelInitialize(void)
{
	C3DObj::InitNormalModelLoad(&Gallery_model[0], "asset/model/playertest.x");
	Position = D3DXVECTOR3(0, 0.0f, 0.0);
	Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Scaling = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	D3DXMatrixScaling(&mtxScaling, Scaling.x, Scaling.y, Scaling.z);
	D3DXMatrixRotationY(&mtxRotation, Rotation.y);
	D3DXMatrixTranslation(&mtxTranslation, Position.x, Position.y, Position.z);

	mtxWorld = mtxTranslation * mtxRotation;

}

void CGallery::Gallery_Finalize(void)
{
	for (int i = 0; i < GALLERY_MODELMAX; i++)
	{
		if (&Gallery_model[i] != NULL)
		{
			NormalModel_Finalize(&Gallery_model[i]);
		}
	}
	delete pGallery;
}

void CGallery::Gallery_Update(void)
{
	if (!g_bIsFade)
	{
		if (Keyboard_IsTrigger(DIK_RETURN))
		{
			if (tipsflag)
			{
				PlaySound(CURSOR_OK_SE);
				tipsflag = false;
				return;
			}
			if (galleryflag)
			{
				PlaySound(CURSOR_OK_SE);
				galleryflag = false;
				return;
			}
			if(!tipsflag && !galleryflag)
			{
				switch (cursor)
				{
				case GALLERY_IROIRO:
					PlaySound(CURSOR_OK_SE);
					tipsflag = true;
					break;
				case GALLERY_JIXTUSEKI:
					PlaySound(CURSOR_OK_SE);
					galleryflag = true;
					break;
				case GALLERY_BACK:
					PlaySound(CURSOR_OK_SE);
					Fade_Start(true, 90, 0, 0, 0);
					g_bIsFade = true;
					break;
				}
			}
		}
		if (!tipsflag && !galleryflag)
		{
			if (Keyboard_IsTrigger(DIK_W))
			{
				PlaySound(CURSOR_SE);
				cursor--;
				if (cursor < GALLERY_IROIRO)
				{
					cursor = GALLERY_BACK;
				}
			}
			if (Keyboard_IsTrigger(DIK_S))
			{
				PlaySound(CURSOR_SE);
				cursor++;
				if (cursor > GALLERY_BACK)
				{
					cursor = GALLERY_IROIRO;
				}
			}
		}
	}
	else
	{
		// フェードが終わったら
		if (!Fade_IsFade())
		{
			g_bIsFade = false;
			Scene_Change(SCENE_INDEX_TITLEMENU);
		}
	}
	//ギャラリーを開いてるときモデルを回転させる
	if (galleryflag)
	{
		Rotation = D3DXVECTOR3(0.0f, 0.01f, 0.0f);
		D3DXMatrixRotationY(&mtxRotation, Rotation.y);
		mtxWorld *=  mtxRotation;
	}
	else//ギャラリーを閉じたときモデルの回転を初期化する
	{
		D3DXMatrixTranslation(&mtxTranslation, 0.0f, -0.6f, 0.0f);
		Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXMatrixRotationY(&mtxRotation, Rotation.y);
		mtxWorld = mtxRotation * mtxScaling * mtxTranslation;
	}
}

void CGallery::Gallery_Draw(void)
{
	Gallery_CameraDraw();
	
	if (galleryflag)
	{
		Gallery_ModelDraw();
	}
	//左のウィンドウ描画
	pGallery->m_Sprite_Draw(CTexture::TEX_BLACK, 0, 0, 0, 0, pGallery->Texture_GetWidth(CTexture::TEX_BLACK), pGallery->Texture_GetHeight(CTexture::TEX_BLACK));
	pGallery->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 50.0f, 50.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 1.7f, 0.58f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(105, 75, D3DCOLOR_RGBA(255, 255, 255, 255), "冒険の記憶");
	CUserinterface::UI_TextDraw(105, 125, D3DCOLOR_RGBA(255, 255, 255, 255), "実績");
	CUserinterface::UI_TextDraw(105, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "戻る");

	if (!tipsflag && !galleryflag)
	{
		pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 75.0f, 75.0f + (cursor * 50.0f), 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	else if (tipsflag || galleryflag)
	{
		pGallery->Sprite_Draw(CTexture::TEX_CURSOL_GRY, 75.0f, 75.0f + (cursor * 50.0f), 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL_GRY), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL_GRY), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
	}
	if (tipsflag)
	{
		Tips_Draw();
	}
	if (galleryflag)
	{
		Gallery_JIXTUSEKI_Draw();
	}
}

void CGallery::Gallery_CameraDraw(void)
{
	//カメラ
	// カメラの振り向き　回転行列
	D3DXMATRIX mtxR;        // 1フレームに回転する角度
							//g_Came_at = (g_front * g_length) + g_Cposition;
							// 攻撃時以外はカメラをプレイヤーに追従

							// ビュー座標変換行列の作成と設定
							// カメラ座標
	D3DXVECTOR3 eye(5.0f, 5.0f, -10.0f);
	// 見てる場所(注視点)
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
	// カメラの上方向ベクトル
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);


	//g_front = D3DXVECTOR3(0, -0.1f, 1.0f);
	//D3DXVec3TransformNormal(&g_front, &g_front);

	D3DXMatrixLookAtLH(&m_mtxView, &CameraPos, &m_At, &up);

	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクション座標変換行列の作成と設定
	D3DXMATRIX mtxProjection;				// 視野角120°にしたい								nearには0以下禁止 読み込む距離
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 25.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

}

void CGallery::Gallery_ModelDraw(void)
{
	unsigned int i;
	
	// マトリックスのセット
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルのレンダリング
	for (i = 0; i < Gallery_model[0].dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&Gallery_model[0].pMaterials[i]);
		m_pD3DDevice->SetTexture(0, Gallery_model[0].pTextures[i]);
		Gallery_model[0].pMesh->DrawSubset(i);
	}
}

void CGallery::Tips_Draw(void)
{
	SAVE_COUNT *save;
	save = Get_SaveCount();
	pGallery->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 300.0f, 40.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 4.2f, 1.8f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(355, 75, D3DCOLOR_RGBA(255, 255, 255, 255), "冒険回数 %d", save->danjyon_in);
	CUserinterface::UI_TextDraw(355, 125, D3DCOLOR_RGBA(255, 255, 255, 255), "合計突破階層数 %d", save->floor_count);
	CUserinterface::UI_TextDraw(355, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "ゲームクリア数 %d", save->game_clear);
	CUserinterface::UI_TextDraw(355, 225, D3DCOLOR_RGBA(255, 255, 255, 255), "歩いた歩数 %d", save->walk_count);
	CUserinterface::UI_TextDraw(355, 275, D3DCOLOR_RGBA(255, 255, 255, 255), "アイテムを使用した回数 %d", save->item_count);
	CUserinterface::UI_TextDraw(355, 325, D3DCOLOR_RGBA(255, 255, 255, 255), "やられた回数 %d",save->death_count);
	CUserinterface::UI_TextDraw(355, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "合計与ダメージ %d",save->damage_count);
	CUserinterface::UI_TextDraw(355, 425, D3DCOLOR_RGBA(255, 255, 255, 255), "合計被ダメージ %d",save->damage_on_count);
	CUserinterface::UI_TextDraw(355, 475, D3DCOLOR_RGBA(255, 255, 255, 255), "倒した敵の数 %d",save->enemy_death_count);
	CUserinterface::UI_TextDraw(355, 525, D3DCOLOR_RGBA(255, 255, 255, 255), "武器を切り替えた回数 %d", save->wepon_chenge_count);
	CUserinterface::UI_TextDraw(355, 575, D3DCOLOR_RGBA(255, 255, 255, 255), "   戻る");
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 355.0f, 575.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
}

void CGallery::Gallery_JIXTUSEKI_Draw(void)
{
	pGallery->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 325.0f + JIXTUSEKI_ADDX, 450.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 5.4f, 0.5f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(395 + JIXTUSEKI_ADDX, 465, D3DCOLOR_RGBA(255, 255, 255, 255), "剣の勇者の像・銅");
	CUserinterface::UI_TextDraw(395 + JIXTUSEKI_ADDX, 515, D3DCOLOR_RGBA(255, 255, 255, 255), "低ランクの剣でクリアしたものに送られる銅像");
	CUserinterface::UI_TextDraw(395 + JIXTUSEKI_ADDX, 565, D3DCOLOR_RGBA(255, 255, 255, 255), "   戻る");
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 395.0f + JIXTUSEKI_ADDX, 565.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);

	//左右のカーソル
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 925.0f - 70, 200.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 475.0f - 70, 256.3f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.3f, 0.3f, D3DXToRadian(180.0f));
}