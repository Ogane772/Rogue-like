#ifndef GRID_H_
#define GRID_H_
#include <d3dx9.h>
#include "CGameObj.h"

#define GRID_MAX (12)

class CGrid : public CGameObj
{
public:
	

	// 3D�p���_�\����
	typedef struct
	{
		D3DXVECTOR3 position;		// ���W
		D3DXVECTOR3 normal;			// �@��
		D3DCOLOR color;				// �J���[
	}Vertex3d;

	

	static void Init();
	// �e�N�X�`���ԍ��A���̒����A�c�̒����A���l�p���A�c�l�p��  ,�|�W�V����
	static void Finalize();
	static void Update(int z, int x);
	static void Draw();
	static void Grid_Draw();
private:
	static Vertex3d gLine[GRID_MAX + GRID_MAX];

	};

#endif
