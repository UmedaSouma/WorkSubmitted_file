//===========================================================================================================================================================
// 
// bullet.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "object2D.h"

class CBullet :public CObject2D
{
public:
	CBullet(int nPriority = PRIORITY_DEFAULT+1);
	~CBullet();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void LookObjectALL();	// 全てのオブジェクトを確認する
	bool CollisionEnemy(CObject* pObj);

	static HRESULT TextureLoad();
	static void TextureUnLoad();
	static CBullet* Create(D3DXVECTOR3 pos);

	static const int MAX_LIFE = 60;
private:
	int m_nLife;
	float m_fSpeed;
	int m_nPriority;
	bool m_bIsEnemyCollision;

	static LPDIRECT3DTEXTURE9 m_pTexture;
};



#endif // !_BULLET_H_
