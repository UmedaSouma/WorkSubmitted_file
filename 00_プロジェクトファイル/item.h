//===========================================================================================================================================================
// 
// item.cppÇÃÉwÉbÉ_Å[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "object2D.h"
#include "manager.h"

class CItem:public CObject2D
{
public:
	CItem();
	~CItem();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	bool CollisionItem();

	static CItem* Create(D3DXVECTOR3 pos);
	static HRESULT TextureLoad();
	static void TextureUnLoad();
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
};



#endif // !_ITEM_H_
