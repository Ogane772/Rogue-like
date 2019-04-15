//////////////////////////////////////////////////
////
////	���C�g�N���X
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
	void Finalize(void);		//	�I������
	static CLight* Light_Create(void);	//	���C�g�N���X�쐬

protected:
	

private:
	void Light_Initialize(void);	//�@������
	void Light_Set(void);			//	���C�g�Z�b�g
	
	static CLight *m_pLight;
};

#endif // !1
