//===========================================================================================================================================================
// 
// ��������̂�ǂݍ��� [load.cpp]
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
// �R���X�g���N�^
//===========================================================================================================
CLoad::CLoad()
{
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
CLoad::~CLoad()
{
}

//===========================================================================================================
// �X�e�[�W�ǂݍ��ݏ���
//===========================================================================================================
void CLoad::LoadText()
{
	std::string saveloadtxt = "data\\text\\stage01.txt";

	std::ifstream file(saveloadtxt);
	std::string line;

	//****************************************************************
	// < debugmemo >
	// �t�@�C���ǂݍ��݈�s�R�[�h
	// ��X�����\��
	//****************************************************************

	if (!file)
	{// �t�@�C�������݂��Ȃ������ꍇ

		if (CScene::GetMode() != CScene::MODE_EDITOR)
		{// �G�f�B�^�[�ȊO�̎����~������
			assert(0);
		}

		return;
	}

	while (std::getline(file, line))
	{// ��񂸂ǂݎ���Ă���

		if (line == "SCRIPT")
		{// �t�@�C���̕����Ɣ�r
			StageText(file);
		}
	}
}



void CLoad::StageText(std::ifstream& file)
{
	using namespace std;

	vector<float> Vec3;	// vector3 �^�̐��l������ϐ�
	float f = 0.0f;	// �擾�������l���ꎞ�i�[����ϐ�
	string line;	// �t�@�C������ǂݎ���������������ϐ�

	//while (getline(file, line))
	{
		while (getline(file, line, '='))
		{

			if (line.find("pos") != string::npos)
			{
				getline(file, line);	// GetLine �� = �ȍ~�ɐi�߂�

				stringstream ss{ line };	// Getline �Ŏg����悤�� stringstream �Ɍ��݂̕������������

				// �R���}�ŋ�؂��ď����擾����
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
// ���f���t�@�C���f�[�^�̓ǂݍ���
//===========================================================================================================
void CLoad::ModelFiledata()
{
	using namespace std;

	CModeldata* pModeldata = CManager::GetModeldata();	// ���f���f�[�^�̃|�C���^���擾����
	CModeldata::MODELFILEPASS modelfiledata;	// ���f���t�@�C���f�[�^�̊i�[�ϐ�

	string saveloadtxt = "data\\text\\test.txt";	//�e�L�X�g�t�@�C���̃p�X

	ifstream file(saveloadtxt);	// �t�@�C���̓ǂݍ���
	string line, name, filepass;	// �ǂݍ��񂾕�����������

	if (!file)
	{// �t�@�C�������݂��Ȃ������ꍇ

		if (CScene::GetMode() != CScene::MODE_EDITOR)
		{// �G�f�B�^�[�ȊO�̎����~������
			assert(0);
		}
		return;
	}

	// ���f���f�[�^�̔ԍ�
	int dataIdx = 0;

	while (getline(file, line, '='))
	{// ��񂸂ǂݎ��
		if (line.find("NAME") != string::npos)
		{
			getline(file, name);	// GetLine �� = �ȍ~�ɐi�߂�
			modelfiledata.Name = name;	// data�̕ϐ��ɑ��
		}
		if (line.find("FILE_PASS") != string::npos)
		{
			getline(file, filepass);
			modelfiledata.Filepass = filepass;	// data�̕ϐ��ɑ��

			pModeldata->SetModelFilepass(modelfiledata.Name, modelfiledata.Filepass,CModeldata::ADD_DATA_READING);

			// �ǂݍ��񂾃f�[�^��ݒ肷��
			//pModeldata->SetModelFilePassData(modelfiledata, dataIdx);
			dataIdx++;	// �ԍ���i�߂�
		}
	}
}
