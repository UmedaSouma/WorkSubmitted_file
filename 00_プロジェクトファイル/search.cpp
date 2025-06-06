#include "search.h"

CSearch::CSearch()
{
}

CSearch::~CSearch()
{
}

CPlayer3D* CSearch::SearchPlayer()
{
	CPlayer3D* pPlayer = nullptr;

	for (int nCntPrio = 0; nCntPrio < CObject::PRIORITY_MAX; nCntPrio++)
	{
		for (int nCntObj = 0; nCntObj < CObject::MAX_OBJECT; nCntObj++)
		{
			CObject* pObj = CObject::GetOBJECT(nCntPrio, nCntObj);
			if (pObj != nullptr&&
				pObj->GetType() == CObject::TYPE::PLAYER
				)
			{
				pPlayer = (CPlayer3D*)pObj;

				return pPlayer;
			}
		}
	}

	return nullptr;
}

