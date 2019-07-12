#pragma once
#ifndef CTRAP_SLEEP_H_
#define CTRAP_SLEEP_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapSleep : public CTrap
{
public:
	CTrapSleep(int x, int z, int trap_type);
	~CTrapSleep();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
