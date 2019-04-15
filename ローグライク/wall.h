#ifndef WALL_H_
#define WALL_H_
#include <d3dx9.h>
#include "collision.h"
#include "C2DObj.h"
//===================================================
// �萔
//===================================================
#define WALL_NOTCOLIISION (-100.0f)	// �����蔻�肶��Ȃ��z��
#define WALL_MAX (250)		// ���b�V���쐬�\��
#define WALL_WIDTH_MAX (200)

//===================================================
// ���b�V���E�H�[���\����
//===================================================
class CWall : public C2DObj
{
public:
	// 3D�p���_�\����
	typedef struct
	{
		D3DXVECTOR3 position;		// ���W
		D3DXVECTOR3 normal;			// �@��
		D3DCOLOR color;				// �J���[

		D3DXVECTOR2 texcoord;
	}Vertex3D;
	typedef struct
	{
		D3DXVECTOR3 pos;					// ���W
		D3DXVECTOR3 nor;					// �@��
		int tex;							// �e�N�X�`���[
		bool wuse;							// �g�p�m�F
		LPDIRECT3DVERTEXBUFFER9 pVBuffer;	// �o�[�e�b�N�X�o�b�t�@
		LPDIRECT3DINDEXBUFFER9 pIBuffer;	// �C���f�b�N�X�o�b�t�@
		int widthmax;						// ���̒��� 
		int heightmax;						// �c�̒���
		int widthcube;						// ���̎l�搔
		int heightcube;						// �c�̎l�p��

		int type;							// �����^�C�v
		Vertex col[WALL_WIDTH_MAX];			// �����蔻��
	}WALL;

	static void MeshWall_Init(void);
	// �e�N�X�`���ԍ��A���̒����A�c�̒����A���l�p���A�c�l�p��  ,�|�W�V����		�@��				
	static void MeshWall_Create(int tex, int width, int height, int wcube, int hcube, D3DXVECTOR3 pos, D3DXVECTOR3 normal);
	static void MeshWall_Finalize(void);
	static void MeshWall_Draw(void);

	static WALL Wall_GetData(int i) { return g_wall[i]; }

	static const Vertex* Wall_GetCollision(int i, int c) { return &g_wall[i].col[c];}
	static WALL g_wall[WALL_MAX];
};
#endif
