//////////////////////////////////////////////////
////
////	�Q�[���I�u�W�F�N�g�N���X
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
		TYPE_LIGHT,  // ���C�g
		TYPE_CAMERA, //	�J����
		TYPE_MESH,	 // ���b�V���t�B�[���h
		TYPE_UI,	 //	UI

		TYPE_MAX

	} OBJTYPE;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Finalize(void) = 0;
	bool Get_Enable(void) { return m_Enable; }

	static void FrameCountReset(void);
	static int Get_FraemCount(void) { return m_FrameCount; }	//	�t���[���J�E���g�擾
	static int Get_GameObjNum(void) { return m_GameObjNum; }	//	���Q�[���I�u�W�F�N�g�擾
	static int Get_GameObjIndex(void) { return m_GameObjNum - 1; }	//	�Q�[���I�u�W�F�N�g�C���f�b�N�X

	static void FrameCountUp(void) { m_FrameCount++; }	//  �t���[���J�E���g�A�b�v
	static bool Device_Initialize(HWND hWnd);	//	�f�o�C�X������
	static void Device_Finalize(void);	//	�f�o�C�X�j��
	static LPDIRECT3DDEVICE9 m_pD3DDevice;	//	�f�o�C�X


	static void UpdateAll();	// �S�I�u�W�F�N�g�X�V
	static void DrawAll();		// �S�I�u�W�F�N�g�`��
	static void DeleteAll2D();	// �S�I�u�W�F�N�g�폜
	static CGameObj *Get(int type);				// �C���X�^���X�擾
												/*	static CGameObj *Get(int nIdx);				// �C���X�^���X�擾
												int Get_GameObjIndex() { return m_GameObjIndex;}	// ���[�N�C���f�b�N�X�擾*/
	int Get_GameType(void) { return m_GameObjType; }		// ��ގ擾
	int Get_GameIndex(void) { return m_GameObjIndex; }
	static void DebugDraw(void);
protected:

	static int m_FrameCount;	//	�t���[���J�E���g�@
	static int m_GameObjNum;	//	�Q�[���I�u�W�F�N�g����
	int m_TimeKeep = 0;
	bool m_Enable;
	static bool g_bIsFade;				//�t�F�[�h�t���O
private:

	int m_GameObjIndex;
	int m_GameObjType;
	static CGameObj *pGameObj[MAX_GAMEOBJ];
	static LPDIRECT3D9 m_pD3D;

};

#endif // !1


