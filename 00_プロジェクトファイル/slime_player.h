//===========================================================================================================================================================
// 
// slime_player.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SLIME_PLAYER_H_
#define _SLIME_PLAYER_H_
#include "main.h"
#include "slime.h"

class CSlimePlayer :public CSlime
{
public:
	CSlimePlayer();
	~CSlimePlayer()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	void UseSkillUpdate();
	static CSlimePlayer* Create(D3DXVECTOR3 pos);   // 生成処理
private:
	int m_nSoundCnt;
};

#endif // !SLIME_PLAYER_H_