#pragma once
//===========================================================================================================================================================
// 
// golem_player.cppのヘッダー [golem_player.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GOLEM_PLAYER_H_
#define _GOLEM_PLAYER_H_
#include "main.h"
#include "golem.h"

class CGolemPlayer:public CGolem
{
public:
	CGolemPlayer();
	~CGolemPlayer()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CGolemPlayer* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};



#endif // !GOLEM_PLAYER_H_
