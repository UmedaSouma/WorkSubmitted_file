//===========================================================================================================================================================
// 
// �X�e�[�W�ҏW�c�[���̏��
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "manager.h"
#include "search.h"
#include "input.h"
#include "camera.h"
#include "editor.h"
#include "fade.h"

#include "player3D.h"
#include "grass3D.h"
#include "slime_enemy.h"
#include "stone.h"
#include "start.h"
#include "goal.h"
#include "slime_enemy.h"
#include "golem_enemy.h"

#include "load.h"
#include "save.h"

#include "nlohmann/test.hpp"

#include < fstream >

// �ÓI�����o�ϐ��̏�����
CModel* CEditor::pBaseObj = nullptr;
int CEditor::m_type = CModel::MODEL_TYPE::NORMAL;
int CEditor::m_old_type = CModel::MODEL_TYPE::NORMAL;

//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
CEditor::CEditor()
{
	IsOpenBaseObjWindow = false;
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
CEditor::~CEditor()
{
}

//===========================================================================================================
// �����ݒ�
//===========================================================================================================
HRESULT CEditor::Init()
{

	//std::ifstream stream("nlohmann\\test.hpp");

	//if(!stream.is_open())
	//	throw new std::exception("Failed open file.");

	//if (!json::accept(stream))
	//	throw new std::exception("json�̃t�H�[�}�b�g���s��");

	//// json::accept���t�H�[�}�b�g�`�F�b�N����position��i�߂Ă��܂��̂ŁA�擪�ɖ߂�
	//stream.seekg(0, std::ios::beg);

	//json j = json::parse(stream);

	//auto result = j.get<test>();

	pBaseObj = new CModel;

	pBaseObj->SetPos({ 0.0f,0.0f,0.0f });
	pBaseObj->SetModel(CModel::MODEL_TYPE::GRASS);
	pBaseObj->SetType(CObject::BASE);
	pBaseObj->Init();

	// ���f���^�C�v�̐ݒ�
	m_type = CModel::MODEL_TYPE::GRASS;
	m_old_type = m_type;	// ���݂ƌÂ��^�C�v�𓝈ꂳ����

	// ���f���t�@�C���f�[�^�̓ǂݍ���

	CLoad::LoadText();
	CManager::GetModeldata()->LoadModelFilePass();	// ���f���t�@�C���p�X
	
	CScene::Init();

	return S_OK;
}

//===========================================================================================================
// �I������
//===========================================================================================================
void CEditor::Uninit()
{
	if (pBaseObj != nullptr)
	{
		pBaseObj->Uninit();
	}

	CScene::Uninit();
}

//===========================================================================================================
// �X�V����
//===========================================================================================================
void CEditor::Update()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	CFade* pFade = CManager::GetFade();

	D3DXVECTOR3 pos = pBaseObj->GetPos();

	UpdateSetModel(m_type);
	
	InputEdit(pos);
	
	pBaseObj->SetPos(pos);

	CCamera* pCamera = CManager::GetCamera();	// camera �������Ă���
	pCamera->SetTargetPos(pos);			// camera �� object ������
	
	CScene::Update();

	if (keyboard->GetTrigger(DIK_F3))
	{
		pFade->SetFade(CScene::MODE_GAME);
	}
}

//===========================================================================================================
// �`�揈��
//===========================================================================================================
void CEditor::Draw()
{
	CScene::Draw();
}

