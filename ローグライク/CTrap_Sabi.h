#pragma once
#ifndef CTRAP_SABI_H_
#define CTRAP_SABI_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapSabi : public CTrap
{
public:
	CTrapSabi(int x, int z, int trap_type);
	~CTrapSabi();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
