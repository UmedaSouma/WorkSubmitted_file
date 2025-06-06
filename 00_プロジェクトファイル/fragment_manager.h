//===========================================================================================================================================================
// 
// fragment_manager.cppのヘッダー
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
	void CreateMessenger();	// 使役敵を召喚する
private:
	int m_FragmentNum[CEnemy3D::TAG_ENEMY::TAG_MAX];	// タグごとに欠片数を管理する変数
	bool m_bIsAppear[CEnemy3D::TAG_ENEMY::TAG_MAX];

public: 
	void SetAppear(CEnemy3D::TAG_ENEMY tag) { m_bIsAppear[tag] = true; }
	bool GetAppear(CEnemy3D::TAG_ENEMY tag) { return m_bIsAppear[tag]; }
	int GetFragmentNum(CEnemy3D::TAG_ENEMY tag) { return m_FragmentNum[tag]; }	// タグごとに数を取得する処理
	void AddFragmentNum(CEnemy3D::TAG_ENEMY tag) { m_FragmentNum[tag]++; }	// タグを参照して所持数を増やす
};




#endif // !FRAGMENT_MANAGER_H_