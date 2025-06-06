#pragma once
//===========================================================================================================================================================
// 
// sword.cppのヘッダー [sword.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SWORD_H_
#define _SWORD_H_
#include "main.h"
#include "model.h"

//===========================================================================================================
// < debugmemo >
// weapon クラスという派生から派生していくといろんな武器クラスが作れるかも？
// 今回は要らないか？剣スキルとかは weapon から派生した方がいいのか？
//===========================================================================================================

class CSword:public CModel
{
public:
	CSword();
	~CSword()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CSword* Create(D3DXVECTOR3 pos);   // 生成処理

	void UpdateRot();
	void TargetPos(D3DXVECTOR3 pos);	// このオブジェクトの親の位置
	void TargetRot(D3DXVECTOR3 rot, int frame);	// このオブジェクトの目標の角度 + 何フレームで回転しきるか
	D3DXVECTOR3& GetTargetrot() { return m_Targetrot; }
private:
	D3DXVECTOR3 m_Targetrot;	// 目標の角度
	D3DXVECTOR3 m_Moverot;		// 目標の角度までの移動量
	int m_TargetFrame;			// 目標のフレーム数
	int m_FrameCnt;			// フレームを保持する変数

};

#endif // !SWORD_H_
