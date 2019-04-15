//////////////////////////////////////////////////
////
////	2D�I�u�W�F�N�g�N���X
////
//////////////////////////////////////////////////

#ifndef _2DOBJ_H_
#define _2DOBJ_H_

#include "CTexture.h"
#include <d3dx9.h>

class C2DObj : public CTexture
{
public:

	C2DObj();
	~C2DObj();

	void Update(void);						//	�X�V
	void Draw(void);						//	�`��
	void Finalize(void);					//	�I������
	static void Sprite_Initialize(void);	//	�X�v���C�g������
	static void Sprite_Finalize(void);		//	�X�v���C�g�j��

	void m_Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th);	//	�X�v���C�g�`��
	static void Sprite_Draw(int texture_index, float dx, float dy, int tx, int ty, int tw, int th);	//	�X�v���C�g�`��^�C�g�����U���g�p
	void Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, float cx, float cy, float sx, float sy, float rotation);
	static void SpriteDraw(int texture_index, float dx, float dy);
	/*	
		texture_index:�e�N�X�`���ԍ�
		dx:�\���������X�v���C�g�̍�����WX
		dy:�\���������X�v���C�g�̍�����WY
		tx:�\���������e�N�X�`���̎n�_X
		ty:�\���������e�N�X�`���̎n�_Y
		tw:�\���������e�N�X�`���̕�
		th:�\���������e�N�X�`���̍���
	*/
protected:

	//2D�|���S�����_�@�\����
	typedef struct Vertex2D_tag
	{
		D3DXVECTOR4 pos;	//	floot�ɂ���4��1.0f(RHW)�@���W�ϊ��ςݒ��_
		D3DCOLOR color;		//  �F
		D3DXVECTOR2 texcoord;	//�@�e�N�X�`��
	}Vertex2D;

	#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//	�\���̂̂������ɏ���
    //#define FVF_VERTEX2DUI (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)	//	�\���̂̂������ɏ���
	static LPDIRECT3DVERTEXBUFFER9 m_p2DVertexBuffer;		//	���_�o�b�t�@
	static LPDIRECT3DINDEXBUFFER9 m_p2DIndexBuffer;		//	�C���f�b�N�X�o�b�t�@

};

#endif // !1

