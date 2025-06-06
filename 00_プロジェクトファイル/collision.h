//===========================================================================================================================================================
// 
// collision.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_
#include "main.h"
#include "object.h"
#include "block3D.h"


class CCollision
{
public:
	CCollision();
	~CCollision();
	HRESULT Init();	// 初期設定
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	static bool BlockVertical(CObject *obj,CObject::TYPE ObjType,CBlock3D *block);
	static bool BlockHorizon(CObject *obj, CObject::TYPE ObjType, CBlock3D *block);
	static bool BlockHighLow(CObject *obj, CObject::TYPE ObjType, CBlock3D *block);

private:

};




#endif // !COLLISION_H_