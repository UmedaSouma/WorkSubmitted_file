#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include "main.h"

//====================================================================================
// ���f���ɕK�v�ȏ��̍\����
//====================================================================================

// ���f���f�[�^�̃N���X
class CModeldata
{
public:
//===========================================================================================================
// �񋓌^
//===========================================================================================================
	// ���f���t�@�C���f�[�^�ǉ����̃��[�h
	typedef enum
	{
		ADD_DATA_WRITING = 0,	// �����o��
		ADD_DATA_READING,		// �ǂݍ���
	}ADD_DATA_MODE;

	// ���f���ǉ����̌���
	typedef enum
	{
		ADD_RESULT_NONE = 0,				// �����N����Ȃ�(�ǂݍ��ݎ�
		ADD_RESULT_SUCCESS,					// �ǉ�����
		ADD_RESULT_FAILURE_SAME_NAME,		// �ǉ����s(���O���ꏏ)
		ADD_RESULT_FAILURE_SAME_FILEPASS,	// �ǉ����s(�t�@�C���p�X���ꏏ)
		ADD_RESULT_MAX
	}ADD_RESULT;

//===========================================================================================================
// �萔�l
//===========================================================================================================
	static const unsigned int MAX_MODEL_DATA = 64;	// ���f���f�[�^�̍ő�l
	static const unsigned int MAX_MAT_DATA = 514;	// �P�L�����N�^�[���̃}�e���A���f�[�^�ő�l

	// �ǉ��{�^�������������ʂ̃e�L�X�g
	std::string m_AddResultText[ADD_RESULT_MAX] = {
		"none",
		"�ǉ�����������(add success)",
		"ID�̖��O���ꏏ���������ߒǉ������s����(add failure same name)",
		"�t�@�C���p�X���ꏏ���������ߒǉ������s����(add failure same filepass)",
	};

//===========================================================================================================
// �\����
//===========================================================================================================
	// ���f���t�@�C���̏��
	typedef struct
	{
		LPD3DXMESH Mesh;	// ���b�V��(���_���)�ւ̃|�C���^
		LPD3DXBUFFER BuffMat;	// �}�e���A���ւ̃|�C���^
		DWORD NumMat;	// �}�e���A���̐�
	}ModelData;

	// ���f���t�@�C���p�X�̃f�[�^
	typedef struct
	{
		std::string Name;	// �t�@�C����IDname
		std::string Filepass;	// �t�@�C���̃p�X
	}MODELFILEPASS;

	// �e�N�X�`���f�[�^
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTex[MAX_MAT_DATA];
		int nIdx;
	}TexData;

	CModeldata();	// �R���X�g���N�^
	~CModeldata();	// �f�X�g���N�^
	void Unload();	// 
	int Regist(const char* pTexturename);
	ModelData *GetAddress(int Idx);
	D3DCOLORVALUE *GetMatData(const char* pAddress, int MatNum);

	void LoadModelFilePass();	// ���f���t�@�C���p�X�̏�����
	std::string SetModelFilepass(std::string name, std::string address, ADD_DATA_MODE mode);	// �t�@�C���p�X��ݒ�
	ADD_RESULT CompareWithExistingNames(std::string name, std::string address, int Idx, ADD_DATA_MODE mode);	// ���O�Ƃ̔�r
	ADD_RESULT CompareWithExistingAddress(std::string name, std::string address, int Idx, ADD_DATA_MODE mode);	// �t�@�C���p�X�̔�r

	void SetModelFilePassData(MODELFILEPASS pData, int Idx) { m_ModelFile[Idx] = pData; }	// ���f���t�@�C���p�X�f�[�^�̐ݒ�
	MODELFILEPASS* GetModelFilePassData() { return &m_ModelFile[0]; }	// ���f���t�@�C���p�X�f�[�^�̎擾
	int GetMaxFilepassIdx() { return m_MaxFilepassIdx; }	// ���f���t�@�C���p�X�̃C���f�b�N�X�̎擾
private:
	
	TexData m_pTexData[MAX_MODEL_DATA];
	ModelData m_pModelData[MAX_MODEL_DATA];
	D3DCOLORVALUE m_StandardCol[MAX_MODEL_DATA][MAX_MAT_DATA];	// �F��ς���O�̌��̐F
	MODELFILEPASS m_ModelFile[MAX_MODEL_DATA];	// ���f���t�@�C���̃t�@�C���p�X�̃f�[�^
	int m_MaxFilepassIdx;	// ���f���t�@�C���p�X�̃C���f�b�N�X

	const char* m_pStorage[MAX_MODEL_DATA];
	static int m_NumAll;
	

	//LPD3DXMESH Mesh;			// ���b�V��(���_���)�ւ̃|�C���^
	//LPD3DXBUFFER BuffMat;	// �}�e���A���ւ̃|�C���^
	//DWORD NumMat;	// �}�e���A���̐�
};

#endif // !_MODELDATA_H_
