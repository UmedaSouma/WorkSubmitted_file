//=====================================================================================================================================
// 
// �A�E�g���C���V�F�[�_�[�̏���
// Author : Umeda Souma
// 
//=====================================================================================================================================

//===================================
// �o�͏��
//===================================
struct VS_OUTPUT
{
    float4 Pos : POSITION;  // �ʒu
    float3 Nor : TEXCOORD;  // �@��
};

// �O���[�o���錾
float4x4 matWVP;    // ���[���h�r���[�ˉe�ϊ��s��錾
float4x4 World; // ���[���h�s��
float4 ScreenColor = { 1.0f , 1.0f, 1.0f, 1.0f, };  // ���f���̐F�����̂܂܉f���o��
float4 Color = { 1.0f , 1.0f, 1.0f, 1.0f, };    // �A�E�g���C���̐F
float thickness = 0.5f; // �A�E�g���C���̌���

//===================================================================================
// ���_�V�F�[�_
//===================================================================================
VS_OUTPUT SicTransform(
    float4 LocalPos : POSITION,
    float4 Normal :NORMAL)
{
    // �o�̓f�[�^�̏�����
    VS_OUTPUT l_out = (VS_OUTPUT)0;

    //���[���h��Ԃł̖@�����v�Z
    float3 worldNormal = normalize(mul(Normal, (float3x3)World));
    l_out.Nor = normalize(worldNormal);

    float3 offset;

    //���_�����������o���i�A�E�g���C���̌����𒲐��j
    offset.x = normalize(LocalPos.x) * thickness;
    offset.y = normalize(LocalPos.y) * thickness;
    offset.z = normalize(LocalPos.z) * thickness;

    float4 worldPos = LocalPos + float4(offset, 0.0f);

    //���_���W��ϊ�
    l_out.Pos = mul(worldPos, matWVP);

    return l_out;
}

//===================================================================================
// �}�e���A�������̂܂܏o�͂��钸�_�V�F�[�_�[
//===================================================================================
float4 BasicTransform(float4 LocalPos : POSITION) : POSITION
{
    // ���[�J�����W�ɂ��钸�_�����[���h�r���[�ˉe�ϊ���
    // ��C�ɃX�N���[�����W�ɂ��Ă��܂�
    return mul(LocalPos, matWVP);
}

//===================================================================================
// �s�N�Z���V�F�[�_�[
//===================================================================================
float4 NoWorkingPixelShader(float4 _ScreenColor : COLOR0) : COLOR0
{
    // ���͂��ꂽ�X�N���[���s�N�Z���̐F�����̂܂܃X���[
    return ScreenColor;
}

//===================================================================================
// ���F���f���o��
//===================================================================================
float4 WorkingPixelShader(float4 _ScreenColor : COLOR0) : COLOR0
{
    // ���͂��ꂽ�X�N���[���s�N�Z���̐F�����̂܂܃X���[
    return Color;
}

//===================================================================================
// �e�N�j�b�N�錾
//===================================================================================
technique BasicTec
{
    pass P0
    {
        VertexShader = compile vs_2_0 BasicTransform();
        PixelShader = compile ps_2_0 NoWorkingPixelShader();
    }
    pass P1
    {
        //���ʂ�`��i�A�E�g���C���p�j
        CullMode = CW;

        VertexShader = compile vs_2_0 SicTransform();
        PixelShader = compile ps_2_0 WorkingPixelShader();
    }
}