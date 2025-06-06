//===========================================================================================================================================================
// 
// fragment_manager.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _FRAGMENT_MANAGER_H_
#define _FRAGMENT_MANAGER_H_
#include "main.h"
#include "enemy3D.h"

class CFragmentManager
{
public:
	CFragmentManager();
	~CFragmentManager();
	void CreateMessenger();	// �g��G����������
private:
	int m_FragmentNum[CEnemy3D::TAG_ENEMY::TAG_MAX];	// �^�O���ƂɌ��А����Ǘ�����ϐ�
	bool m_bIsAppear[CEnemy3D::TAG_ENEMY::TAG_MAX];

public: 
	void SetAppear(CEnemy3D::TAG_ENEMY tag) { m_bIsAppear[tag] = true; }
	bool GetAppear(CEnemy3D::TAG_ENEMY tag) { return m_bIsAppear[tag]; }
	int GetFragmentNum(CEnemy3D::TAG_ENEMY tag) { return m_FragmentNum[tag]; }	// �^�O���Ƃɐ����擾���鏈��
	void AddFragmentNum(CEnemy3D::TAG_ENEMY tag) { m_FragmentNum[tag]++; }	// �^�O���Q�Ƃ��ď������𑝂₷
};




#endif // !FRAGMENT_MANAGER_H_