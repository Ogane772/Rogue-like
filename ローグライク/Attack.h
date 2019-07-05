#ifndef ATTACK_H_
#define ATTACK_H_
#define CHARGE_BUFF (1.25f)//チャージ倍率
#define ESCAPE_NUMBER (12)//回避値
#define ESCAPE_OK (true)  //回避成功
#define ESCAPE_NO (false) //回避失敗
//回避チェックするかどうか
#define ESCAPE_CHECK_OK (true)//チェックする
#define ESCAPE_CHECK_NO (false)//チェックしない
class CAttack :virtual public C3DObj
{
public:
	enum EnemySkillNumber{
		NORMAL_ATTACK_SKILL,
		HIGH_ATTACK_SKILL,
		HITYU_ATTACK_SKILL,//必中斬り
		HANI_FURASHU_SKILL,//ハニーフラッシュ
		KIAIDAME_SKILL,//気合溜め
		SUNAKEMURI_SKILL,
		KOMORIUTA_SKILL,
		POIZUN_SKILL,
	};
	static void Attack_PlayerUpdate(int charatype, float vec, int str, float str2);
	static void Attack_PlayerSkill(int skill_number, float vec, int str, float str2);
	//目の前の3マス攻撃
	static int Attack_PlayerThreeRenge(int skill_number, float vec, int str, float str2);
	static void Attack_EnemyUpdate(int enemy_wepon_type, int charatype, int str, float str2 ,float angle);
	static void Attack_EnemySkill(int skill_number, int enemy_wepon_type, int charatype, int str, float str2, float angle, bool escape_check);
	static void Skill_PlayerEffect(C3DObj *enemy, int skill_number, int damage, int week_check);//プレイヤーの対応するスキル効果
	static void Skill_Effect(int skill_number, int damage, int week_check, float angle, int charatype);//敵の対応するスキル効果
	static bool Attack_Check(void);//攻撃当たるか避けるか
	static int Attack_WeekCheck(int attack_wepontype, int damage_wepontype);//相性を比較
	static int Attack_DamegeKeisan(int week_check, C3DObj *enemy, int str, float str2);//ダメージを計算して返す
};
#endif