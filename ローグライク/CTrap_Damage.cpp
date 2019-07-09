#include "CTrap.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CTrap_Damage.h"
#include "C3DObj.h"
#include "debug_font.h"
//===================================
// �O���[�o���ϐ�
//===================================
//=============================================================================
//	����
//=============================================================================

CTrapDamage ::CTrapDamage (int x, int z, int trap_type) :CTrap(trap_type), C3DObj(C3DObj::TYPE_TRAP)
{
	Initialize(x, z, trap_type);
}

//=============================================================================
//	�j��
//=============================================================================

CTrapDamage ::~CTrapDamage ()
{

}

void CTrapDamage ::Initialize(int x, int z, int trap_type)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	m_Type = trap_type;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
	m_WanaCheck = false;
	m_WanaEffectOn = false;
}

void CTrapDamage ::Finalize(void)
{

}

void CTrapDamage ::Update(void)
{

}

void CTrapDamage ::Draw(void)
{
	// ���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	if (alive && m_WanaCheck)
	{
		D3DXMatrixIdentity(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslotation, m_Position.x, m_Position.y, m_Position.z);
		CBilboard::BilBoard_ChangeSizeColor(5.0f, 4.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
		CBilboard::BilBoard3_Draw(CTexture::TEX_TRAP_DAMAGE, m_Position);
	}
}

