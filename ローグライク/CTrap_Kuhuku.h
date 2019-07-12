#pragma once
#ifndef CTRAP_KUHUKU_H_
#define CTRAP_KUHUKU_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapKuhuku : public CTrap
{
public:
	CTrapKuhuku(int x, int z, int trap_type);
	~CTrapKuhuku();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
