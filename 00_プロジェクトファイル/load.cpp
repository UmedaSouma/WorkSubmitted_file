//===========================================================================================================================================================
// 
// あらゆるものを読み込む [load.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include <fstream>
#include <sstream>

#include "load.h"
#include "grass3D.h"
#include "stone.h"
#include "start.h"
#include "goal.h"
#include "slime_enemy.h"
#include "golem_enemy.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
CLoad::CLoad()
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
CLoad::~CLoad()
{
}

//===========================================================================================================
// ステージ読み込み処理
//===========================================================================================================
void CLoad::LoadText()
{
	std::string saveloadtxt = "data\\text\\stage01.txt";

	std::ifstream file(saveloadtxt);
	std::string line;

	//****************************************************************
	// < debugmemo >
	// ファイル読み込み一行コード
	// 後々実装予定
	//****************************************************************

	if (!file)
	{// ファイルが存在しなかった場合

		if (CScene::GetMode() != CScene::MODE_EDITOR)
		{// エディター以外の時中止させる
			assert(0);
		}

		return;
	}

	while (std::getline(file, line))
	{// 一列ずつ読み取っていく

		if (line == "SCRIPT")
		{// ファイルの文字と比較
			StageText(file);
		}
	}
}



void CLoad::StageText(std::ifstream& file)
{
	using namespace std;

	vector<float> Vec3;	// vector3 型の数値を入れる変数
	float f = 0.0f;	// 取得した数値を一時格納する変数
	string line;	// ファイルから読み取った文字列を入れる変数

	//while (getline(file, line))
	{
		while (getline(file, line, '='))
		{

			if (line.find("pos") != string::npos)
			{
				getline(file, line);	// GetLine で = 以降に進める

				stringstream ss{ line };	// Getline で使えるように stringstream に現在の文字列を代入する

				// コンマで区切って情報を取得する
				while (getline(ss, line, ','))
				{
					f = stof(line);
					Vec3.push_back(f);
				}
			}

			if (line.find("type") != string::npos)
			{
				getline(file, line);

				if (line.find("grass") != string::npos)
				{
					CGrass::Create({ Vec3[0],Vec3[1],Vec3[2] });

					Vec3.clear();
				}
				else if (line.find("stone") != string::npos)
				{
					CStone::Create({ Vec3[0],Vec3[1],Vec3[2] });

					Vec3.clear();
				}
				else if (line.find("start") != string::npos)
				{
					CStart::Create({ Vec3[0],Vec3[1],Vec3[2] });

					Vec3.clear();
				}
				else if (line.find("goal") != string::npos)
				{
					CGoal::Create({ Vec3[0],Vec3[1],Vec3[2] });

					Vec3.clear();
				}
				else if (line.find("slime") != string::npos)
				{
					CSlimeEnemy::Create({ Vec3[0],Vec3[1],Vec3[2] });

					Vec3.clear();
				}
				else if (line.find("golem") != string::npos)
				{
					CGolemEnemy::Create({ Vec3[0],Vec3[1],Vec3[2] });

					Vec3.clear();
				}
			}

		}
	}
}

//===========================================================================================================
// モデルファイルデータの読み込み
//===========================================================================================================
void CLoad::ModelFiledata()
{
	using namespace std;

	CModeldata* pModeldata = CManager::GetModeldata();	// モデルデータのポインタを取得する
	CModeldata::MODELFILEPASS modelfiledata;	// モデルファイルデータの格納変数

	string saveloadtxt = "data\\text\\test.txt";	//テキストファイルのパス

	ifstream file(saveloadtxt);	// ファイルの読み込み
	string line, name, filepass;	// 読み込んだ文字を代入する

	if (!file)
	{// ファイルが存在しなかった場合

		if (CScene::GetMode() != CScene::MODE_EDITOR)
		{// エディター以外の時中止させる
			assert(0);
		}
		return;
	}

	// モデルデータの番号
	int dataIdx = 0;

	while (getline(file, line, '='))
	{// 一列ずつ読み取る
		if (line.find("NAME") != string::npos)
		{
			getline(file, name);	// GetLine で = 以降に進める
			modelfiledata.Name = name;	// dataの変数に代入
		}
		if (line.find("FILE_PASS") != string::npos)
		{
			getline(file, filepass);
			modelfiledata.Filepass = filepass;	// dataの変数に代入

			pModeldata->SetModelFilepass(modelfiledata.Name, modelfiledata.Filepass,CModeldata::ADD_DATA_READING);

			// 読み込んだデータを設定する
			//pModeldata->SetModelFilePassData(modelfiledata, dataIdx);
			dataIdx++;	// 番号を進める
		}
	}
}
