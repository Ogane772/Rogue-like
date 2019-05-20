#ifndef CWEPON_RING_H_
#define CWEPON_RING_H_

//===================================
// �萔
//===================================

class CWeponRing : public CWepon
{
public:
	CWeponRing(int x, int z);
	~CWeponRing();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle);
};
#endif
