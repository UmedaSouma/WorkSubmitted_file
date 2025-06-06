#pragma once
//===========================================================================================================================================================
// 
// dangerzone.cppのヘッダー [dangerzone.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _DANGER_ZONE_H_
#define _DANGER_ZONE_H_
#include "main.h"
#include "object3D.h"

class CDangerZone :public CObject3D
{
public:
	CDangerZone();
	~CDangerZone()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CDangerZone* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);   // 生成処理
private:

};

#endif // !DANGER_ZONE_H_