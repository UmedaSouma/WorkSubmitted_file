//===========================================================================================================================================================
// 
// enemy3D.cpp�̃w�b�_�[
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _ENEMY3D_H_
#define _ENEMY3D_H_

#include "main.h"
#include "model.h"
//===========================================================================================================
// �N���X
//===========================================================================================================
class CEnemy3D :public CModel
{
public:

	// �G���Ƃ̃^�O
	typedef enum
	{
		TAG_NONE = 0, 
		TAG_SLIME,	// �X���C��
		TAG_GOLEM,	// �S�[����
		TAG_MAX
	}TAG_ENEMY;

	// �����蔻��̏ꏊ
	typedef enum
	{
		COLLISION_HORIZON = 0,
		COLLISION_VERTICAL,
		COLLISION_HIGHLOW,
		COLLISION_MAX
	}COLLISION_TYPE;

	// �퓬���̏��
	typedef enum
	{
		BUTTLE_NONE = 0,	// �����Ȃ����
		BUTTLE_DISTANCE,	// �U���O�̋������Ƃ���
		BUTTLE_ATTACK,		// �U����
		BUTTLE_SKILL_THROW,	// �X�L��������
		BUTTLE_SKILL,		// �g�����̃X�L���g�p���
		BUTTLE_MAX
	}STATE_BUTTLE;

	//===========================================================================================================
	// < debugmemo >
	// state �̊Ǘ����������肷��
	// ���� state �͓G�̑S�̓I�ȏ�Ԃ��Ǘ�������
	// �g�������G�̍s�����������ŊǗ�������
	//===========================================================================================================

	// �G���̂̏��
	typedef enum
	{
		STATE_NORMAL = 0,	// �����Ȃ����
		STATE_MOVE,			// �ړ����
		STATE_ATTACK,		// �U�����
		STATE_BEFORE_SKILL,	// �X�L���g�p�O
		STATE_SKILL,		// �X�L���g�p��
		STATE_SWORD,		// ���X�L���g�p��
		STATE_DAMAGE,		// �_���[�W���󂯂����
		STATE_DEAD,			// ���񂾏��
		STATE_CREATE,		// �������ꂽ�u��
		STATE_MAX
	}STATE_ENEMY;

	CEnemy3D();				// �R���X�g���N�^
	~CEnemy3D()override;			// �f�X�g���N�^
	HRESULT Init()override;	// �����ݒ�
	void Uninit()override;	// �I��
	void Update()override;	// �X�V
	void Draw()override;	// �`��
	void SetTarget(D3DXVECTOR3 pos);			// �^�[�Q�b�g�ʒu��ݒ�
	void SetState(STATE_ENEMY state);			// ��Ԃ�ݒ�
	void TargetPlayer();						// �v���C���[�Ƀ^�[�Q�b�g��������
	void CreateCountDown();						// �������ꂽ���̃J�E���g
	void UpdateState();
	void UpdateSkillSwordDamage();	// ���X�L���̍U�����󂯂��ꍇ�̍X�V����

	D3DXVECTOR3 LookObjectCollision(COLLISION_TYPE type);
	D3DXVECTOR3 VerticalCollision(CObject* pObj);	// �c�̓����蔻��
	D3DXVECTOR3 HorizonCollision(CObject* pObj);	// ���̓����蔻��
	D3DXVECTOR3 HighLowCollision(CObject* pObj);	// �㉺�̓����蔻��
	void Sufferdamage(int damage);	// ��_���[�W����

	void TargetWarp(D3DXVECTOR3 playerpos);

	static CEnemy3D* Create(D3DXVECTOR3 pos);	// �v���C���[�̐���
private:
	static inline const float MAX_SIZE = 20.0f;	// �v���C���[�̃T�C�Y
	static const unsigned int MAX_CREATECOUNT = 3;
	D3DXVECTOR3 m_TargetPos;	// �ڕW�̈ʒu
	int m_nMaxLife;				// �̗͍ő�l
	int m_fLife;				// �̗�
	float m_fSpeed;				// ���x
	float m_fPower;				// �U����
	int m_fAtkInterval;			// �U���Ԋu(�t���[����)
	STATE_BUTTLE m_StateButtle;	// �퓬�Ԑ����
	STATE_ENEMY m_State;		// �G�̏��
	STATE_ENEMY m_OldState;		// 1�t���[���O�̓G�̏��
	bool m_bBattle;				// �퓬��Ԃ��ۂ�(ture ��������퓬���)
	float m_Follow;				// �x��ă^�[�Q�b�g�ɒǂ����Ă���W��
	float m_fDistance;			// �^�[�Q�b�g�Ƃ̋���
	TAG_ENEMY m_tag;			// �G�̎��
	static int m_nFragmentNum;	// ���Ђ̐�
	bool m_bIsSkill;	// �X�L���g�p�L��
	int m_nCreateCnt;	// �������ꂽ����
	int m_nCntDamage;	// �_���[�W��Ԃ̃J�E���g
	bool m_bSufferKnockback;	// true -> �m�b�N�o�b�N���󂯂�
	unsigned int m_nBulletInterval;	// �e�̔����Ԋu
	int m_nRespawnCnt;	// ���X�|�[������܂ł̎��Ԃ��J�E���g

	D3DXVECTOR3 m_Move;	// �ړ�����
	D3DXVECTOR3 m_oldPos;	// 1�t���[���O��pos

public:
	void SetLife(int Life) { m_fLife = Life; }
	int GetLife() { return m_fLife; }

	void SetMaxLife(int MaxLife) { m_nMaxLife = MaxLife;}	// �̗͍ő�l��ݒ肷��
	int GetMaxLife() { return m_nMaxLife; }						// �̗͍ő�l���擾����

	void SetStateEnemy(STATE_ENEMY state) { m_State = state; }
	STATE_ENEMY &GetState() { return m_State; }

	void SetMove(D3DXVECTOR3 move) { m_Move = move; }
	D3DXVECTOR3 GetMove() { return m_Move; }

	void SetTargetpos(D3DXVECTOR3 targetpos) { m_TargetPos = targetpos; }
	D3DXVECTOR3 GetTargetpos() { return m_TargetPos; }

	void SetTag(TAG_ENEMY tag) { m_tag = tag; }
	TAG_ENEMY GetTag() { return m_tag; }

	static void SetFragmentNum(int num) { m_nFragmentNum = num; }
	static int GetFragmentNum() { return m_nFragmentNum; }

	void SetStateButtle(const STATE_BUTTLE state) { m_StateButtle = state; }
	STATE_BUTTLE& GetStateButtle() { return m_StateButtle; }

	void SetSufferKnockback(bool bSuffer) { m_bSufferKnockback = bSuffer; }
	bool GetSufferKnockback() { return m_bSufferKnockback; }

	void SetBulletInterval(unsigned int interval) { m_nBulletInterval = interval; }
	int GetBulletInterval() { return m_nBulletInterval; }

	int GetRespawnCnt() { return m_nRespawnCnt; }

	STATE_ENEMY &GetOldState() { return m_OldState; }

	static CEnemy3D* enemyget();	// �f�o�b�N�p�̃G�l�~�[�擾
};

#endif // !_ENEMY3D_H_
