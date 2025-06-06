//===========================================================================================================================================================
// 
// object3D.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

class CObject3D:public CObject 
{
public:
	CObject3D(int nPriority = PRIORITY_DEFAULT);
	~CObject3D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void UpdateMatrix();
	void BindTexture(LPDIRECT3DTEXTURE9 pTex);	// テクスチャの設定

	//----------------------------------------------------
	// Set / Get
	//----------------------------------------------------

	// 位置 -------------------------------
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3& GetPos();

	// サイズ -----------------------------
	void SetSize(D3DXVECTOR3 size);
	D3DXVECTOR3& GetSize();

	// 角度 -------------------------------
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 &GetRot() { return m_rot; }

	// 拡縮率 -----------------------------
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	D3DXVECTOR3& GetScale() { return m_scale; }

	// 対角線の長さ ------------------------
	void SetLength(D3DXVECTOR3 sizeA, D3DXVECTOR3 sizeB);
	float GetLength() { return m_length; }

	// 対角線の角度 ------------------------
	void SetAngle(float sizeA, float sizeB);
	float GetAngle() { return m_Angle; }

	// 色 ---------------------------------
	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor() { return m_color; }

	const D3DXMATRIX& GetMatrix() const { return m_mtxWorld; }	// マトリックスの取得
	static CObject3D* Create(D3DXVECTOR3 pos);	// 生成処理
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_pVtxTexture;	// 頂点テクスチャ
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 角度
	D3DXVECTOR3 m_size;	// ポリゴンサイズ
	D3DXVECTOR3 m_scale;	// 拡縮率
	D3DXCOLOR m_color;	// 色

	float m_length;	// 範囲	
					//	|ー>円の判定に使う物
	float m_Angle;	// 角度
	D3DXMATRIX m_mtxWorld;
};

#endif // !_OBJECT3D_H_
