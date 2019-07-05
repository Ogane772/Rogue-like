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
		int type;//武器タイプ
		char Wepon_name[MAX_NAME];
		char Wepon_effect[WEPON_MAXTIPS];//オブジェクト説明文
		int str;
		int	def;
		int first_floor;//出現開始フロア
		int end_floor;//出現終わりフロア
		int weponchance;//装備出現率
		int rare;//武器のレア値
		int kantei_type;//どの装備か識別する
	}Wepon_Data;
	//武器スキルの名前と説明文
	typedef struct {
		char Skill_name[MAX_NAME];
		char Skill_Hp[MAX_NAME];//スキルの消費HP量
		char Skill_effect[WEPON_MAXTIPS];
		char Skill_uieffect[WEPON_MAXTIPS];//UIログでの表示
	}Wepon_Skill;
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

	typedef enum {
		TYPE_NONE = 0,

		TYPE_IYASI_RING,
		TYPE_WANAYOKE_RING,
		TYPE_HARAMOTI_RING,
		TYPE_SIAWASE_RING,
		TYPE_HIGHATTACK_SKILL,//強攻撃
		TYPE_BACKATTACK_SKILL,//吹っ飛ばし攻撃
		TYPE_WARP_SKILL,//ワープ攻撃	
		TYPE_POIZUN_ATTACK,//毒攻撃	
		TYPE_SLEEP_ATTACK,//眠り攻撃
		TYPE_KURAYAMI_ATTACK,//暗闇攻撃	
		TYPE_MAHUJI_ATTACK,//魔封じ攻撃	
		TYPE_THREE_ATTACK,//目の前3マス攻撃	
		WEPON_NAME_NONE
	}WEPON_NAME_TYPE;

	CWepon();
	CWepon::CWepon(int WeponType);
	~CWepon();

	void Wepon_Finalize(int Index);
	static void WeponDataInit(void);
	static void Create(int wepon_type, int x, int z);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int Get_WEPONDATAMAX(void) { return m_WEPONDATA_MAX; }
	static int Get_WeponNum(int OrnamType) { return m_WeponNum[OrnamType]; }
	static int Get_WeponIndex(int EnemyType) { return m_WeponNum[EnemyType] - 1; }
	int Get_Type(void) { return m_Type; }
	static Wepon_Skill* Get_Wepon_Skill(int index) { return &m_WeponSkill[index]; }
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
	static int m_WeponNum[WEPONTYPE_NONE];

	static Wepon_Data m_WeponData[];
	static Wepon_Skill m_WeponSkill[];
	static int CSV_Wepon_Load(Wepon_Data* wepondata, const int num);//装備テーブル読み込み
};
#endif
