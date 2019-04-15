//////////////////////////////////////////////////
////
////	3D�I�u�W�F�N�g�N���X
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
#define VF_ANGLE (1.0f)//�J�����O�̑�̂̑傫��
#define ADD_HP (3)//HP�񕜗�
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//=============================================================================
//	�ÓI�ϐ�
//===========================================================================
C3DObj *C3DObj::p3DObj[MAX_GAMEOBJ];
int C3DObj::m_3DObjNum = 0;

C3DObj::MaterialFileData C3DObj::NORMAL_MODEL_FILES[] = {
	{ "asset/model/sraim1_10.x" },
	{ "asset/model/sraimv3.x" },
};
//	�g�������A�j�����f���̐���������
C3DObj::MaterialFileData2 C3DObj::ANIME_MODEL_FILES[] = {
	{ "asset/anime_model/hewplayer_judge->x" },
	{ "asset/anime_model/small_enemy.x" },
	{ "asset/anime_model/middle_enemy.x" },
	{ "asset/anime_model/special_enemy.x" },
	{ "asset/anime_model/big_enemy.x" },
};
int C3DObj::MODEL_FILES_MAX = sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0]);
int C3DObj::ANIME_MODEL_FILES_MAX = sizeof(C3DObj::ANIME_MODEL_FILES) / sizeof(ANIME_MODEL_FILES[0]);

C3DObj::NormalModelData C3DObj::NormalModel[(sizeof(C3DObj::NORMAL_MODEL_FILES) / sizeof(NORMAL_MODEL_FILES[0]))];//�ǂݍ��ރ��f���̍ő吔+1


//=============================================================================
//	����
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
	// ���[�N�i�[���s
	if (m_3DObjIndex >= MAX_GAMEOBJ)
	{
		m_3DObjIndex = -1;
	}


}

C3DObj::C3DObj(int type)
{
	// ���[�N�Ɏ������g(this)���i�[
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
	// ���[�N�i�[���s
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
//	�j��
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
		// �|�����[�t�B�Y���ɂ���Ĕh���N���X��Update()���Ă΂��
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
// �S�I�u�W�F�N�g�X�V
//=============================================================================
void C3DObj::UpdateAll()
{
	int i;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// �|�����[�t�B�Y���ɂ���Ĕh���N���X��Update()���Ă΂��
		if (p3DObj[i])
		{
			p3DObj[i]->Update();
		}
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�`��
//=============================================================================
void C3DObj::DrawAll()
{
	int i;
	int dc = 0;
	for (i = 0; i < MAX_GAMEOBJ; i++)
	{
		// �|�����[�t�B�Y���ɂ���Ĕh���N���X��Draw()���Ă΂��
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
					//��ʊO�̎��̓e�N�X�`�����X�V
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
					//��ʊO�̎��̓e�N�X�`�����X�V
					p3DObj[i]->SkinMesh.DrawFrame(m_pD3DDevice, p3DObj[i]->Thing.pFrameRoot, &p3DObj[i]->Thing, false);
				}
			}
		}
	}
}



//=============================================================================
// �S�I�u�W�F�N�g�폜
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
// �C���X�^���X�擾
//=============================================================================
C3DObj *C3DObj::Get(int nIdx)
{
	// �C���f�b�N�X���͈͊O
	if (p3DObj[nIdx])
	{
		return p3DObj[nIdx];
	}
	return NULL;
}


//=============================================================================
// �I�u�W�F�N�g�폜
//=============================================================================
void C3DObj::C3DObj_delete(void)
{
	C3DObj *pobj = Get(Get_3DObjIndex());
	delete pobj;
}



//=============================================================================
// ���f���ǂݍ���
//=============================================================================
// Geometry�̏������i���f���̓ǂݍ��݁j
HRESULT C3DObj::InitModelLoad()
{
	int i;
	//�ʏ탂�f���ǂݍ���	
	for (i = 0; i < MODEL_FILES_MAX; i++)
	{
		InitNormalModelLoad(&NormalModel[i], NORMAL_MODEL_FILES[i].filename);
	}

	return S_OK;
}

HRESULT C3DObj::InitNormalModelLoad(NormalModelData *pNomalModel, LPSTR szXFileName)
{
	/////////
	// X�t�@�C�����烁�b�V�������[�h����	
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	if (FAILED(D3DXLoadMeshFromX(szXFileName, D3DXMESH_SYSTEMMEM,
		m_pD3DDevice, NULL, &pD3DXMtrlBuffer, NULL,
		&pNomalModel->dwNumMaterials, &pNomalModel->pMesh)))
	{
		MessageBox(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", szXFileName, MB_OK);
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
				MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
			}
		}
	}
	pD3DXMtrlBuffer->Release();

	return S_OK;
}

//=============================================================================
// ���f���j��
//=============================================================================
void C3DObj::Model_Finalize(void)	//	���f���f�[�^�̊J���@������������S���������ǂꂩ������
{

}

