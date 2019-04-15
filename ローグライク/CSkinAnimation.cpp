#pragma warning(disable:4996)
#include "CSkinAnimation.h"
#include <crtdbg.h>
#include "CGameObj.h"
#define _CRTDBG_MAP_ALLOC

#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//MY_HIERARCHY SKIN_MESH::cHierarchy;
//
//フレームを作成する
HRESULT MY_HIERARCHY::CreateFrame(LPCTSTR Name, LPD3DXFRAME *ppNewFrame)
{
    HRESULT hr = S_OK;
    MYFRAME *pFrame;

	*ppNewFrame = NULL;

    pFrame = new MYFRAME;
    if (pFrame == NULL)
    {
		return E_OUTOFMEMORY;    
    }
	pFrame->Name=new TCHAR[lstrlen(Name) + 1];	
    if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy(pFrame->Name,Name);

    D3DXMatrixIdentity(&pFrame->TransformationMatrix);
    D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
    pFrame->pMeshContainer = NULL;
    pFrame->pFrameSibling = NULL;
    pFrame->pFrameFirstChild = NULL;
    *ppNewFrame = pFrame;
	
    return S_OK;
}
//
//メッシュコンテナーを作成する
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
                            CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
							DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppMeshContainer) 
{

    MYMESHCONTAINER *pMeshContainer = NULL;
    int iFacesAmount;
    int iMaterial;
    LPDIRECT3DDEVICE9 pDevice = NULL;
    LPD3DXMESH pMesh = NULL;
    *ppMeshContainer = NULL;
	DWORD dwBoneNum=0;
	DWORD i;
    pMesh = pMeshData->pMesh;
    pMeshContainer = new MYMESHCONTAINER;
    if (pMeshContainer == NULL)
    {
        return E_OUTOFMEMORY;
    }
    ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));

	pMeshContainer->Name=new TCHAR[lstrlen(Name) + 1];	
    if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	strcpy(pMeshContainer->Name,Name);
    //pMesh->GetDevice(&pDevice);
	pMesh->GetDevice(&pDevice);
    iFacesAmount = pMesh->GetNumFaces();  
   
	pMeshContainer->MeshData.pMesh = pMesh;   
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;	

	if (pSkinInfo == NULL)
	{
		pMesh->AddRef();//通常メッシュの場合はこれが必要。スキンの場合、これをするとなぜかメモリリークになる。
	} 
	//メッシュのマテリアル設定
    pMeshContainer->NumMaterials = max(1, NumMaterials);
    pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
    pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
    pMeshContainer->pAdjacency = new DWORD[iFacesAmount*3];
    if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
    {
        return E_FAIL;
    }
    memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount*3);
    memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

    if (NumMaterials > 0)            
    {
        memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

        for (iMaterial = 0; iMaterial < (signed)NumMaterials; iMaterial++)
        {
            if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
            {
                TCHAR strTexturePath[MAX_PATH];
                strcpy( strTexturePath,  pMeshContainer->pMaterials[iMaterial].pTextureFilename );
                if( FAILED( D3DXCreateTextureFromFile( pDevice, strTexturePath, 
                                                        &pMeshContainer->ppTextures[iMaterial] ) ) )
                    pMeshContainer->ppTextures[iMaterial] = NULL;
                pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
            }
        }
    }
    else
    {
        pMeshContainer->pMaterials[0].pTextureFilename = NULL;
        memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
        pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
    }
	//当該メッシュがスキン情報を持っている場合（スキンメッシュ固有の処理）
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
        pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for (i= 0; i < dwBoneNum; i++)
		{	
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i),sizeof(D3DMATRIX));	
		}
		if(FAILED(pMeshContainer->pSkinInfo->ConvertToBlendedMesh(
			pMesh,                                       
			NULL,	pMeshContainer->pAdjacency,NULL, NULL, NULL,
			&pMeshContainer->dwWeight,
			&pMeshContainer->dwBoneNum,
			&pMeshContainer->pBoneBuffer,
			&pMeshContainer->MeshData.pMesh)
			))
		{
			return E_FAIL;
		}
	}
	//ローカルに生成したメッシュコンテナーを呼び出し側にコピーする （コピーじゃないけど・・・）
	*ppMeshContainer = pMeshContainer;
	//参照カウンタを増やしたので減らす
	SAFE_RELEASE(pDevice);

    return S_OK;
}
//
//フレームを破棄する
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{

    //SAFE_DELETE_ARRAY( pFrameToFree->Name );
	if (pFrameToFree->Name)
		delete pFrameToFree->Name;
	if (pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);
	if(pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if(pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	SAFE_DELETE( pFrameToFree );
	//delete pFrameToFree;

    return S_OK; 
}

//
//メッシュコンテナーを破棄する
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
    int iMaterial;
    MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

    SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
    SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
    SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );

	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrix );
	
    if (pMeshContainer->ppTextures != NULL)
    {
        for (iMaterial = 0; iMaterial < (signed)pMeshContainer->NumMaterials; iMaterial++)
        {
            SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
        }
    }
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

    SAFE_RELEASE( pMeshContainer->MeshData.pMesh );

	if(pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE( pMeshContainer->pBoneBuffer );	
		SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );
	}

    SAFE_DELETE( pMeshContainer );

    return S_OK;
}

HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXANIMATIONCONTROLLER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < (signed)pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);

	return S_OK;
}
//
//
HRESULT SKIN_MESH::AllocateBoneMatrix( THING* pThing,LPD3DXMESHCONTAINER pMeshContainerBase )
{
    MYFRAME *pFrame=NULL;
	DWORD dwBoneNum=0;
	DWORD i;
    MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
    if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
    pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

    for (i=0; i<dwBoneNum; i++)
    {
		pFrame = (MYFRAME*)D3DXFrameFind( pThing->pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i) );
        if (pFrame == NULL)
		{
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;
 
	}    
    return S_OK;
}
//
//
HRESULT SKIN_MESH::AllocateAllBoneMatrices( THING* pThing,LPD3DXFRAME pFrame )
{
    if (pFrame->pMeshContainer != NULL)
    {
        if(FAILED(AllocateBoneMatrix(pThing,pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
    }
    if (pFrame->pFrameSibling != NULL)
    {
        if(FAILED(AllocateAllBoneMatrices(pThing,pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
    }
    if (pFrame->pFrameFirstChild != NULL)
    {
        if(FAILED(AllocateAllBoneMatrices(pThing,pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
    }
    return S_OK;
}
//
HRESULT SKIN_MESH::InitSphere(LPDIRECT3DDEVICE9 pDevice, THING* pThing)
{
	// メッシュの外接円の中心と半径を計算する
	/*D3DXFrameCalculateBoundingSphere(pThing->pFrameRoot, &pThing->Sphere.vCenter,
		&pThing->Sphere.fRadius);*/
	// 得られた中心と半径を基にメッシュとしてのスフィアを作成する
	D3DXCreateSphere(pDevice, pThing->Sphere.fRadius, 5, 5, &pThing->pSphereMesh, NULL);
	//スフィアメッシュのマテリアル　白色、半透明、光沢強
	pThing->pSphereMeshMaterials = new D3DMATERIAL9;
	pThing->pSphereMeshMaterials->Diffuse.r = 1.0f;
	pThing->pSphereMeshMaterials->Diffuse.g = 1.0f;
	pThing->pSphereMeshMaterials->Diffuse.b = 1.0f;
	pThing->pSphereMeshMaterials->Diffuse.a = 0.3f;
	pThing->pSphereMeshMaterials->Ambient = pThing->pSphereMeshMaterials->Diffuse;
	pThing->pSphereMeshMaterials->Specular.r = 1.0f;
	pThing->pSphereMeshMaterials->Specular.g = 1.0f;
	pThing->pSphereMeshMaterials->Specular.b = 1.0f;
	pThing->pSphereMeshMaterials->Emissive.r = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.g = 0.1f;
	pThing->pSphereMeshMaterials->Emissive.b = 0.1f;
	pThing->pSphereMeshMaterials->Power = 120.0f;

	return S_OK;
}
//
HRESULT SKIN_MESH::UpdateSphere(LPDIRECT3DDEVICE9 pDevice, THING* pThing)
{
/*	D3DXFrameCalculateBoundingSphere(pThing->pFrameRoot, &pThing->Sphere.vCenter,
		&pThing->Sphere.fRadius);
	pThing->pSphereMesh->Release();*/
	D3DXCreateSphere(pDevice, pThing->Sphere.fRadius, 5, 5, &pThing->pSphereMesh, NULL);

	return S_OK;
}
//
HRESULT SKIN_MESH::InitThing(LPDIRECT3DDEVICE9 pDevice,THING *pThing,LPSTR szXFileName)
{

	// Xファイルからアニメーションメッシュを読み込み作成する
	if( FAILED( 
	D3DXLoadMeshHierarchyFromX(szXFileName, D3DXMESH_MANAGED, pDevice, &cHierarchy, 
		                                         NULL, &pThing->pFrameRoot, &pThing->pAnimController)))
    {
            MessageBox(NULL, "Xファイルの読み込みに失敗しました",szXFileName, MB_OK);
            return E_FAIL;   
			
    }
	//
	AllocateAllBoneMatrices(pThing,pThing->pFrameRoot);

	return S_OK;
}

HRESULT SKIN_MESH::InitThing2(LPDIRECT3DDEVICE9 pDevice, THING *pThing, LPSTR szXFileName, MY_HIERARCHY Hierarchy)
{

	// Xファイルからアニメーションメッシュを読み込み作成する
	if (FAILED(
		D3DXLoadMeshHierarchyFromX(szXFileName, D3DXMESH_MANAGED, pDevice, &Hierarchy,
			NULL, &pThing->pFrameRoot, &pThing->pAnimController)))
	{
		MessageBox(NULL, "Xファイルの読み込みに失敗しました", szXFileName, MB_OK);
		return E_FAIL;

	}
	//
	AllocateAllBoneMatrices(pThing, pThing->pFrameRoot);

	return S_OK;
}
//

//フレーム内のそれぞれのメッシュをレンダリングする
VOID SKIN_MESH::RenderMeshContainer(LPDIRECT3DDEVICE9 pDevice, MYMESHCONTAINER* pMeshContainer, MYFRAME* pFrame, THING* pThing, bool type)
{

	DWORD i, k;
	DWORD dwBlendMatrixNum;
	DWORD dwPrevBoneID;
	LPD3DXBONECOMBINATION pBoneCombination;
	UINT iMatrixIndex;
	D3DXMATRIX mStack;
	//スキンメッシュの場合 
	if (type == true && pMeshContainer->pSkinInfo != NULL)
	{
		pBoneCombination = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneBuffer->GetBufferPointer();

		dwPrevBoneID = UINT_MAX;
		for (i = 0; i < pMeshContainer->dwBoneNum; i++)
		{
			dwBlendMatrixNum = 0;
			for (k = 0; k< pMeshContainer->dwWeight; k++)
			{
				if (pBoneCombination[i].BoneId[k] != UINT_MAX)
				{
					dwBlendMatrixNum = k;
				}
			}
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, dwBlendMatrixNum);
			for (k = 0; k < pMeshContainer->dwWeight; k++)
			{
				iMatrixIndex = pBoneCombination[i].BoneId[k];
				if (iMatrixIndex != UINT_MAX)
				{
					mStack = pMeshContainer->pBoneOffsetMatrices[iMatrixIndex] * (*pMeshContainer->ppBoneMatrix[iMatrixIndex]);
					pDevice->SetTransform(D3DTS_WORLDMATRIX(k), &mStack);
				}
			}
			pThing->texture = pMeshContainer->ppTextures;
			pDevice->SetMaterial(&pMeshContainer->pMaterials[pBoneCombination[i].AttribId].MatD3D);
			pDevice->SetTexture(0, pMeshContainer->ppTextures[pBoneCombination[i].AttribId]);
			dwPrevBoneID = pBoneCombination[i].AttribId;
			pMeshContainer->MeshData.pMesh->DrawSubset(i);

		}
	}
	//通常メッシュの場合
	else
	{
		pThing->texture = pMeshContainer->ppTextures;
	}
}
//
//フレームをレンダリングする。
VOID SKIN_MESH::DrawFrame(LPDIRECT3DDEVICE9 pDevice, LPD3DXFRAME pFrameBase, THING* pThing, bool type)
{
	int i = 0;
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;
	if (pMeshContainer != NULL)
	{

	}
	while (pMeshContainer != NULL)
	{
		RenderMeshContainer(pDevice, pMeshContainer, pFrame, pThing, type);

		pMeshContainer = (MYMESHCONTAINER*)pMeshContainer->pNextMeshContainer;
		i++;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameSibling, pThing, type);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pDevice, pFrame->pFrameFirstChild, pThing, type);
	}
}
//
//フレーム内のメッシュ毎にワールド変換行列を更新する
VOID SKIN_MESH::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
    MYFRAME *pFrame = (MYFRAME*)pFrameBase;	

    if (pParentMatrix != NULL)
	{
        D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
    else
	{
        pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
    if (pFrame->pFrameSibling != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
    }
    if (pFrame->pFrameFirstChild != NULL)
    {
        UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
    }
}
