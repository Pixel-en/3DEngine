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
    //return g_texture.Sample(g_sampler, inData.uv) * inData.uv.y;
    float4 output;
 //   if(inData.uv.y==0.5)
 //       output = float4(1, 0, 0, 0);
 //   else
 //       output = g_texture.Sample(g_sampler, inData.uv);
    
    //if(inData.uv.y<1.0/3.0)
    //    output = float4(0, 0, 0, 0);
    //else if (inData.uv.y < 2.0/3.0)
    //    output = float4(1, 0, 0, 0);
    //else 
    //    output = float4(1, 207 / 255, 0, 0);
    
    float2 size;
    g_texture.GetDimensions(size.x, size.y);
    if ((int) (inData.uv.y * 600) % 2 == 1)
    {
        if ((int) (inData.uv.x * 800) % 2 == 1)
        {
            output = g_texture.Sample(g_sampler, inData.uv);
        
            output = float4((output.x + output.y + output.z) / 3,
                        (output.x + output.y + output.z) / 3,
                        (output.x + output.y + output.z) / 3, 0);
        }
        else
            output = float4(0, 0, 0, 0);

    }
    else
        output = float4(0, 0, 0, 0);
    
    return output;
}