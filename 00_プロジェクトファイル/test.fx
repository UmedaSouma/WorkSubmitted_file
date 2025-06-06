//=====================================================================================================================================
// 
// アウトラインシェーダーの処理
// Author : Umeda Souma
// 
//=====================================================================================================================================

//===================================
// 出力情報
//===================================
struct VS_OUTPUT
{
    float4 Pos : POSITION;  // 位置
    float3 Nor : TEXCOORD;  // 法線
};

// グローバル宣言
float4x4 matWVP;    // ワールドビュー射影変換行列宣言
float4x4 World; // ワールド行列
float4 ScreenColor = { 1.0f , 1.0f, 1.0f, 1.0f, };  // モデルの色をそのまま映し出す
float4 Color = { 1.0f , 1.0f, 1.0f, 1.0f, };    // アウトラインの色
float thickness = 0.5f; // アウトラインの厚さ

//===================================================================================
// 頂点シェーダ
//===================================================================================
VS_OUTPUT SicTransform(
    float4 LocalPos : POSITION,
    float4 Normal :NORMAL)
{
    // 出力データの初期化
    VS_OUTPUT l_out = (VS_OUTPUT)0;

    //ワールド空間での法線を計算
    float3 worldNormal = normalize(mul(Normal, (float3x3)World));
    l_out.Nor = normalize(worldNormal);

    float3 offset;

    //頂点を少し押し出す（アウトラインの厚さを調整）
    offset.x = normalize(LocalPos.x) * thickness;
    offset.y = normalize(LocalPos.y) * thickness;
    offset.z = normalize(LocalPos.z) * thickness;

    float4 worldPos = LocalPos + float4(offset, 0.0f);

    //頂点座標を変換
    l_out.Pos = mul(worldPos, matWVP);

    return l_out;
}

//===================================================================================
// マテリアルをそのまま出力する頂点シェーダー
//===================================================================================
float4 BasicTransform(float4 LocalPos : POSITION) : POSITION
{
    // ローカル座標にある頂点をワールドビュー射影変換で
    // 一気にスクリーン座標にしてしまう
    return mul(LocalPos, matWVP);
}

//===================================================================================
// ピクセルシェーダー
//===================================================================================
float4 NoWorkingPixelShader(float4 _ScreenColor : COLOR0) : COLOR0
{
    // 入力されたスクリーンピクセルの色をそのままスルー
    return ScreenColor;
}

//===================================================================================
// 白色を映し出す
//===================================================================================
float4 WorkingPixelShader(float4 _ScreenColor : COLOR0) : COLOR0
{
    // 入力されたスクリーンピクセルの色をそのままスルー
    return Color;
}

//===================================================================================
// テクニック宣言
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
        //裏面を描画（アウトライン用）
        CullMode = CW;

        VertexShader = compile vs_2_0 SicTransform();
        PixelShader = compile ps_2_0 WorkingPixelShader();
    }
}