//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源
    float4x4 matW; //ワールド行列
    //float4x4 matN;
    int2  winSize;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    //outData.pos = mul(pos, matW);
    outData.pos = pos;
    outData.uv = uv;
    
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    //return g_texture.Sample(g_sampler, inData.uv);
    
    float4 output;
    output = g_texture.Sample(g_sampler, inData.uv);
    //(int) (inData.uv.x * winSize.x), int(inData.uv.y * winSize.y);
    float2 nowUV = inData.uv;
    (int) (inData.uv.x + 1.0f / winSize.x) * winSize.x, (int) int(inData.uv.y * winSize.y);
    float2 neiUV = float2(inData.uv.x + 2.0f / winSize.x, inData.uv.y);
    float4 now, nei;
    now = g_texture.Sample(g_sampler, inData.uv);
    nei = g_texture.Sample(g_sampler, neiUV);
    
    
    float2 n1, n2, n3, n4, n5, n6, n7, n8, n9;
    float4 c1, c2, c3, c4, c5, c6, c7, c8, c9;
    
    n1 = float2(inData.uv.x - 1.0f / winSize.x, inData.uv.y - 1.0f / winSize.y);
    n2 = float2(inData.uv.x                   , inData.uv.y - 1.0f / winSize.y);
    n3 = float2(inData.uv.x + 1.0f / winSize.x, inData.uv.y - 1.0f / winSize.y);
    n4 = float2(inData.uv.x - 1.0f / winSize.x, inData.uv.y);
    n5 = float2(inData.uv.x                   , inData.uv.y);
    n6 = float2(inData.uv.x + 1.0f / winSize.x, inData.uv.y);
    n7 = float2(inData.uv.x - 1.0f / winSize.x, inData.uv.y + 1.0f / winSize.y);
    n8 = float2(inData.uv.x                   , inData.uv.y + 1.0f / winSize.y);
    n9 = float2(inData.uv.x + 1.0f / winSize.x, inData.uv.y + 1.0f / winSize.y);
    
    c1 = g_texture.Sample(g_sampler, n1);
    c2 = g_texture.Sample(g_sampler, n2);
    c3 = g_texture.Sample(g_sampler, n3);
    c4 = g_texture.Sample(g_sampler, n4);
    c5 = g_texture.Sample(g_sampler, n5);
    c6 = g_texture.Sample(g_sampler, n6);
    c7 = g_texture.Sample(g_sampler, n7);
    c8 = g_texture.Sample(g_sampler, n8);
    c9 = g_texture.Sample(g_sampler, n9);
    
    return ((c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8 + c9) / 9.0f);
    return ((now + nei) / 3.0);
}