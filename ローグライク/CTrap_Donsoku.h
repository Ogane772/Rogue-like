#pragma once
#ifndef CTRAP_DONSOKU_H_
#define CTRAP_DONSOKU_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapDonsoku : public CTrap
{
public:
	CTrapDonsoku(int x, int z, int trap_type);
	~CTrapDonsoku();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
