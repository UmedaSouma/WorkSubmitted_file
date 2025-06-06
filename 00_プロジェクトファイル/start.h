//===========================================================================================================================================================
// 
// start.cppのヘッダー [start.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _START_H_
#define _START_H_
#include "main.h"
#include "model.h"


class CStart :CModel
{
public:
	CStart();
	~CStart()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CStart* Create(D3DXVECTOR3 pos);   // 生成処理
private:

};


#endif // !START_H_