#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include "main.h"

//====================================================================================
// モデルに必要な情報の構造体
//====================================================================================

// モデルデータのクラス
class CModeldata
{
public:
//===========================================================================================================
// 列挙型
//===========================================================================================================
	// モデルファイルデータ追加時のモード
	typedef enum
	{
		ADD_DATA_WRITING = 0,	// 書き出し
		ADD_DATA_READING,		// 読み込み
	}ADD_DATA_MODE;

	// モデル追加時の結果
	typedef enum
	{
		ADD_RESULT_NONE = 0,				// 何も起こらない(読み込み時
		ADD_RESULT_SUCCESS,					// 追加成功
		ADD_RESULT_FAILURE_SAME_NAME,		// 追加失敗(名前が一緒)
		ADD_RESULT_FAILURE_SAME_FILEPASS,	// 追加失敗(ファイルパスが一緒)
		ADD_RESULT_MAX
	}ADD_RESULT;

//===========================================================================================================
// 定数値
//===========================================================================================================
	static const unsigned int MAX_MODEL_DATA = 64;	// モデルデータの最大値
	static const unsigned int MAX_MAT_DATA = 514;	// １キャラクター分のマテリアルデータ最大値

	// 追加ボタンを押した結果のテキスト
	std::string m_AddResultText[ADD_RESULT_MAX] = {
		"none",
		"追加が成功した(add success)",
		"IDの名前が一緒だったため追加が失敗した(add failure same name)",
		"ファイルパスが一緒だったため追加が失敗した(add failure same filepass)",
	};

//===========================================================================================================
// 構造体
//===========================================================================================================
	// モデルファイルの情報
	typedef struct
	{
		LPD3DXMESH Mesh;	// メッシュ(頂点情報)へのポインタ
		LPD3DXBUFFER BuffMat;	// マテリアルへのポインタ
		DWORD NumMat;	// マテリアルの数
	}ModelData;

	// モデルファイルパスのデータ
	typedef struct
	{
		std::string Name;	// ファイルのIDname
		std::string Filepass;	// ファイルのパス
	}MODELFILEPASS;

	// テクスチャデータ
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTex[MAX_MAT_DATA];
		int nIdx;
	}TexData;

	CModeldata();	// コンストラクタ
	~CModeldata();	// デストラクタ
	void Unload();	// 
	int Regist(const char* pTexturename);
	ModelData *GetAddress(int Idx);
	D3DCOLORVALUE *GetMatData(const char* pAddress, int MatNum);

	void LoadModelFilePass();	// モデルファイルパスの初期化
	std::string SetModelFilepass(std::string name, std::string address, ADD_DATA_MODE mode);	// ファイルパスを設定
	ADD_RESULT CompareWithExistingNames(std::string name, std::string address, int Idx, ADD_DATA_MODE mode);	// 名前との比較
	ADD_RESULT CompareWithExistingAddress(std::string name, std::string address, int Idx, ADD_DATA_MODE mode);	// ファイルパスの比較

	void SetModelFilePassData(MODELFILEPASS pData, int Idx) { m_ModelFile[Idx] = pData; }	// モデルファイルパスデータの設定
	MODELFILEPASS* GetModelFilePassData() { return &m_ModelFile[0]; }	// モデルファイルパスデータの取得
	int GetMaxFilepassIdx() { return m_MaxFilepassIdx; }	// モデルファイルパスのインデックスの取得
private:
	
	TexData m_pTexData[MAX_MODEL_DATA];
	ModelData m_pModelData[MAX_MODEL_DATA];
	D3DCOLORVALUE m_StandardCol[MAX_MODEL_DATA][MAX_MAT_DATA];	// 色を変える前の元の色
	MODELFILEPASS m_ModelFile[MAX_MODEL_DATA];	// モデルファイルのファイルパスのデータ
	int m_MaxFilepassIdx;	// モデルファイルパスのインデックス

	const char* m_pStorage[MAX_MODEL_DATA];
	static int m_NumAll;
	

	//LPD3DXMESH Mesh;			// メッシュ(頂点情報)へのポインタ
	//LPD3DXBUFFER BuffMat;	// マテリアルへのポインタ
	//DWORD NumMat;	// マテリアルの数
};

#endif // !_MODELDATA_H_
