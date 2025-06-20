//===========================================================================================================================================================
// 
// score.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"
#include "object.h"

class CScore: public CObject
{
public:
	CScore();
	~CScore()override;
	HRESULT Init()override;	// 初期設定
	void Uninit()override;	// 終了
	void Update()override;	// 更新
	void Draw()override;	// 描画

	static CScore* Create();
private:
	int m_nScore;	// スコア

public:
	void AddScore(int score) { m_nScore += score; }	// スコアの加算減算
	void SetScore(int score) { m_nScore = score; }	// スコアの数値設定
	int GetScore() { return m_nScore; }				// スコア取得
};

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CScore::CScore()
{
}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CScore::~CScore()
{
}

//========================================================================================================================
// 初期設定
//========================================================================================================================
HRESULT CScore::Init()
{
	CObject::Init();
	return S_OK;
}

//========================================================================================================================
// 終了処理
//========================================================================================================================
void CScore::Uninit()
{
	CObject::Uninit();
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void CScore::Update()
{
	CObject::Update();
}

//========================================================================================================================
// 描画処理
//========================================================================================================================
void CScore::Draw()
{
	CObject::Draw();
}

//========================================================================================================================
// 生成処理
//========================================================================================================================
CScore* CScore::Create()
{
	CScore* pScore = new CScore;

	return pScore;
}

#endif // !SCORE_H_