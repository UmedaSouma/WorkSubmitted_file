#pragma once
//===========================================================================================================================================================
// 
// block_gimmick.cppのヘッダー [block_gimmick.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BLOCK_GIMMICK_H_
#define _BLOCK_GIMMICK_H_
#include "main.h"
#include "block3D.h"

class CBlockGimmick:public CBlock3D
{
public:
	CBlockGimmick();
	~CBlockGimmick()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CBlockGimmick* Create(D3DXVECTOR3 pos/*, D3DXVECTOR3 size*/);   // 生成処理
private:
	bool m_Gimmick;
};



#endif // !BLOCK_GIMMICK_H_
