//===========================================================================================================================================================
// 
// モデルデータの管理
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "modeldata.h"
#include "manager.h"
#include "save.h"
#include "load.h"

int CModeldata::m_NumAll = 0;

//========================================================================================================================
// コンストラクタ
//========================================================================================================================
CModeldata::CModeldata()
{
	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		m_pModelData[nCnt] = {};

		
		m_pStorage[nCnt] = nullptr;

		m_pModelData[nCnt].Mesh = nullptr;
		m_pModelData[nCnt].BuffMat = nullptr;
		m_pModelData[nCnt].NumMat = 0;

		for (int n = 0; n < MAX_MAT_DATA; n++)
		{
			m_StandardCol[nCnt][n] = { 0.0f,0.0f,0.0f };
			m_pTexData[nCnt].pTex[n] = nullptr;
		}

		m_pTexData[nCnt].nIdx = 0;

		m_ModelFile[nCnt].Name = "";
		m_ModelFile[nCnt].Filepass = "";
	}

	m_MaxFilepassIdx = 0;

}

//========================================================================================================================
// デストラクタ
//========================================================================================================================
CModeldata::~CModeldata()
{
}

//========================================================================================================================
// 破棄
//========================================================================================================================
void CModeldata::Unload()
{
	//for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	//{
	//	if (m_pModelData != nullptr)
	//	{
	//		m_pModelData[nCnt] = {};
	//	}
	//}

	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		if (m_pStorage[nCnt] != nullptr)
		{
			m_pStorage[nCnt] = nullptr;
		}
	}

	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		if (m_pModelData[nCnt].BuffMat != nullptr)
		{
			m_pModelData[nCnt].BuffMat->Release();
			m_pModelData[nCnt].BuffMat = nullptr;
		}	
	}

	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		if (m_pModelData[nCnt].Mesh != nullptr)
		{
			m_pModelData[nCnt].Mesh->Release();
			m_pModelData[nCnt].Mesh = nullptr;
		}	
	}
}

//========================================================================================================================
// モデルデータの登録
//========================================================================================================================
int CModeldata::Regist(const char*modelpass)
{
	int nIdx = 0;
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		if (m_pModelData[nCnt].BuffMat == nullptr
			&& m_pModelData[nCnt].Mesh == nullptr
			&& m_pModelData[nCnt].NumMat == 0)
		{
			// Xファイルの読み込み
			if (SUCCEEDED(D3DXLoadMeshFromX(modelpass,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				nullptr,
				&m_pModelData[nCnt].BuffMat,
				nullptr,
				&m_pModelData[nCnt].NumMat,
				&m_pModelData[nCnt].Mesh)))
			{
				D3DXMATERIAL* pMat;				// マテリアルデータのポインタ
				pMat = (D3DXMATERIAL*)m_pModelData[nCnt].BuffMat->GetBufferPointer();	// マテリアルデータのポインタの取得

				for (int nCntMat = 0; nCntMat < (int)m_pModelData[nCnt].NumMat; nCntMat++)
				{
					//if (pMat[nCntMat].pTextureFilename != nullptr)
					//{
					//	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexData[nCnt].pTex[nCntMat])))	// テクスチャ作成
					//	{

					//	}
					//}

					m_StandardCol[nCnt][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;	// 元の色を保存しておく
				}

				m_pStorage[nCnt] = modelpass;
				nIdx = nCnt;
				m_NumAll++;

				break;
			}
			else
			{
				assert(false);
			}

			//===========================================================================================================
			// < debugmemo >
			// ここで元の色を保存しておく
			// SET or GET でどうにかする
			//===========================================================================================================
			
		}
		else if (!strcmp(modelpass, m_pStorage[nCnt]))
		{
			nIdx = nCnt;

			break;
		}
	}

	return nIdx;
}

//========================================================================================================================
// アドレスを返す
//========================================================================================================================
CModeldata::ModelData *CModeldata::GetAddress(int Idx)
{
	return &m_pModelData[Idx];
}

//===========================================================================================================
// 通常色のマテリアルデータを返す
//===========================================================================================================
D3DCOLORVALUE *CModeldata::GetMatData(const char* pAddress, int MatNum)
{
	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		if (!strcmp(pAddress, m_pStorage[nCnt]))
		{// アドレスが同じ番号を見つける
			return &m_StandardCol[nCnt][MatNum];	// 通常色を返す
		}
	}

	return nullptr;
}

//===========================================================================================================
// モデルファイルパスの初期化処理
//===========================================================================================================
void CModeldata::LoadModelFilePass()
{
	CLoad::ModelFiledata();
}

//===========================================================================================================
// ファイルパスの設定
//===========================================================================================================
std::string CModeldata::SetModelFilepass(std::string name, std::string address, ADD_DATA_MODE mode)
{	
	ADD_RESULT result = ADD_RESULT_NONE;

	for (int i = 0; i < MAX_MODEL_DATA; i++)
	{
		if (m_ModelFile[i].Name != "")	// 中身があったら
			continue;

		result = CompareWithExistingNames(name, address, i, mode);	// 名前の比較
		break;
	}

	return m_AddResultText[result];
}

//===========================================================================================================
// 名前の比較
//===========================================================================================================
CModeldata::ADD_RESULT CModeldata::CompareWithExistingNames(std::string name, std::string address, int Idx, ADD_DATA_MODE mode)
{
	ADD_RESULT result;

	for (int i = Idx; i >= 0; i--)
	{
		//if (strcmp(m_ModelFile[i].Name, name) == 0)	// 名前がかぶっていたら 即return
		//	return;

		if (m_ModelFile[i].Name == name)
			return ADD_RESULT_FAILURE_SAME_NAME;
	}

	result = CompareWithExistingAddress(name, address, Idx, mode);	// ファイルパスの比較

	return result;
}

//===========================================================================================================
// ファイルパスの比較
//===========================================================================================================
CModeldata::ADD_RESULT CModeldata::CompareWithExistingAddress(std::string name, std::string address, int Idx, ADD_DATA_MODE mode)
{
	for (int i = Idx; i >= 0; i--)
	{
		//if (strcmp(m_ModelFile[i].Filepass, address) == 0)	// ファイルパスがかぶっていたら 即return
		//	return;

		if (m_ModelFile[i].Filepass == address)
			return ADD_RESULT_FAILURE_SAME_FILEPASS;
	}

	// メンバ変数に登録する
	m_ModelFile[Idx].Name = name;
	m_ModelFile[Idx].Filepass = address;

	if (mode == ADD_DATA_MODE::ADD_DATA_WRITING)
	{
		// テキストファイルに書き出す
		CSave::AddModeFilepass(name.c_str(), address.c_str());

		return ADD_RESULT_SUCCESS;
	}

	m_MaxFilepassIdx = Idx;	// MAXIDXを設定する

	return ADD_RESULT_NONE;
}
