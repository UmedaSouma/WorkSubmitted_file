#include "object.h"

//************************************************
// < debugmemo >
// for_each �̎c�[
//************************************************
//#include <algorithm>

// �ÓI�����o�ϐ��̏�����
int CObject::m_nNumAll = 0;				// �I�u�W�F�N�g����
CObject* CObject::m_apObject[PRIORITY_MAX][MAX_OBJECT] = {};	// �I�u�W�F�N�g�̊Ǘ�
CObject* CObject::m_pObjectList = nullptr;
CObject* CObject::m_pTop[PRIORITY_MAX] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };
CObject* CObject::m_pCur[PRIORITY_MAX] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };

CObject::CObject(int nPriority)
{// �������g�̓o�^

	if (nPriority == 4)
	{
		int n = 0;
	}

	//for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		if (m_pTop[nPriority] == nullptr)
		{// top ���ݒ肳��Ă��Ȃ�������
			m_pPrev = nullptr;	// �O�̃|�C���^��ݒ�(�ŏ��̂��� nullptr
			m_pNext = nullptr;	// ���̃|�C���^��ݒ�(�ŏ��̂��� nullptr
			m_pTop[nPriority] = this;		// �ŏ��̃|�C���^�Ƃ��Đݒ�
			m_pCur[nPriority] = this;		// �Ō���̃|�C���^�Ƃ��Đݒ�
		}
		else
		{
			m_pNext = nullptr;			// ���̃|�C���^���k���ɂ��Ă���
			m_pPrev = m_pCur[nPriority];			// ���̎��_�ł̍Ō����O�̃|�C���^�ɐݒ肷��
			m_pCur[nPriority] = this;				// �Ō�������g�ɐݒ肷��
			m_pPrev->m_pNext = this;	// �O�̃|�C���^�̎��̃|�C���^�Ɏ��g��ݒ肷��
		}
	}
	
	m_nNumAll++;				// �I�u�W�F�N�g�̑����𑝂₷
	m_nPriority = nPriority;
	m_type = TYPE::NONE;
	m_bRelese = false;


	//for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//{
	//	if (m_apObject[nPriority][nCnt] == nullptr)
	//	{
	//		m_apObject[nPriority][nCnt] = this;	// �������g����
	//		m_nID = nCnt;				// �������g��ID����
	//		m_nNumAll++;				// �I�u�W�F�N�g�̑����𑝂₷
	//		m_nPriority = nPriority;
	//		m_type = TYPE::NONE;
	//		m_bRelese = false;

	//		break;
	//	}
	//}
}

CObject::~CObject()
{

}

HRESULT CObject::Init()
{
	// �������z�֐�

	return S_OK;
}

void CObject::Uninit()
{
	// �������z�֐�
}

void CObject::Update()
{
	// �������z�֐�
}

void CObject::Draw()
{
	// �������z�֐�
}

void CObject::Release()
{
	//int nID = m_nID;
	//int nPrio = m_nPriority;

	m_bRelese = true;

	//if (m_apObject[nPrio][nID] != nullptr)
	//{
	//	delete m_apObject[nPrio][nID];
	//	m_apObject[nPrio][nID] = nullptr;
	//	m_nNumAll--;
	//}
}

void CObject::ReleaseAll()
{

	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			if (pObj->m_type != FADE)
			{
				pObj->Release();
			}
			pObj = pNext;
		}
	}
	

	//// �S�Ă�������鏈��
	//for (int nCntPrio = 0; nCntPrio < PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//	{
	//		if (m_apObject[nCntPrio][nCnt] != nullptr)
	//		{
	//			if (m_apObject[nCntPrio][nCnt]->m_type != FADE)
	//			{
	//				m_apObject[nCntPrio][nCnt]->Uninit();
	//				delete m_apObject[nCntPrio][nCnt];
	//				m_apObject[nCntPrio][nCnt] = nullptr;
	//			}
	//			
	//		}
	//	}
	//}
}

