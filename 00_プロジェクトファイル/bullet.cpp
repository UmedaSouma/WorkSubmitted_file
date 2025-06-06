//================================================================================================================
// 
// �e�̏���
// Aouther:umeda souma
// 
//================================================================================================================
#include "bullet.h"
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "explosion.h"
#include "enemy.h"

LPDIRECT3DTEXTURE9 CBullet::m_pTexture = nullptr;

//======================================================================
// �R���X�g���N�^
//======================================================================
CBullet::CBullet(int nPriority) :CObject2D(nPriority), m_fSpeed(0), m_nLife(0), m_bIsEnemyCollision(false)
{
	m_nPriority = nPriority;
}

//======================================================================
// �f�X�g���N�^
//======================================================================
CBullet::~CBullet()
{
}

//======================================================================
// �����ݒ�
//======================================================================
HRESULT CBullet::Init()
{
	// �T�C�Y�̒l��ݒ�
	float bulletsize = 30.0f;

	m_fSpeed = 10.0f;	// �X�s�[�h�ݒ�
	m_nLife = MAX_LIFE;		// �����ݒ�
	m_nTexSplit = { 1,1 };	// �e�N�X�`�������ݒ�

	// �T�C�Y��ݒ�
	SetSize({ bulletsize,bulletsize,bulletsize });

	CObject2D::Init();

	return S_OK;
}

//======================================================================
// �I������
//======================================================================
void CBullet::Uninit()
{
	CObject2D::Uninit();
}

//======================================================================
// �X�V����
//======================================================================
void CBullet::Update()
{
	D3DXVECTOR3 pos = GetPos();	// �ʒu���擾

	// �S�ẴI�u�W�F�N�g���m�F����
	LookObjectALL();

	// pos.x += m_fSpeed;	// �������ʒu�����炷
	pos.x += m_fSpeed;	// �������ʒu�����炷
	SetPos(pos);	// �ʒu���X�V������

	// �e���������}�����Ƃ�
	if (m_nLife <= 0) 
	{ 
		CExplosion::Create(pos, { 3,3 });	// �����𐶐�

		// �������
		Uninit();
		//Release();
		return;
	}

	CObject2D::Update();

	m_nLife--;
}

//======================================================================
// �`�揈��
//======================================================================
void CBullet::Draw()
{
	CObject2D::Draw();
}

//======================================================================
// ��������
//======================================================================
CBullet* CBullet::Create(D3DXVECTOR3 pos)
{
	// ���I�m��
	CBullet* pBullet = new CBullet;

	pBullet->SetPos(pos);	// �ʒu�ݒ�
	pBullet->BindTexture(m_pTexture);	// �e�N�X�`���Z�b�g
	pBullet->Init();	// �����ݒ�

	return pBullet;	// �e�̏���Ԃ�
}

//======================================================================
// �S�ẴI�u�W�F�N�g�m�F->�����蔻��
//======================================================================
void CBullet::LookObjectALL()
{
	// �I�u�W�F�N�g��S�Ċm�F
	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	{
		// �I�u�W�F�N�g�̃|�C���^�ɑ��
		CObject* pObj = GetOBJECT(PRIORITY_DEFAULT, nCntObj);

		if (pObj != nullptr)
		{
			// �����蔻��
			m_bIsEnemyCollision = CollisionEnemy(pObj);
		}
	}
}

//======================================================================
// �G�Ƃ̓����蔻��
//======================================================================
bool CBullet::CollisionEnemy(CObject* pObj)
{
	// ���g�������Ă���I�u�W�F�N�g�̃^�C�v���m�F
	CObject::TYPE type = pObj->GetType();

	// Bullet�̏��������Ă���
	D3DXVECTOR3 BulletPos = GetPos();	// pos
	D3DXVECTOR3 BulletSize = GetSize(); // size

	// �T�C�Y�𔼕��ɐݒ肵�Ă���
	BulletSize.x *= 0.5f;
	BulletSize.y *= 0.5f;

	// �擾�����I�u�W�F�N�g���G�l�~�[�������ꍇ
	if (type == TYPE::ENEMY)
	{
		// �I�u�W�F�N�g���G�l�~�[�Ƀ_�E���L���X�g
		CEnemy* pEnemy = (CEnemy*)pObj;

		// enemy�̏����擾
		D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
		D3DXVECTOR3 EnemySize = pEnemy->GetSize();

		// �T�C�Y�𔼕��ɐݒ�
		EnemySize.x *= 0.5f;
		EnemySize.y *= 0.5f;

		if (EnemyPos.x-EnemySize.x<=BulletPos.x+BulletSize.x
			&&EnemyPos.x+EnemySize.x>=BulletPos.x-BulletSize.x
			&&EnemyPos.y+EnemySize.y>=BulletPos.y-BulletSize.y
			&&EnemyPos.y-EnemySize.y<=BulletPos.y+BulletSize.y
			)
		{// �͈͓��ŏd�Ȃ����Ƃ���	
			m_nLife = 0;	// �e�̎������Ȃ���

			return true;
		}
	}

	return false;
}

//========================================================================================================================
// �e�N�X�`���ǂݍ���
//========================================================================================================================
HRESULT CBullet::TextureLoad()
{
	if (m_pTexture == nullptr)
	{
		CRenderer* Renderer = CManager::GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = Renderer->GetDevice();

		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ElementWind.png", &m_pTexture);
	}

	return S_OK; 
}

//========================================================================================================================
// �e�N�X�`���j��
//========================================================================================================================
void CBullet::TextureUnLoad()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}