void C3DObj::NormalModel_Finalize(NormalModelData *DeleteNormalModel)	//	���f���f�[�^�̊J��
{
	for (int i = 0; i < DeleteNormalModel->dwNumMaterials; i++)
	{
		//�I�u�W�F�N�g�̃R�s�[�����Ă��邹���ŃG���[���o��̂Œ��ڃ��f����ǂނ���
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
				// �v���C���[�ƕǂ̓����蔻��
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
	// �v���C���[�ƃG�l�~�[�̓����蔻��
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

		// �΂ߔ���
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
// ���f���`��  �A�j���[�V�����L
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

	//���f�������_�����O
	SkinMesh.UpdateFrameMatrices(pNomalModel->pFrameRoot, &mtxWorld);
	SkinMesh.DrawFrame(m_pD3DDevice, pNomalModel->pFrameRoot, pNomalModel, true);
	pNomalModel->pAnimController->AdvanceTime(fAnimTime - fAnimTimeHold, NULL);

	mtxWorld *= mat;

	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	////////////////////////////////
	//�A�j���Đ����Ԃ�+
	/*	fAnimTimeHold = fAnimTime;
	if (boPlayAnim)
	{
	fAnimTime += 0.01f;
	}*/
}



//=============================================================================
// ���f���`��  �A�j���[�V������
//=============================================================================
void C3DObj::DrawDX_Normal(D3DXMATRIX mtxWorld, NormalModelData * pNomalModel)
{
	DWORD i;

	//m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	//m_pD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	//m_pD3DDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);


	// �}�g���b�N�X�̃Z�b�g
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//���f���̃����_�����O
	for (i = 0; i<pNomalModel->dwNumMaterials; i++)
	{
		m_pD3DDevice->SetMaterial(&pNomalModel->pMaterials[i]);
		m_pD3DDevice->SetTexture(0, pNomalModel->pTextures[i]);
		pNomalModel->pMesh->DrawSubset(i);
	}
}

//=============================================================================
// �����蔻��
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
// �A�j���[�V�����ύX
//=============================================================================

void C3DObj::Animation_Change(int index, float speed)
{
	if (TrackDesc.Speed != speed)
	{
		TrackDesc.Speed = speed;//���[�V�����X�s�[�h
								//Thing.pAnimController->SetTrackDesc(0, &TrackDesc);//�A�j�����Z�b�g
		Thing.pAnimController->SetTrackPosition(0, 0);
		Thing.pAnimController->SetTrackSpeed(0, TrackDesc.Speed);//�A�j�����Z�b�g
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
	//���E���Ftrue �O�Ffalse 
	D3DXPLANE VFLeftPlane, VFRightPlane, VFTopPlane, VFBottomPlane;
	D3DXVECTOR3 Pos = *pPosition;
	float Radius = 0;
	D3DXMATRIX pmView = CCamera::Get_ViewMtx();
	float Angle = D3DX_PI / VF_ANGLE;
	float NearClip = 0.1f;//���̓�������Ă��傫���ς��
	float FarClip = 80.0f;//
	float Aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
	//�܂��A�W�I���g���̈ʒu�x�N�g�������[���h����r���[��Ԃɕϊ�
	D3DXVec3TransformCoord(&Pos, &Pos, &pmView);

	//���E�A�㉺�̕��ʂ��v�Z
	{
		D3DXVECTOR3 p1, p2, p3;
		//����
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = -FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFLeftPlane, &p1, &p2, &p3);
		//�E��
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = p2.x;
		p3.y = -p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFRightPlane, &p1, &p2, &p3);
		//���
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = -FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFTopPlane, &p1, &p2, &p3);
		//����
		p1 = D3DXVECTOR3(0, 0, 0);
		p2.x = FarClip * ((FLOAT)tan(Angle / 2)*Aspect);
		p2.y = -FarClip * (FLOAT)tan(Angle / 2);
		p2.z = FarClip;
		p3.x = -p2.x;
		p3.y = p2.y;
		p3.z = p2.z;
		D3DXPlaneFromPoints(&VFBottomPlane, &p1, &p2, &p3);
	}

	//6�̕��ʂƃW�I���g�����E�����`�F�b�N
	{
		//�j�A�N���b�v�ʂɂ���
		if ((Pos.z + Radius) < NearClip)
		{
			return false;
		}
		//�t�@�[�N���b�v�ʂɂ���
		if ((Pos.z - Radius) > FarClip)
		{
			return false;
		}
		//���N���b�v�ʂɂ���
		FLOAT Distance = (Pos.x * VFLeftPlane.a) + (Pos.z * VFLeftPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//�E�N���b�v�ʂɂ���
		Distance = (Pos.x * VFRightPlane.a) + (Pos.z * VFRightPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//��N���b�v�ʂɂ���
		Distance = (Pos.y * VFTopPlane.b) + (Pos.z * VFTopPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
		//���N���b�v�ʂɂ���
		Distance = (Pos.y * VFBottomPlane.b) + (Pos.z * VFBottomPlane.c);
		if (Distance>Radius)
		{
			return false;
		}
	}

	return true;
}