//===========================================================================================================================================================
// 
// ������Z�[�u�������ł��� [save.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include <fstream>

#include "search.h"
#include "save.h"
#include "model.h"


//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
CSave::CSave()
{
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
CSave::~CSave()
{
}

//===========================================================================================================
// �X�e�[�W�������ݏ���
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
			{// �I�u�W�F�N�g��T��

				CObject* pNext = pObj->GetNext();

				pModel = CSearch::SearchType(pModel, pObj);	// �^�C�v���Z�b�g����

				str = SetSaveTypeText((int)pModel->GetModelType());	// ���f���̃^�C�v���擾����

				// �S�~������̂œ��ڈȍ~����f�[�^������
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
// ���f���̃t�@�C���p�X�ǉ�
//===========================================================================================================
void CSave::AddModeFilepass(const char* name, const char* address)
{
	using namespace std;

	string text = "data\\text\\test.txt";	// �e�L�X�g�t�@�C���̃p�X
	ofstream file(text,ios::app);	// �ǋL����`��

	if (file.is_open())
	{// �t�@�C��������ɊJ������

		// ���O�ƃt�@�C���p�X�������o��
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
// �^�C�v��ݒ肵�ď�������
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