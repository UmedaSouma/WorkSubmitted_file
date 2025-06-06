//===========================================================================================================================================================
// 
// あらゆるセーブをここでする [save.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include <fstream>

#include "search.h"
#include "save.h"
#include "model.h"


//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CSave::CSave()
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CSave::~CSave()
{
}

//===========================================================================================================
// ステージ書き込み処理
//===========================================================================================================
void CSave::StageText()
{
	using namespace std;

	string saveloadtxt = "data\\text\\stage01.txt";
	string testtxt = "data\\text\\test.bin";


	int n = 0;
	ofstream file(saveloadtxt);
	CModel* pModel = nullptr;
	string str = " ";

	if (file.is_open())
	{
		file << "SCRIPT\n\n";

		for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
		{
			CObject* pObj = CObject::GetTop(nPrio);

			while (pObj != nullptr)
			{// オブジェクトを探す

				CObject* pNext = pObj->GetNext();

				pModel = CSearch::SearchType(pModel, pObj);	// タイプをセットする

				str = SetSaveTypeText((int)pModel->GetModelType());	// モデルのタイプを取得する

				// ゴミが入るので二回目以降からデータを入れる
				if (n >= 1 && str != "none")
				{
					//file.write(reinterpret_cast<const char*>(&pModel->GetPos().x), sizeof(pModel->GetPos().x));

					//file << "\n\n";

					file << "pos = " << pModel->GetPos().x << " , " << pModel->GetPos().y << " , " << pModel->GetPos().z << "\n";
					file << "type = " << str << "\n\n";
				}

				pObj = pNext;

				n++;
			}
		}
		file << "END_SCRIPT\n\n";

		file.close();
	}
	else
	{
		assert(0);
		return;
	}


}

//===========================================================================================================
// モデルのファイルパス追加
//===========================================================================================================
void CSave::AddModeFilepass(const char* name, const char* address)
{
	using namespace std;

	string text = "data\\text\\test.txt";	// テキストファイルのパス
	ofstream file(text,ios::app);	// 追記する形で

	if (file.is_open())
	{// ファイルが正常に開けたら

		// 名前とファイルパスを書き出す
		file << "NAME=" << name << "\n" << "FILE_PASS=" << address << "\n\n";

		file.close();
	}
	else
	{
		assert(0);
		return;
	}
}

//===========================================================================================================
// タイプを設定して書き込む
//===========================================================================================================
std::string CSave::SetSaveTypeText(int type)
{
	switch (type)
	{
	case CModel::MODEL_TYPE::GRASS:
		return "grass";

		break;

	case CModel::MODEL_TYPE::STONE:
		return "stone";

		break;

	case CModel::MODEL_TYPE::START:
		return "start";

		break;

	case CModel::MODEL_TYPE::GOAL:
		return "goal";

		break;

	case CModel::MODEL_TYPE::SLIME:
		return "slime";

		break;

	case CModel::MODEL_TYPE::GOLEM:
		return "golem";

		break;

	default:
		assert(1);
		break;
	}

	return "none";
}