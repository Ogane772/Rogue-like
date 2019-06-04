#ifndef CWEPON_BIGSWORD_H_
#define CWEPON_BIGSWORD_H_

//===================================
// íËêî
//===================================

class CWeponBigSword : public CWepon
{
public:
	CWeponBigSword(int x, int z);
	~CWeponBigSword();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle, int week_type);
};
#endif
