#pragma once
#ifndef CTRAP_DAMAGE_H_
#define CTRAP_DAMAGE_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// �萔
//===================================

class CTrapDamage : public CTrap
{
public:
	CTrapDamage(int x, int z, int trap_type);
	~CTrapDamage();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
