//////////////////////////////////////////////////
////
////	3Dオブジェクトクラス
////
//////////////////////////////////////////////////

#include "C3DObj.h"
#include "debug_font.h"
#include "CTexture.h"
#include "Cplayer.h"
#include "exp.h"
#include "CCamera.h"
#include "wall.h"
#include "common.h"
#define _CRTDBG_MAP_ALLOC
#define VF_ANGLE (1.0f)//カリングの大体の大きさ
#define ADD_HP (3)//HP回復量
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	静的変数
//===========================================================================
C3DObj *C3DObj::p3DObj[MAX_GAMEOBJ];
int C3DObj::m_3DObjNum = 0;

C3DObj::MaterialFileData C3DObj::NORMAL_MODEL_FILES[] = {
	{ "asset/model/sraim1_10.x" },
	{ "asset/model/sraimv3.x" },
};
//	使いたいアニメモデルの数だけ書く
C3DObj::MaterialFileData2 C3DObj::ANIME_MODEL_FILES[] = {
	{ "asset/anime_model/hewplayer_judge->x" },
	{ "asset/anime_model/small_enemy.x" },
	{ "asset/anime_model/middle_enemy.x" },
	{ "asset/anime_model/special_enemy.x" },
	{ "asset/anime_model/big_enemy.x" },
};
int C3DObj::MODEL_FILES_MAX = sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0]);
int C3DObj::ANIME_MODEL_FILES_MAX = sizeof(C3DObj::ANIME_MODEL_FILES) / sizeof(ANIME_MODEL_FILES[0]);

C3DObj::NormalModelData C3DObj::NormalModel[(sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0]))];//読み込むモデルの最大数+1


//=============================================================================
//	生成
//=============================================================================
C3DObj::C3DObj()
{
	for (m_3DObjIndex = 0; m_3DObjIndex < MAX_GAMEOBJ; m_3DObjIndex++)
	{
		if (p3DObj[m_3DObjIndex] == NULL)
		{
			p3DObj[m_3DObjIndex] = this;
			m_3DObjType = TYPE_etc;
			m_3DObjNum++;
			break;
		}
	}
	// ワーク格納失敗
	if (m_3DObjIndex >= MAX_GAMEOBJ)
	{
		m_3DObjIndex = -1;
	}


}

C3DObj::C3DObj(int type)
{
	// ワークに自分自身(this)を格納
	for (m_3DObjIndex = 0; m_3DObjIndex < MAX_GAMEOBJ; m_3DObjIndex++)
	{
		if (p3DObj[m_3DObjIndex] == NULL)
		{
			p3DObj[m_3DObjIndex] = this;
			m_3DObjType = type;
			m_3DObjNum++;
			break;
		}
	}
	// ワーク格納失敗
	if (m_3DObjIndex >= MAX_GAMEOBJ)
	{
		m_3DObjIndex = -1;
	}

	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXMatrixIdentity(&m_mtxTranslation);
	D3DXMatrixIdentity(&m_mtxRotation);
	D3DXMatrixIdentity(&m_mtxScaling);
	m_Enable = false;
}
//=============================================================================
//	破棄
//=============================================================================
C3DObj::~C3DObj()
{
	if (Thing.pSphereMeshMaterials)
	{
		//delete Thing.pSphereMeshMaterials;
		for (DWORD i = 0; i < Thing.dwNumMaterials; i++)
		{
			delete[]Thing.pSphereMeshMaterials;
		}
	}
	if (Thing.pFrameRoot)
	{
		SkinMesh.cHierarchy.DestroyFrame(Thing.pFrameRoot);
	}

	/*if (Thing.pSphereMeshMaterials)
	{
	for (DWORD i = 0; i < Thing.dwNumMaterials; i++)
	{
	delete[]Thing.pSphereMeshMaterials;
	}
	}*/
	THING();

	//delete &SkinMesh;

	m_3DObjNum--;
	p3DObj[m_3DObjIndex] = NULL;

}

void C3DObj::FinalizeAll()
{
	for (int i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのUpdate()が呼ばれる
		if (p3DObj[i])
		{
			p3DObj[i]->Finalize();
		}
	}
}
void C3DObj::Finalize()
{

}


//=============================================================================
// 全オブジェクト更新
//=============================================================================
void C3DObj::UpdateAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのUpdate()が呼ばれる
		if (p3DObj[i])
		{
			p3DObj[i]->Update();
		}
	}
}

