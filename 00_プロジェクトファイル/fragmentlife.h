//===========================================================================================================================================================
// 
// fragmentlife.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _FLAGMENTLIFE_H_
#define _FLAGMENTLIFE_H_
#include "main.h"
#include "particle3D.h"
#include "enemy3D.h"

class CFlagmentLife:public CParticle3D
{
public:
	CFlagmentLife();
	~CFlagmentLife()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画
	bool SearchPlayer();
	bool Collision(D3DXVECTOR3 pos,D3DXVECTOR3 size);

	static CFlagmentLife* Create(D3DXVECTOR3 pos, int life, CEnemy3D::TAG_ENEMY tag);
	static void SetNumParticle(D3DXVECTOR3 pos, int life, int num, CEnemy3D::TAG_ENEMY tag);
private:
	CEnemy3D::TAG_ENEMY m_tag;	// 敵のタグ
};


#endif // !FLAGMENTLIFE_H_