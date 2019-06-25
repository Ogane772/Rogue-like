#ifndef TRAP_H_
#define TRAP_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
//===================================
// 定数
//===================================
#define OBJECT_MAXTIPS (399)//説明文の最大長さ
class CTrap :virtual public C3DObj
{
public:
	typedef struct {
		int trap_type;
		char trap_name[99];
		char trap_effect[OBJECT_MAXTIPS];//オブジェクト使用文
		int first_floor;//出現開始フロア
		int end_floor;//出現終わりフロア
		int trapchance;//ワナ出現率
	}TRAP_Data;
	typedef enum {
		TYPE_ALL = 0,

		TYPE_DAMAGE,
		TYPE_KUHUKU,
		TYPE_WARP_TRAP,
		TYPE_SLEEP_TRAP,
		TYPE_POIZUN_TRAP,
		TYPE_KUYARAMI_TRAP,
		TYPE_DONSOKU_TRAP,
		TYPE_ITEMDELETE_TRAP,
		TRAPTYPE_NONE
	}TRAPTYPE;

	CTrap();
	CTrap::CTrap(int TrapType);
	~CTrap();
	void Trap_Finalize(int Index);

	static void TrapDataInit(void);//CSVの読み込み
	static void Create(int trap_type, int x, int z);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int Get_TRAPDATAMAX(void) { return m_TRAPDATA_MAX; }
	static TRAP_Data* Get_Trap_Data(int index) { return &m_TrapData[index]; }
	static int Get_TrapNum(int OrnamType) { return m_TrapNum[OrnamType]; }
	static int Get_TrapIndex(int EnemyType) { return m_TrapNum[EnemyType] - 1; }
	int Get_Type(void) { return m_Type; }
	static char* Get_TrapName(int index) { return m_TrapData[index].trap_name; }
	static char* Get_TrapEffect(int index) { return m_TrapData[index].trap_effect; }
	static C3DObj *Get_Trap(int index);
	bool Get_DrawCheck(void) { return alive; }
	bool GetWanaCheck(void) { return m_WanaCheck; }
	bool GetWanaEffectOn(void) { return m_WanaEffectOn; }
	void SetWanaCheck(bool type) { m_WanaCheck = type;; }
	void SetWanaEffectOn(bool type) { m_WanaEffectOn = type; }
	static void CTrap::DeleteAllTrap(void);
	static C3DObj *Get_Map_Trap(int Index);
protected:
	int m_TrapIndex;
	bool m_WanaCheck;//罠が見えてたらtrue
	bool m_WanaEffectOn;//罠が発動してたらtrue

	static C3DObj *Get_AllTrap(int index);

private:
	static int m_TrapNum[TRAPTYPE_NONE];
	static int m_TRAPDATA_MAX;//ワナデータの最大数
	static TRAP_Data m_TrapData[];

	static int CSV_TrapLoad(TRAP_Data* trap, const int num);
};
#endif