//===========================================================================================================
// �G�f�B�^�[�̓��͏���
//===========================================================================================================
void CEditor::InputEdit(D3DXVECTOR3 &pos)
{
	CFade* pFade = CManager::GetFade();

	// �E�B���h�E�̓�����󂯎��ϐ�
	EditWindowMain::WindowAction w_act = EditWindowMain::W_ACT_NONE;

	// Edit Main �������Ă���
	EditWindowMain* pEditmain = CManager::GetImguiManager()->GetEditMain();
	w_act = pEditmain->DrawWindow();	// MainWindow�̕`��

	CInputKeyBoard* keyboard = CManager::GetKeyboard();	// �L�[�{�[�h�̎擾

	// �X�e�[�W���������o��
	if (keyboard->GetTrigger(DIK_F1) || w_act == EditWindowMain::W_ACT_SAVE)
	{// F1�������ꂽ�Ƃ��A�܂��� ImGUI �̃N���G�C�g�{�^���������ꂽ�Ƃ�

		CSave::StageText();
	}

	// �X�e�[�W����ǂݍ���
	if (w_act == EditWindowMain::W_ACT_LOAD)
	{// ImGUI �̃��[�h�{�^���������ꂽ�Ƃ�
		pFade->SetFade(CScene::MODE_EDITOR);
	}
	
	if (w_act == EditWindowMain::W_ACT_OPEN_BLOCK)
	{
		IsOpenBaseObjWindow = true;
	}

	if (IsOpenBaseObjWindow)
	{
		InputBaseObj(pos);
	}

	SelectObject();	// �I�u�W�F�N�g��I��

	// �I�u�W�F�N�g����
	if (keyboard->GetTrigger(DIK_RETURN))
	{
		CreateEdit(pos);
	}
	// �I�u�W�F�N�g�폜
	else if (keyboard->GetTrigger(DIK_BACKSPACE))
	{
		DestroyEdit(pos);
	}

	

#ifdef _DEBUG
#endif // _DEBUG

	//return pos;
}

//===========================================================================================================
// �x�[�X�I�u�W�F�N�g�̑���
//===========================================================================================================
void CEditor::InputBaseObj(D3DXVECTOR3& pos)
{
	// �x�[�X�I�u�W�F�N�g�𓮂����E�B���h�E���擾
	EditWindowBaseobj* pEditBaseobj = CManager::GetImguiManager()->GetEditBase();

	D3DXVECTOR3 scale = pBaseObj->GetScale();
	D3DXVECTOR3 rot = pBaseObj->GetRot();
	
	pEditBaseobj->Begin();	// ������

	pEditBaseobj->SelectType(m_type);	// �x�[�X�I�u�W�F�N�g�̎�ނ�I��

	InputMove(pos);	// �ړ��֘A

	pEditBaseobj->ScaleNum(scale);

	pEditBaseobj->RotNum(rot);

	pBaseObj->SetRot(rot);
	pBaseObj->SetScale(scale);

	//�z�u�������{�^��
	if (ImGui::Button("create"))
		CreateEdit(pos);

	// �X�y�[�X���J����
	ImGui::SameLine(0, 10);

	// �z�u���폜�{�^��
	if (ImGui::Button("destroy"))
		DestroyEdit(pos);

	pEditBaseobj->End();	// ���̏I��
}

//===========================================================================================================
// �ړ�����
//===========================================================================================================
void CEditor::InputMove(D3DXVECTOR3& pos)
{
	// �x�[�X�I�u�W�F�N�g�𓮂����E�B���h�E���擾
	EditWindowBaseobj* pEditBaseobj = CManager::GetImguiManager()->GetEditBase();

	int MoveMag = 1;	// �ړ��{��
	D3DXVECTOR3 move = { 0,0,0 };	// �ړ��l

	if (ImGui::CollapsingHeader("MOVE"))
	{// �ړ������ׂĂ܂Ƃ߂��w�b�_�[

		// bar �� BaseObject �ړ�
		MoveMag = pEditBaseobj->MoveBar(move);
		pos += move;

		// pos ���ړ���
		pEditBaseobj->PosNum(pos);
	}

	// �L�[�{�[�h�ړ�����
	InputKeyboadMove(pos, MoveMag);
}

//===========================================================================================================
// �x�[�X�I�u�W�F�N�g�̃L�[�{�[�h����
//===========================================================================================================
void CEditor::InputKeyboadMove(D3DXVECTOR3& pos,int mag)
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	// �ړ����������߂�
	if (keyboard->GetTrigger(DIK_D))
	{
		move.x = 1;
	}
	else if (keyboard->GetTrigger(DIK_A))
	{
		move.x = -1;
	}
	else if (keyboard->GetTrigger(DIK_W))
	{
		move.y = 1;
	}
	else if (keyboard->GetTrigger(DIK_S))
	{
		move.y = -1;
	}

	// move �� mag(�ړ��{��)�������Ĉړ�������
	pos += move * mag;
}

