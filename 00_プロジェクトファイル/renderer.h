//===========================================================================================================================================================
// 
// renderer.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include"d3dx9.h"
#include "text.h"

class CRenderer
{
private:

	typedef enum
	{
		_FADE_ALPHA=0,
		_ENEMY_POS,
		_ENEMY_MOVE,
		_PLAYER_POS,
		_PLAYER_MOVE,
		_ENEMY_BUTTLE,
		_MAX_PRINT
	}_DEBUG_PRINT;

public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd,BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice();	// 3D�f�o�C�X�̎擾
	D3DPRESENT_PARAMETERS GetPresentP() { return m_d3dpp; }
	UINT GetResizeWidth() { return m_ResizeWidth; }
	UINT GetResizeHeight() { return m_ResizeHeight; }

	void SetResizeWidth(UINT Resize) { m_ResizeWidth = Resize; }
	void SetResizeHeight(UINT Resize) { m_ResizeHeight = Resize; }

	void DebugPrint(char&pStr, char& pStr2);
	static CText* GetText() { return m_pText; }	// FragmentManager �̎擾
private:
	LPDIRECT3D9 m_pD3D;	// Direct3D
	LPDIRECT3DDEVICE9 m_pD3DDevice;	// Direct3D�f�o�C�X
	D3DPRESENT_PARAMETERS m_d3dpp;	// �v���[���e�[�V�����p�����[�^�[
	LPD3DXFONT m_pFont;	// �t�H���g�̃|�C���^

	UINT m_ResizeWidth;
	UINT m_ResizeHeight;

	static CText* m_pText;	// text �̃|�C���^

};

#endif // !_RENDERER_H_