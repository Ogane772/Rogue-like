#ifndef CWEPON_REIPIA_H_
#define CWEPON_REIPIA_H_

//===================================
// íËêî
//===================================

class CWeponReipia : public CWepon
{
public:
	CWeponReipia(int x, int z);
	~CWeponReipia();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle);
};
#endif
