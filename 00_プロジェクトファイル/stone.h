//===========================================================================================================================================================
// 
// stone.cppのヘッダー [stone.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _STONE_H_
#define _STONE_H_
#include "main.h"
#include "block3D.h"
#include "manager.h"


class CStone :CBlock3D
{
public:
	CStone();
	~CStone()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CStone* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};

#endif // !STONE_H_