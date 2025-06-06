//===========================================================================================================================================================
// 
// grass.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GRASS_H_
#define _GRASS_H_
#include "main.h"
#include "block3D.h"


class CGrass: CBlock3D
{
private:
	static inline const float MAX_SIZE = 23.0f;
public:
	CGrass();
	~CGrass()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CGrass* Create(D3DXVECTOR3 pos);
private:

};


#endif // !_GRASS_H_