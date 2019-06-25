#pragma once
#ifndef CTRAP_DAMAGE_H_
#define CTRAP_DAMAGE_H_

#include <d3dx9.h>
#include "C2DObj.h"
#include "C3DObj.h"
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapDamage : public CTrap
{
public:
	//===================================
	// ÉèÉiç\ë¢ëÃ
	//===================================
	CTrapDamage(int x, int z, int trap_type);
	~CTrapDamage();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);
	bool Damage(int str, float angle, int week_type);

};
#endif
