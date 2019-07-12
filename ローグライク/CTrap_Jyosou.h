#pragma once
#ifndef CTRAP_JYOSOU_H_
#define CTRAP_JYOSOU_H_

#include <d3dx9.h>
#include "CTrap.h"
//===================================
// íËêî
//===================================

class CTrapJyosou : public CTrap
{
public:
	CTrapJyosou(int x, int z, int trap_type);
	~CTrapJyosou();

	void Initialize(int x, int z, int trap_type);
	void Finalize(void);
	void Update(void);
	void Draw(void);

};
#endif
