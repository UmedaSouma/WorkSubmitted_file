#pragma once
//===========================================================================================================================================================
// 
// buttleUI.cppのヘッダー [buttleUI.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BUTTLE_UI_H_
#define _BUTTLE_UI_H_
#include "main.h"
#include "object2D.h"
#include "enemy3D.h"

class CButtleUI :public CObject2D
{
public:
	// 選択されている味方モンスターとその前後の番号
	typedef enum
	{
		BUTTLE_MESSE_FRONT=0,	// 選択されているモンスター
		BUTTLE_MESSE_NEXT,	// 次
		BUTTLE_MESSE_PREV,	// 前
		BUTTLE_MESSE_MAX
	}BUTTLE_MESSE;

	CButtleUI(int nPriority = 4);
	~CButtleUI()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CButtleUI* Create(D3DXVECTOR3 pos,D3DXVECTOR3 size);   // 生成処理

	void SetTex_SelectMessenger(CEnemy3D::TAG_ENEMY tag);	// 
	void SetBPush(bool bPush) { m_bIsPush = bPush; }	// 
private:
	bool m_bIsPush;		// ボタンが入力されたか、確認する
	bool m_bIsCoolTime;	// クールタイム中か、確認する
};



#endif // !BUTTLE_UI_H_