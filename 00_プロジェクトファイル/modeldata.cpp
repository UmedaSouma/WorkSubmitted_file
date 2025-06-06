//===========================================================================================================================================================
// 
// ���f���f�[�^�̊Ǘ�
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "modeldata.h"
#include "manager.h"
#include "save.h"
#include "load.h"

int CModeldata::m_NumAll = 0;

//========================================================================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//========================================================================================================================
CModeldata::~CModeldata()
{
}

//========================================================================================================================
// �j��
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
// ���f���f�[�^�̓o�^
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
			// X�t�@�C���̓ǂݍ���
			if (SUCCEEDED(D3DXLoadMeshFromX(modelpass,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				nullptr,
				&m_pModelData[nCnt].BuffMat,
				nullptr,
				&m_pModelData[nCnt].NumMat,
				&m_pModelData[nCnt].Mesh)))
			{
				D3DXMATERIAL* pMat;				// �}�e���A���f�[�^�̃|�C���^
				pMat = (D3DXMATERIAL*)m_pModelData[nCnt].BuffMat->GetBufferPointer();	// �}�e���A���f�[�^�̃|�C���^�̎擾

				for (int nCntMat = 0; nCntMat < (int)m_pModelData[nCnt].NumMat; nCntMat++)
				{
					//if (pMat[nCntMat].pTextureFilename != nullptr)
					//{
					//	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &m_pTexData[nCnt].pTex[nCntMat])))	// �e�N�X�`���쐬
					//	{

					//	}
					//}

					m_StandardCol[nCnt][nCntMat] = pMat[nCntMat].MatD3D.Diffuse;	// ���̐F��ۑ����Ă���
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
			// �����Ō��̐F��ۑ����Ă���
			// SET or GET �łǂ��ɂ�����
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
// �A�h���X��Ԃ�
//========================================================================================================================
CModeldata::ModelData *CModeldata::GetAddress(int Idx)
{
	return &m_pModelData[Idx];
}

//===========================================================================================================
// �ʏ�F�̃}�e���A���f�[�^��Ԃ�
//===========================================================================================================
D3DCOLORVALUE *CModeldata::GetMatData(const char* pAddress, int MatNum)
{
	for (int nCnt = 0; nCnt < MAX_MODEL_DATA; nCnt++)
	{
		if (!strcmp(pAddress, m_pStorage[nCnt]))
		{// �A�h���X�������ԍ���������
			return &m_StandardCol[nCnt][MatNum];	// �ʏ�F��Ԃ�
		}
	}

	return nullptr;
}

//===========================================================================================================
// ���f���t�@�C���p�X�̏���������
//===========================================================================================================
void CModeldata::LoadModelFilePass()
{
	CLoad::ModelFiledata();
}

//===========================================================================================================
// �t�@�C���p�X�̐ݒ�
//===========================================================================================================
std::string CModeldata::SetModelFilepass(std::string name, std::string address, ADD_DATA_MODE mode)
{	
	ADD_RESULT result = ADD_RESULT_NONE;

	for (int i = 0; i < MAX_MODEL_DATA; i++)
	{
		if (m_ModelFile[i].Name != "")	// ���g����������
			continue;

		result = CompareWithExistingNames(name, address, i, mode);	// ���O�̔�r
		break;
	}

	return m_AddResultText[result];
}

//===========================================================================================================
// ���O�̔�r
//===========================================================================================================
CModeldata::ADD_RESULT CModeldata::CompareWithExistingNames(std::string name, std::string address, int Idx, ADD_DATA_MODE mode)
{
	ADD_RESULT result;

	for (int i = Idx; i >= 0; i--)
	{
		//if (strcmp(m_ModelFile[i].Name, name) == 0)	// ���O�����Ԃ��Ă����� ��return
		//	return;

		if (m_ModelFile[i].Name == name)
			return ADD_RESULT_FAILURE_SAME_NAME;
	}

	result = CompareWithExistingAddress(name, address, Idx, mode);	// �t�@�C���p�X�̔�r

	return result;
}

//===========================================================================================================
// �t�@�C���p�X�̔�r
//===========================================================================================================
CModeldata::ADD_RESULT CModeldata::CompareWithExistingAddress(std::string name, std::string address, int Idx, ADD_DATA_MODE mode)
{
	for (int i = Idx; i >= 0; i--)
	{
		//if (strcmp(m_ModelFile[i].Filepass, address) == 0)	// �t�@�C���p�X�����Ԃ��Ă����� ��return
		//	return;

		if (m_ModelFile[i].Filepass == address)
			return ADD_RESULT_FAILURE_SAME_FILEPASS;
	}

	// �����o�ϐ��ɓo�^����
	m_ModelFile[Idx].Name = name;
	m_ModelFile[Idx].Filepass = address;

	if (mode == ADD_DATA_MODE::ADD_DATA_WRITING)
	{
		// �e�L�X�g�t�@�C���ɏ����o��
		CSave::AddModeFilepass(name.c_str(), address.c_str());

		return ADD_RESULT_SUCCESS;
	}

	m_MaxFilepassIdx = Idx;	// MAXIDX��ݒ肷��

	return ADD_RESULT_NONE;
}
