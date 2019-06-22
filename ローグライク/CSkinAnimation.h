#pragma once
//必要なヘッダーファイルのインクルード
#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//// シンボル定義及びマクロ
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define THING_AMOUNT 1//描画アニメーションモデル数


//派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
struct MYFRAME: public D3DXFRAME
{
    D3DXMATRIX CombinedTransformationMatrix;
};
//派生メッシュコンテナー構造体(
//コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
    LPDIRECT3DTEXTURE9*  ppTextures;
    DWORD dwWeight;//重みの個数（重みとは頂点への影響。）
	DWORD dwBoneNum;//ボーンの数
    LPD3DXBUFFER pBoneBuffer;//ボーン・テーブル
    D3DXMATRIX** ppBoneMatrix;//全てのボーンのワールド行列の先頭ポインター
    D3DXMATRIX* pBoneOffsetMatrices;//フレームとしてのボーンのワールド行列のポインター
};
//Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
//ID3DXAllocateHierarchyは派生すること想定して設計されている。
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
// 物体構造体
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