void CObject::DeathAll()
{
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;
			if (pObj->m_bRelese)
			{

				// ���ꂼ��̎��g�̗����ʒu�ɂ���ĂȂ�����ς���
				if (pObj == m_pTop[nPrio] && pObj == m_pCur[nPrio])
				{ // �����ɓ������ꍇ�Ō�̃I�u�W�F�N�g�Ƃ݂Ȃ��A���̂܂܉��̏����Ɏ����Ă���
					m_pTop[nPrio] = nullptr;
					m_pCur[nPrio] = nullptr;
				}
				else if (pObj == m_pTop[nPrio])
				{
					m_pTop[nPrio] = pObj->m_pNext;	// ���g�̎��̃|�C���^�� top �ɐݒ肷��
					pObj->m_pNext->m_pPrev = nullptr; // ���̃|�C���^�� prev �� nullptr �ɂ���
				}
				else if (pObj == m_pCur[nPrio])
				{
					m_pCur[nPrio] = pObj->m_pPrev;	// ���g�̑O�̃|�C���^�� cur �ɐݒ肷��
					pObj->m_pPrev->m_pNext = nullptr;	// �O�̃|�C���^�� next �� nullptr �ɂ���
				}
				else
				{
					pObj->m_pPrev->m_pNext = pObj->m_pNext;	// �O�̃|�C���^�� Next �Ɏ��g�̎��̃|�C���^�̏�������
					pObj->m_pNext->m_pPrev = pObj->m_pPrev;	// ���̃|�C���^�� Prev �Ɏ��g�̑O�̃|�C���^�̏������
				}

				pObj->Uninit();
				delete pObj;
				pObj = nullptr;

			}
			pObj = pNext;
		}
	}

	
}

void CObject::Death()
{
	delete this;
}

void CObject::UpdateAll()
{
	// �z��ȊO��
	{
		//for (unsigned int n = 0; n < m_pObjectList.size(); n++)
	//{
	//	m_pObjectList
	//}

	//CObject* pObj = m_pObjectList.front();

	//std::for_each(m_pObjectList.begin(), m_pObjectList.end(), [](CObject* obj) {obj->GetType(); })


	//***************************************************
	// < debugmemo >
	// iterator�ŊǗ����悤�Ƃ���
	//***************************************************
	//std::list<CObject*>::iterator itr;
	//for (itr = m_pObjectList.begin(); itr != m_pObjectList.end(); itr++)
	//{

	//}
	}
	
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;

			if (pObj->GetType() != TYPE::ENEMY_MESSE)
			{
				pObj->Update();
			}

			pObj = pNext;
		}
	}

	DeathAll();	// ���S�t���O�������� object �� delete ����
	
	//for (int nCntPrio = 0; nCntPrio < PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//	{
	//		if (m_apObject[nCntPrio][nCnt] != nullptr)
	//		{
	//			
	//			m_apObject[nCntPrio][nCnt]->Update();
	//		}
	//	}
	//}
}

void CObject::DrawAll()
{
	for (int nPrio = 0; nPrio < PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = m_pTop[nPrio];

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;

			//if (pObj->m_type == FADE)
			//{}
			if (pObj->m_type == ENEMY_MESSE)
			{
			}
			else
			{
				pObj->Draw();
			}

			pObj = pNext;
		}
	}
	


	//for (int nCntPrio = 0; nCntPrio < PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	//	{
	//		if (m_apObject[nCntPrio][nCnt] != nullptr)
	//		{
	//			m_apObject[nCntPrio][nCnt]->Draw();
	//		}
	//	}
	//}
}

CObject* CObject::GetOBJECT(int nPrio, int nIdx)
{
	//return m_apObject[nPrio][nIdx];

	return nullptr;
}

CObject::TYPE CObject::GetType()
{
	return m_type;
}

void CObject::SetType(TYPE type)
{
	m_type = type;
}
