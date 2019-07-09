#include "CObject.h"
#include "bilboard.h"
#include "CTexture.h"
#include "CObject_Kaki.h"
#include "C3DObj.h"
#include "debug_font.h"
//===================================
// �O���[�o���ϐ�
//===================================
//=============================================================================
//	����
//=============================================================================

CObjectKaki::CObjectKaki(int x, int z, int object_type) :CObject(object_type), C3DObj(C3DObj::TYPE_OBJECT)
{
	Initialize(x, z, object_type);
}

//=============================================================================
//	�j��
//=============================================================================

CObjectKaki::~CObjectKaki()
{

}

void CObjectKaki::Initialize(int x, int z, int object_type)
{
	alive = true;
	m_Mapz = z;
	m_Mapx = x;
	type = object_type;
	m_Position = D3DXVECTOR3(-247.5f + x * 5 - 2.5f, 0.0f, 247.5f - z * 5 - 1.0f);
	map_drawflag = false;
}

void CObjectKaki::Finalize(void)
{

}

void CObjectKaki::Update(void)
{

}

void CObjectKaki::Draw(void)
{
	// ���[���h���W�ϊ��s��̍쐬�Ɛݒ�
	D3DXMATRIX mtxWorld;
	D3DXMATRIX mtxRotation;
	D3DXMATRIX mtxTranslotation;
	if (alive)
	{
		D3DXMatrixIdentity(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslotation, m_Position.x, m_Position.y, m_Position.z);
		CBilboard::BilBoard_ChangeSizeColor(5.0f, 4.2f, D3DCOLOR_RGBA(255, 255, 255, 255));
		CBilboard::BilBoard3_Draw(CTexture::TEX_KAKI_ICON, m_Position);
	}
}
