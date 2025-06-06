//===========================================================================================================================================================
// 
// player3D.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _PLAYER_3D_H_
#define _PLAYER_3D_H_
#include "main.h"
#include "model.h"
#include "skill.h"
#include "enemy3D.h"
#include "sword.h"
#include "buttleUI.h"

//===================================================================================
// �N���X
//===================================================================================

// Player3D�̃N���X
class CPlayer3D :public CModel
{
private:
	typedef enum
	{
		COLLISION_HORIZON = 0,
		COLLISION_VERTICAL, 
		COLLISION_HIGHLOW,
		COLLISION_MAX
	}COLLISION_TYPE;

	static inline const float m_fJumpPower = 8.0f;	// �W�����v��
	static inline const float m_fGravity = 0.5f;	// �d��
	static const unsigned int m_MaxLife = 560;		// �̗�
	static const unsigned int m_MaxInterval = 15;	// �_���[�W���󂯂�Ԋu
public:

	// �v���C���[�̃X�L�����
	typedef enum
	{
		SKILL_NONE = 0,
		SKILL_TRIGGER,		// �X�L���{�^���������ꂽ�u��
		SKILL_PRESS,		// ������Ă����
		SKILL_RELEASE,		// �����ꂽ�Ƃ�
		SKILL_CANCELL,		// �X�L������
		SKILL_SWORD,		// ���X�L������
		SKILL_SWORD_CANCEL,	// ���X�L������
		SKILL_MAX
	}SKILL_STATE;

	CPlayer3D(int nPriority = PRIORITY_DEFAULT-1);			// �R���X�g���N�^
	~CPlayer3D()override;			// �f�X�g���N�^
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I������
	void Update()override;	// �X�V����
	void Draw()override;	// �`�揈��

	void UpdateSkillState();	// �X�L����Ԃ̊Ǘ�
	D3DXVECTOR3 InputPosPlayer();
	void SetTargetCamera();	// player �� pos �� camera �� target �ɃZ�b�g����
	void Jump();
	void Attack();	// �U��
	void CreateTarget();
	void SwordAttack(CEnemy3D*pEnemy);	// ���X�L���̍U��
	void FrontChange();	// �퓬�ɏo���g��G��ς���
	void UpddateMessengerList();	// �g��G�̏��Ԃ��X�V����
	void UpdateDamage();		// �_���[�W���󂯂Ă���Ƃ��̏���
	void DeathPlayer();			// �v���C���[�����񂾂Ƃ�
	void SetButtleUI();			// �퓬UI�̐ݒ�

	void MessengerUpdate();	// �g��G�̍X�V
	void MessengerDraw();	// �g��G�̕`��

	D3DXVECTOR3 LookObjectCollision(COLLISION_TYPE type);
	D3DXVECTOR3 VerticalCollision(CObject* pObj);	// �c�̓����蔻��
	D3DXVECTOR3 HorizonCollision(CObject* pObj);	// ���̓����蔻��
	D3DXVECTOR3 HighLowCollision(CObject* pObj);	// �㉺�̓����蔻��

	CPlayer3D* GetPlayer() { return this; }
	static CPlayer3D* Create(D3DXVECTOR3 pos);	// ��������
private:
	float m_fSpeed;				// �v���C���[�̑��x
	bool m_bUseJump;			// �W�����v���g���Ă��邩�ǂ����̔���(�����W�����v���Ȃ�����)
	D3DXVECTOR3 m_Move;			// �ړ�����
	D3DXVECTOR3 m_oldPos;		// 1�t���[���O��pos
	SKILL_STATE m_skillstate;	// �X�L���{�^���������Ƃ��̏��
	static int m_nFragmentNum;	// ���ݎ����Ă��錇�Ђ̐�
	
	CEnemy3D::TAG_ENEMY m_FrontMessengerTag;	// �\�ɏo�Ă���g��G���m�F����^�O
	CEnemy3D::TAG_ENEMY m_NextMessengerTag;		// ���ɏo��g��G���m�F����^�O
	CEnemy3D::TAG_ENEMY m_PrevMessengerTag;		// �O�ɏo��g��G���m�F����^�O

	CEnemy3D* m_pMessenger[CEnemy3D::TAG_ENEMY::TAG_MAX+1];	// �g��G�̃|�C���^
	bool m_bSlip;
	int m_nSlipCnt;
	bool m_bDammage;	// �_���[�W��Ԃ����� bool
	int m_nMaxLife = m_MaxLife;
	int m_nLife;		// �̗�
	int m_nCntDamage;	// �_���[�W��Ԃ̃J�E���g
	
	static CSkill* m_pSkill;		// �X�L���̃|�C���^
	static CSword* m_pSword;		// ���̃|�C���^
	static CButtleUI*m_pButtleUI[CButtleUI::BUTTLE_MESSE_MAX];	// �g��G��UI�|�C���^
public:
	SKILL_STATE GetSkillstate(){ return m_skillstate; }	// �X�L���{�^���̏��

	void SetMove(D3DXVECTOR3 move) { m_Move = move; }	// move �̐��l��ݒ肷��
	D3DXVECTOR3 GetMove() { return m_Move; }			// move �̐��l��Ԃ�

	D3DXVECTOR3 GetOldPos() { return m_oldPos; }		// 1�t���[���O�� pos ���擾����
	void SetbUseJump(bool use) { m_bUseJump = use; }	// �W�����v�̏�Ԃ�ݒ肷��

	static CSkill* GetSkill() {	// �X�L���|�C���^��Ԃ�
		if (m_pSkill != nullptr) { return m_pSkill; }	// null ����Ȃ�������
		return nullptr;		// ����ȊO�� null ��Ԃ�
	}

	static CSword* GetSword() { return m_pSword; }

	int GetFrontTag() { return m_FrontMessengerTag; }

	void AddDamage(int nDamage);	// �_���[�W���₷����

	void SetLife(int life) { m_nLife = life; }
	int GetLife() { return m_nLife; }

	int GetMaxLife() { return m_MaxLife; }
};


#endif // !_PLAYER_3D_H_