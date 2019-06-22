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
�O���[�o���ϐ�
======================================================================*/
//static int g_TextureIndex = TEXTURE_INVALID_INDEX;
static bool g_bIsFade;
static int tFlag = 0;
C2DObj *pGallery;
static int cursor = 0;
static bool tipsflag = false;//�f�[�^�W�J���Ă�Ƃ�true
static bool galleryflag = false;//�M�������[�J���Ă�Ƃ�true
C3DObj::NormalModelData CGallery::Gallery_model[GALLERY_MODELMAX];

static D3DXMATRIX mtxWorld;			//	���[���h�ϊ��p�s��
static D3DXMATRIX mtxTranslation;	//	�ړ��s��
static D3DXMATRIX mtxRotation;		//	�ړ��s��
static D3DXMATRIX mtxScaling;		//	�ړ��s��
static D3DXVECTOR3 Position;
static D3DXVECTOR3 Rotation;			//  �p�x
static D3DXVECTOR3 Scaling;

static D3DXVECTOR3 m_At;		//	���Ă�ꏊ	�����_
static float m_AtLength = 0.0; // at�܂ł̒���
static D3DXVECTOR3 Front;		//	�t�����g�x�N�g��
static D3DXVECTOR3 Right;	//	���C�g�x�N�g��
static D3DXVECTOR3 Up;			//	�A�b�v�x�N�g��

static D3DXMATRIX m_mtxView;	//	�r���[�ϊ��p�s��
static D3DXVECTOR3 CameraPos;	//	�J�������W


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
	// �O��
	//D3DXVec3Cross(&g_right, &g_front, &g_up);
	// �t����
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
		// �t�F�[�h���I�������
		if (!Fade_IsFade())
		{
			g_bIsFade = false;
			Scene_Change(SCENE_INDEX_TITLEMENU);
		}
	}
	//�M�������[���J���Ă�Ƃ����f������]������
	if (galleryflag)
	{
		Rotation = D3DXVECTOR3(0.0f, 0.01f, 0.0f);
		D3DXMatrixRotationY(&mtxRotation, Rotation.y);
		mtxWorld *=  mtxRotation;
	}
	else//�M�������[������Ƃ����f���̉�]������������
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
	//���̃E�B���h�E�`��
	pGallery->m_Sprite_Draw(CTexture::TEX_BLACK, 0, 0, 0, 0, pGallery->Texture_GetWidth(CTexture::TEX_BLACK), pGallery->Texture_GetHeight(CTexture::TEX_BLACK));
	pGallery->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 50.0f, 50.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 1.7f, 0.58f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(105, 75, D3DCOLOR_RGBA(255, 255, 255, 255), "�`���̋L��");
	CUserinterface::UI_TextDraw(105, 125, D3DCOLOR_RGBA(255, 255, 255, 255), "����");
	CUserinterface::UI_TextDraw(105, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "�߂�");

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
	//�J����
	// �J�����̐U������@��]�s��
	D3DXMATRIX mtxR;        // 1�t���[���ɉ�]����p�x
							//g_Came_at = (g_front * g_length) + g_Cposition;
							// �U�����ȊO�̓J�������v���C���[�ɒǏ]

							// �r���[���W�ϊ��s��̍쐬�Ɛݒ�
							// �J�������W
	D3DXVECTOR3 eye(5.0f, 5.0f, -10.0f);
	// ���Ă�ꏊ(�����_)
	D3DXVECTOR3 at(0.0f, 0.0f, 0.0f);
	// �J�����̏�����x�N�g��
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);


	//g_front = D3DXVECTOR3(0, -0.1f, 1.0f);
	//D3DXVec3TransformNormal(&g_front, &g_front);

	D3DXMatrixLookAtLH(&m_mtxView, &CameraPos, &m_At, &up);

	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// �v���W�F�N�V�������W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxProjection;				// ����p120���ɂ�����								near�ɂ�0�ȉ��֎~ �ǂݍ��ދ���
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 25.0f);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

}

