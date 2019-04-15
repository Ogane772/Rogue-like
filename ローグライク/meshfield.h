#ifndef MESHFIELD_H_
#define MESHFIELD_H_
#include <d3dx9.h>
#define MESH_MAX (20)		// ���b�V���쐬�\��
#include "C2DObj.h"

class CMeshField : public C2DObj
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

	// ���b�V���\����
	typedef struct
	{
		D3DXVECTOR3 pos;					// ���W
		D3DXVECTOR3 nor;					// �@��
		int tex;							// �e�N�X�`���[
		bool muse;							// �g�p�m�F
		LPDIRECT3DVERTEXBUFFER9 pVBuffer;	// �o�[�e�b�N�X�o�b�t�@
		LPDIRECT3DINDEXBUFFER9 pIBuffer;	// �C���f�b�N�X�o�b�t�@
		int widthmax;						// ���̒��� 
		int heightmax;						// �c�̒���
		int widthcube;						// ���̎l�搔
		int heightcube;						// �c�̎l�p��
	}MESH;

	static void MeshField_Init(void);
	// �e�N�X�`���ԍ��A���̒����A�c�̒����A���l�p���A�c�l�p��  ,�|�W�V����
	static void MeshField_Create(int tex, int width, int height, int wcube, int hcube, D3DXVECTOR3 pos);
	static void MeshField_Finalize(void);
	static void MeshField_Draw(void);
private:
	static MESH g_mesh[MESH_MAX];
	static D3DXVECTOR3 Rpos;

	static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;
	static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer;

	static int gTexture;
};

#endif
