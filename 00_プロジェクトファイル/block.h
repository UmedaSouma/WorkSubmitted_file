//===========================================================================================================================================================
// 
// block.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "object2D.h"

//========================================================================
// クラス
//========================================================================

// ブロッククラス
class CBlock:public CObject2D
{
public:
	CBlock();					// コンストラクタ
	~CBlock();					// デストラクタ
	HRESULT Init()override;		// 初期設定処理
	void Uninit()override;		// 終了処理
	void Update()override;		// 更新処理
	void Draw()override;		// 描画処理

	static CBlock* Create(D3DXVECTOR3 pos);	// 生成処理
	static void TextureLoad();				// テクスチャ読み込み処理
	static void TextureUnload();			// テクスチャ破棄処理
private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャのポインタ確保変数
};

//class CObstacle :public CBlock
//{
//public:
//	CObstacle();
//	~CObstacle();
//	HRESULT Init()override;
//	void Uninit()override;
//	void Update()override;
//	void Draw()override;
//	void UpDownCollision();
//
//private:
//
//};



#endif // !_BLOCK_H_
