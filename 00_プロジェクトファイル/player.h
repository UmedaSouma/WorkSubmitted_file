//===========================================================================================================================================================
// 
// player.cppÇÃÉwÉbÉ_Å[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object2D.h"

class CPlayer:public CObject2D
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	//void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 size, TEX_FILE tex, D3DXVECTOR2 TexSplit, int Interval);
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static HRESULT TextureLoad();
	static void TextureUnLoad();
	D3DXVECTOR3 GetOldPos();
	void SetMove(D3DXVECTOR3 move);
private:
	D3DXVECTOR3 InputPosPlayer(D3DXVECTOR3 pos);
	D3DXVECTOR3 InputRotPlayer(D3DXVECTOR3 rot);

	D3DXVECTOR3 LookObjectUPDOWN();
	D3DXVECTOR3 LookObjectSIDE();
	D3DXVECTOR3 UpDownCollision(CObject* pObj);	// è„â∫ÇÃìñÇΩÇËîªíË
	D3DXVECTOR3 SideCollision(CObject* pObj);	// â°ÇÃìñÇΩÇËîªíË

	void Jump();

	D3DXVECTOR3 m_oldPos;
	D3DXVECTOR3 m_Move;
	D3DXVECTOR3 m_TargetRot;
	bool m_bMoveSwitch;
	float m_fSpeed;

	static int m_NumAllPlayer;
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//const static unsigned float
};



#endif // !_PLAYER_H_
