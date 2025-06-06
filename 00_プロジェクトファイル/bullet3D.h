#pragma once
//===========================================================================================================================================================
// 
// bullet3D.cppのヘッダー [bullet3D.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BULLET_3D_H_
#define _BULLET_3D_H_
#include "main.h"
#include "model.h"

class CBullet3D :public CModel
{
public:
	CBullet3D();
	~CBullet3D()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	void UpdateType();	// 種類ごとの Update
	void UpdateAlly();	// 味方の Update
	void UpdateEnemy();	// 敵の Update
	int Collision(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// 当たり判定

	static CBullet3D* Create(CObject::TYPE type, D3DXVECTOR3 pos,D3DXVECTOR3 move,int life);   // 生成処理
private:
	int m_nLife;			// 弾の寿命
	D3DXVECTOR3 m_move;		// 移動量
	CObject::TYPE m_type;		// 弾を発射した元のモデルのタイプ
};

#endif // !BULLET_3D_H_