void CGallery::Gallery_ModelDraw(void)
{
	unsigned int i;
	
	// �}�g���b�N�X�̃Z�b�g
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//���f���̃����_�����O
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
	CUserinterface::UI_TextDraw(355, 75, D3DCOLOR_RGBA(255, 255, 255, 255), "�`���� %d", save->danjyon_in);
	CUserinterface::UI_TextDraw(355, 125, D3DCOLOR_RGBA(255, 255, 255, 255), "���v�˔j�K�w�� %d", save->floor_count);
	CUserinterface::UI_TextDraw(355, 175, D3DCOLOR_RGBA(255, 255, 255, 255), "�Q�[���N���A�� %d", save->game_clear);
	CUserinterface::UI_TextDraw(355, 225, D3DCOLOR_RGBA(255, 255, 255, 255), "���������� %d", save->walk_count);
	CUserinterface::UI_TextDraw(355, 275, D3DCOLOR_RGBA(255, 255, 255, 255), "�A�C�e�����g�p������ %d", save->item_count);
	CUserinterface::UI_TextDraw(355, 325, D3DCOLOR_RGBA(255, 255, 255, 255), "���ꂽ�� %d",save->death_count);
	CUserinterface::UI_TextDraw(355, 375, D3DCOLOR_RGBA(255, 255, 255, 255), "���v�^�_���[�W %d",save->damage_count);
	CUserinterface::UI_TextDraw(355, 425, D3DCOLOR_RGBA(255, 255, 255, 255), "���v��_���[�W %d",save->damage_on_count);
	CUserinterface::UI_TextDraw(355, 475, D3DCOLOR_RGBA(255, 255, 255, 255), "�|�����G�̐� %d",save->enemy_death_count);
	CUserinterface::UI_TextDraw(355, 525, D3DCOLOR_RGBA(255, 255, 255, 255), "�����؂�ւ����� %d", save->wepon_chenge_count);
	CUserinterface::UI_TextDraw(355, 575, D3DCOLOR_RGBA(255, 255, 255, 255), "   �߂�");
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 355.0f, 575.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);
}

void CGallery::Gallery_JIXTUSEKI_Draw(void)
{
	pGallery->Sprite_Draw(CTexture::TEX_WIDTH_WINDOW, 325.0f + JIXTUSEKI_ADDX, 450.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_WIDTH_WINDOW), (float)CTexture::Texture_GetHeight(CTexture::TEX_WIDTH_WINDOW), 0.0f, 0.0f, 5.4f, 0.5f, D3DXToRadian(0.0f));
	CUserinterface::UI_TextDraw(395 + JIXTUSEKI_ADDX, 465, D3DCOLOR_RGBA(255, 255, 255, 255), "���̗E�҂̑��E��");
	CUserinterface::UI_TextDraw(395 + JIXTUSEKI_ADDX, 515, D3DCOLOR_RGBA(255, 255, 255, 255), "�჉���N�̌��ŃN���A�������̂ɑ����铺��");
	CUserinterface::UI_TextDraw(395 + JIXTUSEKI_ADDX, 565, D3DCOLOR_RGBA(255, 255, 255, 255), "   �߂�");
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 395.0f + JIXTUSEKI_ADDX, 565.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.19f, 0.19f, 0.0f);

	//���E�̃J�[�\��
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 925.0f - 70, 200.0f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.3f, 0.3f, 0.0f);
	pGallery->Sprite_Draw(CTexture::TEX_CURSOL, 475.0f - 70, 256.3f, 0.0f, 0.0f, (float)CTexture::Texture_GetWidth(CTexture::TEX_CURSOL), (float)CTexture::Texture_GetHeight(CTexture::TEX_CURSOL), 0.0f, 0.0f, 0.3f, 0.3f, D3DXToRadian(180.0f));
}