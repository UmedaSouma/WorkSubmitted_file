//===========================================================================================================================================================
// 
// game.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "main.h"
#include "scene.h"
#include "fragment_manager.h"

class CGame :public CScene
{
public:
	CGame();
	~CGame()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetInitUI();	// ui�̃Z�b�g
	static void UpdateDelay();
	static void SetDelay() { m_Delay = true; m_nDelayEnd = 60;}
private:
	static CFragmentManager* m_pFeagmentMG;	// FragmentManager �̃|�C���^
	static int m_nDelayEnd;	// �|���Ă��烊�U���g�ɂȂ�܂�
	static bool m_Delay;

public:
	static CFragmentManager* GetFragmentMG() { return m_pFeagmentMG; }	// FragmentManager �̎擾
};