#ifndef CWEPON_MEISU_H_
#define CWEPON_MEISU_H_

//===================================
// íËêî
//===================================

class CWeponMeisu : public CWepon
{
public:
	CWeponMeisu(int x, int z);
	~CWeponMeisu();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle);
};
#endif
