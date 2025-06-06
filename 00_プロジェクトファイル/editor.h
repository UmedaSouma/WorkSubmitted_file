//===========================================================================================================================================================
// 
// editor.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _EDITOR_H_
#define _EDITOR_H_
#include "main.h"
#include "scene.h"

class CEditor :public CScene
{
public:
	CEditor();
	~CEditor();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	
	void InputEdit(D3DXVECTOR3& pos);	// エディターの入力処理
	void InputBaseObj(D3DXVECTOR3& pos);	// ベースオブジェクトの入力処理

	void InputMove(D3DXVECTOR3& pos);	// 移動処理
	void InputKeyboadMove(D3DXVECTOR3& pos, int mag);	// キーボードの移動操作
	
	void CreateEdit(const D3DXVECTOR3 pos);	// ブロックを生成
	void DestroyEdit(const D3DXVECTOR3 pos);	// ブロックを消去

	void UpdateSetModel(int type);	// モデルをセット
	void SelectObject();	// オブジェクトを選択

	static int GetModelType() { return m_type; }	// モデルの種類を選択
private:
	static CModel* pBaseObj;	// 全てのベース、このベースを基準に動く
	static int m_type;
	static int m_old_type;	// 比べるための古いタイプ
	bool IsOpenBaseObjWindow;	// ベースオブジェクトのウィンドウを出す

};



#endif // !EDITOR_H_