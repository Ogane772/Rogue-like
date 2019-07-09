#ifndef CWEPON_RANCE_H_
#define CWEPON_RANCE_H_

//===================================
// íËêî
//===================================

class CWeponRance : public CWepon
{
public:
	CWeponRance(int x, int z);
	~CWeponRance();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
};
#endif
