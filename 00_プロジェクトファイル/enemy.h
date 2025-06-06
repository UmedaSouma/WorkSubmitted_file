//===========================================================================================================================================================
// 
// enemy.cppÇÃÉwÉbÉ_Å[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "manager.h"
#include "object2D.h"

class CEnemy:public CObject2D
{
public:
	CEnemy();
	~CEnemy();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	static CEnemy* Create(D3DXVECTOR3 pos);
	static HRESULT TextureLoad();
	static void TextureUnLoad();
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
};

#endif // !_ENEMY_H_