//=============================================================================
// 全オブジェクト描画
//=============================================================================
void C3DObj::DrawAll()
{
	int i;
	int dc = 0;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// ポリモーフィズムによって派生クラスのDraw()が呼ばれる
		if (p3DObj[i])
		{

			if (!p3DObj[i]->Thing.pFrameRoot)
			{
				if (VFCulling(&p3DObj[i]->m_Position))
				{
					p3DObj[i]->Draw();
				}
				else
				{
					//画面外の時はテクスチャを更新
					//p3DObj[i]->SkinMesh.DrawFrame(m_pD3DDevice, p3DObj[i]->Thing_Normal_model.pFrameRoot, &p3DObj[i]->Thing, false);
				}
			}
			else
			{
				if (VFCulling(&p3DObj[i]->Thing.vPosition))
				{
					p3DObj[i]->Draw();
					//dc++;
				}
				else
				{
					//画面外の時はテクスチャを更新
					p3DObj[i]->SkinMesh.DrawFrame(m_pD3DDevice, p3DObj[i]->Thing.pFrameRoot, &p3DObj[i]->Thing, false);
				}
			}
		}
	}
}



//=============================================================================
// 全オブジェクト削除
//=============================================================================
void C3DObj::DeleteAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		if (p3DObj[i])
		{
			delete p3DObj[i];
			//p3DObj[i] = NULL;
		}
	}
}


//=============================================================================
// インスタンス取得
//=============================================================================
C3DObj *C3DObj::Get(int nIdx)
{
	// インデックスが範囲外
	if (p3DObj[nIdx])
	{
		return p3DObj[nIdx];
	}
	return NULL;
}


//=============================================================================
// オブジェクト削除
//=============================================================================
void C3DObj::C3DObj_delete(void)
{
	C3DObj *pobj = Get(Get_3DObjIndex());
	delete pobj;
}



//=============================================================================
// モデル読み込み
//=============================================================================
// Geometryの初期化（モデルの読み込み）
HRESULT C3DObj::InitModelLoad()
{
	int i;
	//通常モデル読み込み	
	for (i = 0; i < MODEL_FILES_MAX; i++)
	{
		InitNormalModelLoad(&NormalModel[i], NORMAL_MODEL_FILES[i].filename);
	}

	return S_OK;
}

