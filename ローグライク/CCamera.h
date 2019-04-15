//////////////////////////////////////////////////
////
////	�J�����N���X
////
//////////////////////////////////////////////////

#ifndef _CCAMERA_H_
#define _CCAMERA_H_

#include "CGameObj.h"
#define NYAA (0.1f)
#define FUAA (150.0f)

class CCamera :public CGameObj
{
public:
	CCamera();
	~CCamera();

	static D3DXVECTOR3 m_CameraPos;	//	�J�������W

	void Update(void);
	void Draw(void);
	void Finalize(void);		//	�I������
	static D3DXVECTOR3 Get_RightVec(void) { return m_Right; }	//	���C�g�x�N�g���擾	
	static const D3DXMATRIX Get_ViewMtx(void) { return m_mtxView; }		//	�r���[�}�g���N�X�擾
	static CCamera* Camera_Create(void);	//	�J�����N���X�쐬
											//	�f�o�b�O�p
	void DebugDraw(void);
	static CCamera* Get_CCamera(void) { return m_pCamera; }
	static D3DXVECTOR3 Camera_GetData(void) { return m_CameraPos; }
	static void Dalete_Camera(void);
protected:


private:
	void Camera_Initialize(void);	//	������

	
	D3DXVECTOR3 m_At;		//	���Ă�ꏊ	�����_
	float m_AtLength = 0.0; // at�܂ł̒���
	D3DXVECTOR3 m_Front;		//	�t�����g�x�N�g��
	static D3DXVECTOR3 m_Right;	//	���C�g�x�N�g��
	D3DXVECTOR3 m_Up;			//	�A�b�v�x�N�g��

	static D3DXMATRIX m_mtxView;	//	�r���[�ϊ��p�s��

	static CCamera *m_pCamera;

	bool view;		// �r���[���[�h�t���O
};

#endif // !1
