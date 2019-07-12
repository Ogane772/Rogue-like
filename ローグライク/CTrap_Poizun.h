#pragma once
#ifndef CTRAP_POIZUN_H_
#define CTRAP_POIZUN_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapPoizun : public CTrap
{
public:
	CTrapPoizun(int x, int z, int trap_type);
	~CTrapPoizun();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
