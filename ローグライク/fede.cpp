//=============================================================================
//	�C���N���[�h�t�@�C��
//=============================================================================

#include <d3dx9.h>
#include "common.h"

#include "CGameObj.h"

//=============================================================================
//	�O���[�o���ϐ��錾
//=============================================================================


typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
} FadeVertex;
#define FVF_FADE_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

//static D3DCOLOR g_FadeColor;
//	D3DXCOLOR�͂����ȊO�g��Ȃ�
static int g_r = 0;				//	��
static int g_g = 0;				//	��
static int g_b = 0;				//	��
static int g_Alpha = 0;			//	�A���t�@
static int g_AddAlpha = 1;		//	�A���t�@���Z�l

static bool g_bOut = false;		// true �t�F�[�h�A�E�g�@false �t�F�[�h�C��
static bool g_bIsFade = false;	// �t�F�[�h�C���A�E�g���Ă邩

//=============================================================================
//	����������
//=============================================================================

void Fade_Initialize(void)
{
	g_bIsFade = false;
	g_r = 0;
	g_g = 0;
	g_b = 0;
	g_Alpha = 0;
}

//=============================================================================
//	�I������
//=============================================================================

void Fade_Finalize(void)
{
	
}

//=============================================================================
//	�X�V����
//=============================================================================

void Fade_Update(void)
{
	if (!g_bIsFade)
	{
		return;
	}

	
	g_Alpha += g_AddAlpha;
	
	//	�t�F�[�h�C��
	if (g_bOut)
	{
		if (g_Alpha >= 255)
		{
			g_bIsFade = false;
			g_Alpha = 255;
		}
	}
	//	�t�F�[�h�A�E�g
	else
	{
		if (g_Alpha <= 0)
		{
			g_bIsFade = false;
			g_Alpha = 0;
		}
	}

}

//=============================================================================
//	�`�揈��
//=============================================================================

void Fade_Draw(void)
{
	if (g_Alpha <= 0)
	{
		return;
		// �����Ȃ��Ȃ�`���Ȃ�
	}

	
	FadeVertex v[] = {
		{ D3DXVECTOR4(0.0f		  ,0.0f		   ,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
		{ D3DXVECTOR4(WINDOW_WIDTH,0.0f		   ,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
		{ D3DXVECTOR4(0.0f		  ,WINDOW_HEIGHT,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
		{ D3DXVECTOR4(WINDOW_WIDTH,WINDOW_HEIGHT,0.0f,1.0f),D3DCOLOR_RGBA(g_r,g_g,g_b,g_Alpha) },
	};

	//LPDIRECT3DDEVICE9  pDevicce = mochi_GetDevice();

	CGameObj::m_pD3DDevice->SetFVF(FVF_FADE_VERTEX);
	CGameObj::m_pD3DDevice->SetTexture(0, NULL);
	CGameObj::m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

//=============================================================================
//	�֐���`
//=============================================================================

//	�t�F�[�h�X�^�[�g
void Fade_Start(bool bOut, int frame, int r, int g, int b)
{
	g_bOut = bOut;
	g_AddAlpha = frame;
	g_r = r;
	g_g = g;
	g_b = b;
	

	g_bIsFade = true;
	//	�t�F�[�h�C��
	if (g_bOut)
	{
		g_Alpha = 0;
	}
	//	�t�F�[�h�A�E�g
	else
	{
		g_Alpha = 255;
		g_AddAlpha = -g_AddAlpha;
	}
}

//	�t�F�[�h�`�F�b�N
bool Fade_IsFade(void)
{
	return g_bIsFade;
}