//===========================================================================================================================================================
// 
// grass.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _GRASS_H_
#define _GRASS_H_
#include "main.h"
#include "block3D.h"


class CGrass: CBlock3D
{
private:
	static inline const float MAX_SIZE = 23.0f;
public:
	CGrass();
	~CGrass()override;
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��

	static CGrass* Create(D3DXVECTOR3 pos);
private:

};


#endif // !_GRASS_H_