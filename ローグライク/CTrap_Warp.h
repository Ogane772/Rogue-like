#pragma once
#ifndef CTRAP_WARP_H_
#define CTRAP_WARP_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapWarp : public CTrap
{
public:
	CTrapWarp(int x, int z, int trap_type);
	~CTrapWarp();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