HRESULT C3DObj::InitNormalModelLoad(NormalModelData *pNomalModel, LPSTR szXFileName)
{
	/////////
	// Xファイルからメッシュをロードする	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(szXFileName, D3DXMESH_SYSTEMMEM,
		m_pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&pNomalModel->dwNumMaterials, &pNomalModel->pMesh)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", szXFileName, MB_OK);
		return E_FAIL;
	}
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pNomalModel->pMaterials = new D3DMATERIAL9[pNomalModel->dwNumMaterials];
	pNomalModel->pTextures = new LPDIRECT3DTEXTURE9[pNomalModel->dwNumMaterials];

	for (DWORD i = 0; i<pNomalModel->dwNumMaterials; i++)
	{
		pNomalModel->pMaterials[i] = d3dxMaterials[i].MatD3D;
		pNomalModel->pMaterials[i].Ambient = pNomalModel->pMaterials[i].Diffuse;
		pNomalModel->pTextures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			if (FAILED(D3DXCreateTextureFromFile(m_pD3DDevice,
				d3dxMaterials[i].pTextureFilename,
				&pNomalModel->pTextures[i])))
			{
				MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();

	return S_OK;
}

//=============================================================================
// モデル破棄
//=============================================================================
void C3DObj::Model_Finalize(void)	//	モデルデータの開放　複数化したら全部消すかどれかを消す
{

}

void C3DObj::NormalModel_Finalize(NormalModelData *DeleteNormalModel)	//	モデルデータの開放
{
	for (int i = 0; i < DeleteNormalModel->dwNumMaterials; i++)
	{
		//オブジェクトのコピーをしているせいでエラーが出るので直接モデルを読むこと
		if (DeleteNormalModel->pMesh != NULL)
		{
			//DeleteNormalModel->pMesh->Release();
			//DeleteNormalModel->pMesh = NULL;
		}
		if (DeleteNormalModel->pTextures[i] != NULL)
		{
			//delete[] DeleteNormalModel->pTextures[i];
		}
		if (DeleteNormalModel->pMaterials != NULL)
		{
			//delete DeleteNormalModel->pMaterials;
		}
	}
}

void C3DObj::PlayerVsWall(JUDGE *player_judge, Sphere *m_PlayerColision)
{
	for (int i = 0; i < WALL_MAX; i++)
	{
		if (CWall::Wall_GetData(i).wuse)
		{
			for (int c = 0; c < WALL_WIDTH_MAX; c++)
			{
				// プレイヤーと壁の当たり判定
				if (Collision_IsHitVtoS(CWall::Wall_GetCollision(i, c), m_PlayerColision))
				{
					if (Collision_IsHitTopVtoS(CWall::Wall_GetCollision(i, c), m_PlayerColision))
					{
						player_judge->HitTop = true;
						player_judge->HitTopLeft = true;
						player_judge->HitTopRight = true;
					}

					if (Collision_IsHitBottomVtoS(CWall::Wall_GetCollision(i, c), m_PlayerColision))
					{
						player_judge->HitBottom = true;
						player_judge->HitBottomLeft = true;
						player_judge->HitBottomRight = true;
					}

					if (Collision_IsHitRightVtoS(CWall::Wall_GetCollision(i, c), m_PlayerColision))
					{
						player_judge->HitRight = true;
						player_judge->HitTopRight = true;
						player_judge->HitBottomRight = true;
					}

					if (Collision_IsHitLeftVtoS(CWall::Wall_GetCollision(i, c), m_PlayerColision))
					{
						player_judge->HitLeft = true;
						player_judge->HitTopLeft = true;
						player_judge->HitBottomLeft = true;
					}
				}

			}
		}
	}
}

void C3DObj::Collision_AnimeVSAnime(JUDGE *player_judge, Sphere *m_PlayerEnemyColision, JUDGE *enemy_judge, Sphere *m_EnemyMyColision)
{
	// プレイヤーとエネミーの当たり判定
	if (Collision_IsHitStoS(m_EnemyMyColision, m_PlayerEnemyColision))
	{
		player_judge->HitEnemy = true;
		enemy_judge->HitEnemy = true;
		if (Collision_IsHitTopStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitTop = true;
			player_judge->HitEnemyTop = true;
			enemy_judge->HitBottom = true;
			enemy_judge->HitEnemyBottom = true;
		}

		if (Collision_IsHitBottomStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitBottom = true;
			player_judge->HitEnemyBottom = true;
			enemy_judge->HitTop = true;
			enemy_judge->HitEnemyTop = true;
		}

		if (Collision_IsHitLeftStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitLeft = true;
			player_judge->HitEnemyLeft = true;
			enemy_judge->HitRight = true;
			enemy_judge->HitEnemyRight = true;
		}

		if (Collision_IsHitRightStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitRight = true;
			player_judge->HitEnemyRight = true;
			enemy_judge->HitLeft = true;
			enemy_judge->HitEnemyLeft = true;
		}

		// 斜め判定
		if (Collision_IsHitLeftTopStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitTopLeft = true;
			player_judge->HitEnemyTopLeft = true;
			enemy_judge->HitBottomRight = true;
			enemy_judge->HitEnemyBottomRight = true;
			player_judge->Hitnaname = true;
			enemy_judge->Hitnaname = true;
		}

		if (Collision_IsHitLeftBottomStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitBottomLeft = true;
			player_judge->HitEnemyBottomLeft = true;
			enemy_judge->HitTopRight = true;
			enemy_judge->HitEnemyTopRight = true;
			player_judge->Hitnaname = true;
			enemy_judge->Hitnaname = true;
		}

		if (Collision_IsHitRightTopStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitTopRight = true;
			player_judge->HitEnemyTopRight = true;
			enemy_judge->HitBottomLeft = true;
			enemy_judge->HitEnemyBottomLeft = true;
			player_judge->Hitnaname = true;
			enemy_judge->Hitnaname = true;
		}

		if (Collision_IsHitRightBottomStoS(m_EnemyMyColision, m_PlayerEnemyColision))
		{
			player_judge->HitBottomRight = true;
			player_judge->HitEnemyBottomRight = true;
			enemy_judge->HitTopLeft = true;
			enemy_judge->HitEnemyTopLeft = true;
			player_judge->Hitnaname = true;
			enemy_judge->Hitnaname = true;
		}
		/*HitBottom = true;
		HitRight = true;
		HitLeft = true;*/
	}
}


//=============================================================================
// モデル描画  アニメーション有
//=============================================================================
void C3DObj::DrawDX_Anime(D3DXMATRIX mtxWorld, int type, THING* pNomalModel)
{
	//static float fAnimTimeHold = fAnimTime;
	float fAnimTimeHold = fAnimTime;
	if (boPlayAnim)
	{
		fAnimTime += 0.01f;
	}

	SKIN_MESH::UpdateSphere(m_pD3DDevice, pNomalModel);

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//	m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	//	m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	//m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	//m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	D3DXMATRIXA16 mat;

	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルレンダリング
	SkinMesh.UpdateFrameMatrices(pNomalModel->pFrameRoot, &mtxWorld);
	SkinMesh.DrawFrame(m_pD3DDevice, pNomalModel->pFrameRoot, pNomalModel, true);
	pNomalModel->pAnimController->AdvanceTime(fAnimTime - fAnimTimeHold, NULL);

	mtxWorld *= mat;

	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	////////////////////////////////
	//アニメ再生時間を+
	/*	fAnimTimeHold = fAnimTime;
	if (boPlayAnim)
	{
	fAnimTime += 0.01f;
	}*/
}



//=============================================================================
// モデル描画  アニメーション無
//=============================================================================
void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, NormalModelData * pNomalModel)
{
	DWORD i;

	//m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	//m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	// マトリックスのセット
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルのレンダリング
	for (i = 0; i<pNomalModel->dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&pNomalModel->pMaterials[i]);
		m_pD3DDevice->SetTexture(0, pNomalModel->pTextures[i]);
		pNomalModel->pMesh->DrawSubset(i);
	}
}

