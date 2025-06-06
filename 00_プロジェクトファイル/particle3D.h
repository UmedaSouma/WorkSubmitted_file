//===========================================================================================================================================================
// 
// particle.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_
#include "main.h"
#include "model.h"

//===========================================================================================================
// クラス
//===========================================================================================================

// パーティクル(モデル継承)
class CParticle3D:public CModel
{
public:
	CParticle3D();
	~CParticle3D()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CParticle3D* Create(D3DXVECTOR3 pos,int life);
private:
	int m_nLife;				// 寿命(フレーム単位)
	int m_nNum;					// 拡散量
	D3DXVECTOR3 m_move;

public:
	void SetLife(int life) { m_nLife = life; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove() { return m_move; }
};

#endif // !PARTICLE_H_