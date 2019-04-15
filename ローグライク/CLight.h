//////////////////////////////////////////////////
////
////	ライトクラス
////
//////////////////////////////////////////////////

#ifndef _CLIGHT_H_
#define _CLIGHT_H_

#include "CGameObj.h"


class CLight : public CGameObj
{
public:
	CLight();
	~CLight();

	void Update(void);
	void Draw(void);
	void Finalize(void);		//	終了処理
	static CLight* Light_Create(void);	//	ライトクラス作成

protected:
	

private:
	void Light_Initialize(void);	//　初期化
	void Light_Set(void);			//	ライトセット
	
	static CLight *m_pLight;
};

#endif // !1
