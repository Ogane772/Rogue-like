#pragma once
#ifndef CTRAP_KURAYAMI_H_
#define CTRAP_KURAYAMI_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapKurayami : public CTrap
{
public:
	CTrapKurayami(int x, int z, int trap_type);
	~CTrapKurayami();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
