
//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

SamplerState g_toon_sampler : register(s1); //サンプラー

Texture2D g_toon_texture : register(t1); //テクスチャー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //法線をワールド座標に対応させる行列＝回転＊スケールの逆行列（平行移動は無視）
    float4x4 matNormal; //ワールド行列
    float4 diffuseColor; // ディフューズカラー（マテリアルの色）拡散反射係数
    //float4 lightPosition; //平行光源のベクトル
    float4 factor; //ディフューズの反射の強さ
    float4 ambientColor;
    float4 specularColor;
    float4 shininess;
    bool isTexture; // テクスチャ貼ってあるかどうか
};

cbuffer gStage : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 color : COLOR; //色（明るさ）
    float4 normal : NORMAL;
};



//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
float4 VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL):SV_Position
{
	//ピクセルシェーダーへ渡す情報
    float4 outPos;
    
    normal.w = 0;
    normal = normalize(normal);
    outPos = pos + normal * 0.1;
    outPos = mul(pos, matWVP);
    
	//まとめて出力
    return outPos;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    //float4 Id = { 1.0, 1.0, 1.0, 0.0 }; //光の強さ
    //float4 Kd = g_texture.Sample(g_sampler, inData.uv); //テクスチャの色のサンプリング
    //float cos_alpha = inData.cos_alpha; //拡散反射係数
    //float4 ambentSource = { 0.3, 0.3, 0.3, 0.0 }; //環境光の強さ
    
   // float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
   // float4 ambientSource = float4(0.2f, 0.2f, 0.2f, 0.0f);
   // float4 diffuse;
   // float ambient;
    
   // float4 NL = saturate(dot(inData.normal, normalize(lightPosition)));
   // //float4 n1 = float4(1.0 / 4.0, 1.0 / 4.0, 1.0 / 4.0, 1);
   // //float4 n2 = float4(2.0 / 4.0, 2.0 / 4.0, 2.0 / 4.0, 1);
   // //float4 n3 = float4(3.0 / 4.0, 3.0 / 4.0, 3.0 / 4.0, 1);
   // //float4 n4 = float4(4.0 / 4.0, 4.0 / 4.0, 4.0 / 4.0, 1);
   // //float4 tI = 0.1 * step(n1, inData.color) + 0.3 * step(n2, inData.color) + 0.6 * step(n3, inData.color);
    
   // //float4 reflection = reflect(normalize(-lightPosition), inData.normal);
   // //float4 specular = pow(saturate(dot(reflection, normalize(inData.eyev))), shininess);
   // float2 uv;
   // uv.x = NL;
   // uv.y = 0.5; //0から1ならなんでもいい
   // float4 tI = g_toon_texture.Sample(g_sampler, uv);
   //// float stI = g_toon_texture.Sample(g_sampler, float2(specular.x, 0));
    
    
   // if (isTexture == false)
   // {
   //     //return Id * cos_alpha * diffuseColor + Id * diffuseColor * ambentSource;
   //     diffuse = diffuseColor * tI;
   //     ambient = diffuseColor * ambientColor;

   // }
   // else
   // {
   //     diffuse = g_texture.Sample(g_sampler, inData.uv) * tI;
   //     ambient = g_texture.Sample(g_sampler, inData.uv) * ambientColor;

   // }
    
    //float4 ret = diffuse + ambient;
    //if (NE > -0.2 && NE < 0.2)
    //{
    //    ret = float4(0, 0, 0, 1);
    //}
        
    //return ret;
    //return Id * Kd * cos_alpha + Id * Kd * ambentSource;
    
    //return diffuse + ambient;
    //return outColor;
    //return diffuse + ambient;
    //return g_texture.Sample(g_sampler, inData.uv);
    //float2 uv = float2(tI.x, 0);
    //return g_toon_texture.Sample(g_sampler, uv);
    return float4(1.0, 0, 0, 1.0);
}
