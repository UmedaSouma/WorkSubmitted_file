//===========================================================================================================================================================
// 
// explosion.cppÇÃÉwÉbÉ_Å[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "manager.h"
#include "object2D.h"

class CExplosion :public CObject2D
{
public:
	CExplosion();
	~CExplosion();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CExplosion *Create(D3DXVECTOR3 pos , D3DXVECTOR2 TexSplit);
	static HRESULT TextureLoad();
	static void TextureUnLoad();
private:
	float m_nMaxTexSplit;
	static LPDIRECT3DTEXTURE9 m_pTexture;
};



#endif // !_EXPLOSION_H_
