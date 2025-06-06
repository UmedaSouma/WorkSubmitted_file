//===========================================================================================================================================================
// 
// �x�[�X�I�u�W�F�N�g�̃E�B���h�E [edit_window_baseobj.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "edit_window_baseobj.h"
#include "ImGUI_manager.h"
#include "manager.h"
#include "calculation.h"

//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
EditWindowBaseobj::EditWindowBaseobj()
{
	m_MoveNum = 1;
	m_move = { 0,0,0 };
	m_IsOpenAddtypeWindow = true;
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
EditWindowBaseobj::~EditWindowBaseobj()
{
}

//===========================================================================================================
// �n��
//===========================================================================================================
void EditWindowBaseobj::Begin()
{
	ImGui::Begin("BaseObject Editer");
}

//===========================================================================================================
// �I��
//===========================================================================================================
void EditWindowBaseobj::End()
{
	ImGui::End();
}

//===========================================================================================================
// �^�C�v�I��
//===========================================================================================================
int EditWindowBaseobj::SelectType(int& type)
{
	static int i = 0;

	// ���f���t�@�C���p�X�̏��̎擾
	int MaxIdx = CManager::GetModeldata()->GetMaxFilepassIdx();
	CModeldata::MODELFILEPASS* modelfilepass = CManager::GetModeldata()->GetModelFilePassData();
	const char* DisplayName[128] = {};	// �\�������i�[����ϐ�

	// �\������ݒ肷��
	for (int n = 0; n <= MaxIdx; n++)
	{
		DisplayName[n] = modelfilepass[n].Name.c_str();
	}

	if (ImGui::CollapsingHeader("TYPE"))
	{
		// �I�u�W�F�N�g���X�g
		if (ImGui::ListBox("type", &i, DisplayName, (MaxIdx+1), 4))
		{
			type = i + 1;
		}

		// �t�@�C���ǉ����o��
		ImGui::SeparatorText("AddFile");

		// ���f���t�@�C���ǉ��E�B���h�E
		DrawAddModelFileWindow();
	}

	return type;
}

//===========================================================================================================
// �ړ�
//===========================================================================================================
int EditWindowBaseobj::MoveBar(D3DXVECTOR3& move)
{
	ImGui::SliderInt("max move", &m_MoveNum, 1, m_MaxMoveSlider);

	// �X���C�_�[���傫�����l����͂��Ă��܂����Ƃ�
	if (m_MoveNum > m_MaxMoveSlider)
	{
		m_MoveNum = m_MaxMoveSlider;
	}

	// 0 �ȉ�����͂��Ă��܂����ꍇ
	else if (m_MoveNum < 1)
	{
		m_MoveNum = 1;
	}

	// �h���b�O�ňʒu�𓮂���
	ImGui::DragFloat3(
		"drag float3",
		move,
		1.f,
		-(float)m_MoveNum,
		(float)m_MoveNum,
		"%.3f",
		ImGuiSliderFlags_NoInput
	);

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		for (int i = 0; i < 3; i++)
		{
			move = { 0,0,0 };
		}
	}

	return m_MoveNum;
}

//===========================================================================================================
// �ʒu�𒼐ړ��͂���
//===========================================================================================================
void EditWindowBaseobj::PosNum(D3DXVECTOR3& pos)
{
	// pos �𒼐ړ���锠
	ImGui::InputFloat("pos X", &pos.x, (float)m_MoveNum, 0, "%.3f");
	ImGui::InputFloat("pos Y", &pos.y, (float)m_MoveNum, 0, "%.3f");
	ImGui::InputFloat("pos Z", &pos.z, (float)m_MoveNum, 0, "%.3f");
}

//===========================================================================================================
// �g�嗦�𒼐ړ��͂���
//===========================================================================================================
void EditWindowBaseobj::ScaleNum(D3DXVECTOR3& scale)
{
	if (ImGui::CollapsingHeader("SCALE"))
	{// �ړ������ׂĂ܂Ƃ߂��w�b�_�[

		// scale �𒼐ړ���锠
		ImGui::InputFloat("scale X", &scale.x, 0.1f, 0, "%.1f");
		ImGui::InputFloat("scale Y", &scale.y, 0.1f, 0, "%.1f");
		ImGui::InputFloat("scale Z", &scale.z, 0.1f, 0, "%.1f");
	}

	// �}�C�i�X������鎞
	if (scale.x <= 0.0f)
	{
		scale.x = 0.1f;
	}
	if (scale.y <= 0.0f)
	{		  
		scale.y = 0.1f;
	}
	if (scale.z <= 0.0f)
	{
		scale.z = 0.1f;
	}
}

//===========================================================================================================
// �p�x�𒼐ړ��͂���
//===========================================================================================================
void EditWindowBaseobj::RotNum(D3DXVECTOR3& rot)
{
	if (ImGui::CollapsingHeader("ROT"))
	{// �ړ������ׂĂ܂Ƃ߂��w�b�_�[

		// rot �𒼐ړ���锠
		ImGui::InputFloat("rot X", &rot.x, 0.1f, 0, "%.2f");
		ImGui::InputFloat("rot Y", &rot.y, 0.1f, 0, "%.2f");
		ImGui::InputFloat("rot Z", &rot.z, 0.1f, 0, "%.2f");
	}

	// �J�����p�x���Z�b�g
	CCalculation::ResetRot(rot.x);
	CCalculation::ResetRot(rot.y);
	CCalculation::ResetRot(rot.z);

	//// �J�������������Ɗp�x�����Z�b�g����
	//if (rot.y > D3DX_PI /*|| rot.y <= -D3DX_PI*/)
	//{
	//	rot.y = -D3DX_PI;
	//}
	//if (rot.y < D3DX_PI)
	//{

	//}
}

//===========================================================================================================
// ���f���t�@�C���̒ǉ��E�B���h�E
//===========================================================================================================
void EditWindowBaseobj::DrawAddModelFileWindow()
{
	static char IdName[128] = "ID NAME";	// ���f���̖��O
	char *filepass[128];	// �t�@�C���p�X

	filepass[0] = ImguiManager::GetEditMain()->GetDropFilename();	// �t�@�C���p�X���擾����

	ImGui::InputText("file name", filepass[0], IM_ARRAYSIZE(filepass));	// �t�@�C���p�X��\��

	ImGui::InputText("ID_NAME", IdName, IM_ARRAYSIZE(IdName));	// ���O����

	if (ImGui::Button("ADD", { 200,20 }))	// �ǉ��{�^��
	{
		// ���f���t�@�C���p�X�f�[�^�������o��
		m_AddResult = CManager::GetModeldata()->SetModelFilepass(IdName, filepass[0],CModeldata::ADD_DATA_WRITING);
	}

	ImGui::Text(m_AddResult.c_str());	// ���ʂ��e�L�X�g�ɕ\������
}
