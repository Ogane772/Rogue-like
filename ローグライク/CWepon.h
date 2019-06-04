#ifndef CWEPON_H_
#define CWEPON_H_

#include "C3DObj.h"
//===================================
// 定数
//===================================
#define WEPON_MAXTIPS (99)//説明文の最大長さ
#define MAX_WEPON_NUMBER (30)//武器番号格納最大数
class CWepon :virtual public C3DObj
{
public:
	typedef struct {
		int type;
		char Wepon_name[MAX_NAME];
		char Wepon_effect[WEPON_MAXTIPS];//オブジェクト説明文
		int str;
		int	def;
		int first_floor;//出現開始フロア
		int end_floor;//出現終わりフロア
		int weponchance;//装備出現率
	}Wepon_Data;
	typedef enum {
		TYPE_ALL = 0,

		TYPE_SWORD,//片手剣
		TYPE_BIGSWORD,//大剣
		TYPE_REIPIA,//レイピア
		TYPE_RANCE,//ランス
		TYPE_MEISU,//メイス
		TYPE_AX,//斧	
		TYPE_SHELD,//盾
		TYPE_RING,//指輪
		WEPONTYPE_NONE
	}WEPONTTYPE;

	CWepon();
	CWepon::CWepon(int WeponType);
	~CWepon();

	void Wepon_Finalize(int Index);
	static void Create(int wepon_type, int x, int z);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int Get_WEPONDATAMAX(void) { return m_WEPONDATA_MAX; }
	static int Get_WeponNum(int OrnamType) { return m_WeponNum[OrnamType]; }
	static int Get_WeponIndex(int EnemyType) { return m_WeponNum[EnemyType] - 1; }
	int Get_Type(void) { return m_Type; }
	static Wepon_Data* Get_Wepon_Data(int index) { return &m_WeponData[index]; }
	static char* Get_WeponName(int index) { return m_WeponData[index].Wepon_name; }
	static char* Get_WeponEffect(int index) { return m_WeponData[index].Wepon_effect; }
	static C3DObj *Get_Wepon(int index);
	bool Get_DrawCheck(void) { return alive; }
	static void CWepon::DeleteAllWepon(void);
	static C3DObj *Get_Map_Wepon(int Index);
protected:

	int m_WeponIndex;

	static C3DObj *Get_AllWepon(int index);

private:
	static int m_WEPONDATA_MAX;//ウェポンデータの最大数
	static int m_WeponNum[TYPE_MAX];

	static Wepon_Data m_WeponData[];
};
#endif
