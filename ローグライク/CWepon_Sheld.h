#ifndef CWEPON_SHELD_H_
#define CWEPON_SHELD_H_
//===================================
// �萔
//===================================

class CWeponSheld : public CWepon
{
public:
	//===================================
	// �I�u�W�F�N�g�\����
	//===================================
	CWeponSheld(int x, int z);
	~CWeponSheld();

	void Initialize(int x, int z);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle);
};
#endif
