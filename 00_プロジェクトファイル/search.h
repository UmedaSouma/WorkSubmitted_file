//===========================================================================================================================================================
// 
// search.cppのヘッダー
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _SEARCH_H_
#define _SEARCH_H_
#include "main.h"
#include "player3D.h"

class CSearch
{
public:
	CSearch();
	~CSearch();
	
private:
	static int m_nCnt;

public:
	static CPlayer3D* SearchPlayer();

	template<typename typeclass>static typeclass* SearchObject(typeclass* Class, CObject::TYPE type);
	template<typename typeclass> static typeclass* SearchType(typeclass* Class, CObject* pObj);
	template<typename typeclass>static std::vector<typeclass*> SearchMultiObject(std::vector<typeclass*>Class, CObject::TYPE type);
	template<typename typeclass> static std::vector<typeclass*> SearchTypeMulti(std::vector<typeclass*> Class, CObject* pObj);
};

//===========================================================================================================
// オブジェクトを探す
//===========================================================================================================
template<typename typeclass> inline typeclass* CSearch::SearchObject(typeclass* Class, CObject::TYPE type)
{
	//===========================================================================================================
	// < debugmemo >
	// 下の処理について
	// うわーーーーーーーーーーーー
	// まだオブジェクト一つ分しか探せなーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーーい
	//===========================================================================================================

	//typeclass ArryClass[100] = {};
	//int nCnt = 0;

	

	for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = CObject::GetTop(nPrio);

		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			if (pObj->GetType() == type)
			{
				Class = SearchType(Class, pObj);	// タイプをセットする
				return Class;
			}

			pObj = pNext;
		}
	}

	
	

	//for (int nCntPrio = 0; nCntPrio < CObject::PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCntObj = 0; nCntObj < CObject::MAX_OBJECT; nCntObj++)
	//	{
	//		CObject* pObj = CObject::GetOBJECT(nCntPrio, nCntObj);

	//		if (pObj != nullptr
	//			&& pObj->GetType() == type)	// オブジェクトが存在して、タイプが一致したとき
	//		{
	//			Class = SearchType(Class, pObj);	// タイプをセットする
	//			//ArryClass[nCnt] = *Class;
	//			//nCnt++;

	//			return Class;
	//		}
	//	}
	//}
	//return &ArryClass[0];
	return Class;
}

//===========================================================================================================
// 探したオブジェクトを引数クラスにキャストする
//===========================================================================================================
template<typename typeclass> inline typeclass* CSearch::SearchType(typeclass* Class, CObject* pObj)
{ // ( 求めているクラス、探したオブジェクト )
	Class = (typeclass*)pObj;	// クラスをキャストする
	return Class;
}

//===========================================================================================================
// オブジェクトを探す(複数?!)
//===========================================================================================================
template<typename typeclass> inline std::vector<typeclass*> CSearch::SearchMultiObject(std::vector<typeclass*>Class, CObject::TYPE type)
{
	std::vector<typeclass*>ClassTemp{};	// 

	for (int nPrio = 0; nPrio < CObject::PRIORITY_MAX; nPrio++)
	{
		CObject* pObj = CObject::GetTop(nPrio);
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();

			if (pObj->GetType() == type)
			{
				ClassTemp = SearchTypeMulti(ClassTemp, pObj);	// タイプをセットする
			}

			pObj = pNext;
		}
	}
	

	//for (int nCntPrio = 0; nCntPrio < CObject::PRIORITY_MAX; nCntPrio++)
	//{
	//	for (int nCntObj = 0; nCntObj < CObject::MAX_OBJECT; nCntObj++)
	//	{
	//		CObject* pObj = CObject::GetOBJECT(nCntPrio, nCntObj);

	//		if (pObj != nullptr
	//			&& pObj->GetType() == type)	// オブジェクトが存在して、タイプが一致したとき
	//		{
	//			ClassTemp = SearchTypeMulti(ClassTemp, pObj);	// タイプをセットする
	//		}
	//	}
	//}

	return ClassTemp;
}

//===========================================================================================================
// 探したオブジェクトを引数クラスにキャストする
//===========================================================================================================
template<typename typeclass> inline std::vector<typeclass*> CSearch::SearchTypeMulti(std::vector<typeclass*> Class, CObject* pObj) // ( 求めているクラス、探したオブジェクト )
{
	//Class = (typeclass*)pObj;	// クラスをキャストする

	//Class.emplace_back((std::vector<typeclass*>)pObj);
	Class.emplace_back((typeclass*)pObj);
	return Class;
}

#endif // !SEARCH_H_