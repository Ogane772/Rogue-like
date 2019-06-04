#ifndef CWEPON_AX_H_
#define CWEPON_AX_H_

//===================================
// �萔
//===================================

class CWeponAx : public CWepon
{
public:
	CWeponAx(int x, int z);
	~CWeponAx();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle, int week_type);
};
#endif
