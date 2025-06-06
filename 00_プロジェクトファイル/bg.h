//===========================================================================================================================================================
// 
// bg.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _BG_H_
#define _BG_H_

#include "object.h"

//========================================================================
// クラス
//========================================================================

// 背景のクラス
class CBg :public CObject
{
public:
	CBg(int nPriorty=0);	// コンストラクタ(描画を一番下に
	~CBg();					// デストラクタ
	HRESULT Init()override;	// 初期化処理
	void Uninit()override;	// 終了処理
	void Update()override;	// 更新処理
	void Draw()override;	// 描画処理
	static CBg* Create();	// 作成処理
private:
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_size;	// 大きさ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_pVtxTexture;	// テクスチャのポインタ
	D3DXVECTOR2 m_nTexSplit;	// テクスチャの分割数
};

#endif // !_BG_H_
