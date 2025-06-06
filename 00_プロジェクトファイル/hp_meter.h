#pragma once
//===========================================================================================================================================================
// 
// hp_meter.cppのヘッダー [hp_meter.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _HP_METER_H_
#define _HP_METER_H_
#include "main.h"
#include "UI.h"

//===========================================================================================================
// HPバーのクラス
//===========================================================================================================
class CHpMeter :public CUI
{
public:
	// HPバーのタイプ
	typedef enum
	{
		HP_LITERAL = 0,	// 不変のHP
		HP_VARIABLE,	// 可変のHP
		HP_MAX
	}HP_TYPE;

	CHpMeter();	// コンストラクタ
	~CHpMeter()override;	// デストラクタ
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CHpMeter* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,HP_TYPE type,TYPE chartype);   // 生成処理
private:
	HP_TYPE m_hptype;	// hitpointバーのタイプ
	TYPE m_CharType;	// HPの持ち主のタイプ
};

#endif // !HP_METER_H_
