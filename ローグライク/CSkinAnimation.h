#pragma once
//�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//// �V���{����`�y�у}�N��
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define THING_AMOUNT 1//�`��A�j���[�V�������f����


//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME: public D3DXFRAME
{
    D3DXMATRIX CombinedTransformationMatrix;
};
//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;
    DWORD dwWeight;//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;//�{�[���̐�
    LPD3DXBUFFER pBoneBuffer;//�{�[���E�e�[�u��
    D3DXMATRIX** ppBoneMatrix;//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
    D3DXMATRIX* pBoneOffsetMatrices;//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
};
//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY(){}
    STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
    STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
    STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
    STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
	HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXANIMATIONCONTROLLER pMeshContainerBase);
};
// ���̍\����
struct SPHERE2
{
	D3DXVECTOR3 vCenter;
	FLOAT fRadius;
};

struct THING;
//
//
//
class SKIN_MESH
{
public:
	HRESULT InitThing(LPDIRECT3DDEVICE9,THING *,LPSTR);
	HRESULT InitThing2(LPDIRECT3DDEVICE9, THING *, LPSTR, MY_HIERARCHY);
	HRESULT AllocateBoneMatrix( THING* , LPD3DXMESHCONTAINER  );
	HRESULT AllocateAllBoneMatrices(THING* ,  LPD3DXFRAME  );
	VOID SKIN_MESH::RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame, THING* pThing, bool type);
	VOID UpdateFrameMatrices(LPD3DXFRAME , LPD3DXMATRIX );
	VOID SKIN_MESH::DrawFrame(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pFrameBase, THING* pThing, bool type);
	VOID ChangeAnim(DWORD NewAnimNum);
	void FreeAnim(LPD3DXFRAME pFrame);
	void Destroy(THING *DeleteAnimeModel);
	MY_HIERARCHY cHierarchy;
protected:
	

};

struct THING
{
	LPD3DXMESH pMesh;
	LPD3DXFRAME pFrameRoot;
	LPD3DXANIMATIONCONTROLLER pAnimController;
	D3DMATERIAL9* pMeshMaterials;
	LPDIRECT3DTEXTURE9* pMeshTextures;
	DWORD dwNumMaterials;
	D3DXVECTOR3 vPosition;
	D3DXMATRIX mRotation;
	D3DXMATRIX mWorld;
	LPDIRECT3DTEXTURE9* texture;

	THING()
	{
		ZeroMemory(this, sizeof(THING));
	}
};