//=============================================================================
// 当たり判定
//=============================================================================

THING* C3DObj::GetAnimeModel(void)
{
	return &Thing;
}

/*THING C3DObj::GetAnimeModel(int index)
{
return Thing_Anime[index];
}*/


/*NormalModelFile C3DObj::GetNormal(int index)
{
return NormalModelFile[index];
}*/
//=============================================================================
// アニメーション変更
//=============================================================================

void C3DObj::Animation_Change(int index, float speed)
{
	if (TrackDesc.Speed != speed)
	{
		TrackDesc.Speed = speed;//モーションスピード
								//Thing.pAnimController->SetTrackDesc(0, &TrackDesc);//アニメ情報セット
		Thing.pAnimController->SetTrackPosition(0, 0);
		Thing.pAnimController->SetTrackSpeed(0, TrackDesc.Speed);//アニメ情報セット
	}
	if (m_AnimationType != index)
	{
		Thing.pAnimController->SetTrackPosition(0, 0);
		Thing.pAnimController->SetTrackAnimationSet(0, pAnimSet[index]);
		m_AnimationType = index;
	}

}

bool C3DObj::VFCulling(D3DXVECTOR3* pPosition)
{
	//視界内：true 外：false 
	D3DXPLANE VFLeftPlane, VFRightPlane, VFTopPlane, VFBottomPlane;
	D3DXVECTOR3 Pos = *pPosition;
	float Radius = 0;
	D3DXMATRIX pmView = CCamera::Get_ViewMtx();
	float Angle = D3DX_PI / VF_ANGLE;
	float NearClip = 0.1f;//この二ついじっても大きさ変わる
	float FarClip = 80.0f;//
	float Aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
	//まず、ジオメトリの位置ベクトルをワールドからビュー空間に変換
	D3DXVec3TransformCoord(&Pos, &Pos, &pmView);

	//左右、上下の平面を計算
	{
		D3DXVECTOR3 p1, p2, p3;
		//左面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = -FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//右面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//上面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//下面
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = -FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}

	//6つの平面とジオメトリ境界球をチェック
	{
		//ニアクリップ面について
		if ((Pos.z + Radius) < NearClip)
		{
			return false;
		}
		//ファークリップ面について
		if ((Pos.z - Radius) > FarClip)
		{
			return false;
		}
		//左クリップ面について
		FLOAT Distance = (Pos.x * VFLeftPlane.a) + (Pos.z * VFLeftPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//右クリップ面について
		Distance = (Pos.x * VFRightPlane.a) + (Pos.z * VFRightPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//上クリップ面について
		Distance = (Pos.y * VFTopPlane.b) + (Pos.z * VFTopPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//下クリップ面について
		Distance = (Pos.y * VFBottomPlane.b) + (Pos.z * VFBottomPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
	}

	return true;
}