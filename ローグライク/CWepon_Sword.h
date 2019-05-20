#ifndef CWEPON_SWORD_H_
#define CWEPON_SWORD_H_

//===================================
// íËêî
//===================================

class CWeponSword : public CWepon
{
public:
	CWeponSword(int x, int z);
	~CWeponSword();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle);
};
#endif
