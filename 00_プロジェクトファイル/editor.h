//===========================================================================================================================================================
// 
// editor.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _EDITOR_H_
#define _EDITOR_H_
#include "main.h"
#include "scene.h"

class CEditor :public CScene
{
public:
	CEditor();
	~CEditor();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	
	void InputEdit(D3DXVECTOR3& pos);	// �G�f�B�^�[�̓��͏���
	void InputBaseObj(D3DXVECTOR3& pos);	// �x�[�X�I�u�W�F�N�g�̓��͏���

	void InputMove(D3DXVECTOR3& pos);	// �ړ�����
	void InputKeyboadMove(D3DXVECTOR3& pos, int mag);	// �L�[�{�[�h�̈ړ�����
	
	void CreateEdit(const D3DXVECTOR3 pos);	// �u���b�N�𐶐�
	void DestroyEdit(const D3DXVECTOR3 pos);	// �u���b�N������

	void UpdateSetModel(int type);	// ���f�����Z�b�g
	void SelectObject();	// �I�u�W�F�N�g��I��

	static int GetModelType() { return m_type; }	// ���f���̎�ނ�I��
private:
	static CModel* pBaseObj;	// �S�Ẵx�[�X�A���̃x�[�X����ɓ���
	static int m_type;
	static int m_old_type;	// ��ׂ邽�߂̌Â��^�C�v
	bool IsOpenBaseObjWindow;	// �x�[�X�I�u�W�F�N�g�̃E�B���h�E���o��

};



#endif // !EDITOR_H_