//===========================================================================================================
// �I�u�W�F�N�g�I������
//===========================================================================================================
void CEditor::SelectObject()
{
	CInputKeyBoard* keyboard = CManager::GetKeyboard();

	// �^�C�v��I��
	if (keyboard->GetTrigger(DIK_E))
	{// �^�C�v�����ɐi�߂�
		m_type++;
	}
	else if (keyboard->GetTrigger(DIK_Q))
	{// �^�C�v��O�ɖ߂�
		m_type--;
	}

	// �^�C�v������̐��𒴂��Ă��܂����ꍇ
	if (m_type == CModel::MODEL_TYPE::MAX)
	{// �^�C�v���ő�ȏ�ɂȂ�����
		m_type = CModel::MODEL_TYPE::NORMAL + 1;	// �ŏ��̃^�C�v�ɖ߂�
	}
	if (m_type == CModel::MODEL_TYPE::NORMAL)
	{// �^�C�v���ŏ��ȉ��ɂȂ�����
		m_type = CModel::MODEL_TYPE::MAX - 1;	// �Ō�̃^�C�v�ɖ߂�
	}
}

//===========================================================================================================
// �^�C�v���Ƃɐ�������
//===========================================================================================================
void CEditor::CreateEdit(const D3DXVECTOR3 pos)
{
	typeid(pBaseObj).name();

	//CObject* pObj = nullptr;

	switch (m_type)
	{
	case CModel::MODEL_TYPE::GRASS:
		CGrass::Create(pos);
		break;

	case CModel::MODEL_TYPE::STONE:
		CStone::Create(pos);
		break;

	case CModel::MODEL_TYPE::START:
		CStart::Create(pos);
		break;

	case CModel::MODEL_TYPE::GOAL:
		CGoal::Create(pos);
		break;

	case CModel::MODEL_TYPE::SLIME:
		CSlimeEnemy::Create(pos);
		break;

	case CModel::MODEL_TYPE::GOLEM:
		CGolemEnemy::Create(pos);
		break;
	}
}

//===========================================================================================================
// �I�u�W�F�N�g��j������
//===========================================================================================================
void CEditor::DestroyEdit(const D3DXVECTOR3 pos)
{
	// �ϐ��錾
	CModel* pModel = nullptr;	// ���f���|�C���^�̏�����

	// �S�I�u�W�F�N�g�̌���
	for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = CObject::GetTop(nPrio);	// �I�u�W�F�N�g�̍ŏ���ݒ肷��
		while (pObj != nullptr)
		{// �I�u�W�F�N�g�����݂������

			CObject* pNext = pObj->GetNext();	// ���̃I�u�W�F�N�g��ݒ肷��
			pModel = CSearch::SearchType(pModel, pObj);	// �^�C�v���Z�b�g����

			if (pos == pModel->GetPos() && pModel->GetType() != CObject::BASE)
			{// �ʒu����v�A�x�[�X�I�u�W�F�N�g�ȊO
				pModel->Uninit();	// �폜
				break;
			}
			// �|�C���^�����̃I�u�W�F�N�g�ɐݒ肷��
			pObj = pNext;
		}
	}
}

//===========================================================================================================
// �x�[�X�̌����ڂ�ς���
//===========================================================================================================
void CEditor::UpdateSetModel(int type)
{
	// �^�C�v���ς���Ă��Ȃ������炱�̏�����ʂ��Ȃ�
	if (m_type == m_old_type)
		return;

	// ���f���̌����ڂ̐ݒ肷��
	CModeldata::MODELFILEPASS* filepassdata = CManager::GetModeldata()->GetModelFilePassData();
	pBaseObj->SetModel(filepassdata[type-1].Filepass.c_str());

	m_old_type = m_type;	// �^�C�v����v������

	//pBaseObj->SetModel((CModel::MODEL_TYPE)type);

}

