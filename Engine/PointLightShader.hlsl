//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────

cbuffer gModel : register(b0)
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
    float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
    //float4 color : COLOR; //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    float4 spos = mul(pos, matWVP);
    float4 wpos = mul(pos, matW);   //ワールド座標変換
    float4 wnormal = mul(normal, matNormal);
    
    outData.pos = spos;
    outData.wpos = wpos;
    outData.uv = uv.xy;
    outData.normal = wnormal;
    outData.eyev = eyePosition - wpos;
   // float4 dir = normalize(lightVec - wpos);
    //outData.color = clamp(dot(normalize(wnormal), dir), 0, 1);
    
	//まとめて出力
    return outData;
    
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
    
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse;
    float4 ambient;
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz); //ピクセル一のポリゴンの３次元座標変換 = wpos
   // inData.normal.z = 0;
    float4 color = saturate(dot(normalize(inData.normal.xyz), dir));
    float3 k = { 0.2f, 0.2f, 1.0f };
    float len = length(lightPosition.xyz - inData.wpos.xyz);
    float dTerm = 1.0f / (k.x + k.y * len + k.z * len * len);
    
    float4 R = reflect(normalize(inData.normal), normalize(float4(dir, 1.0)));
    float4 specular = pow(saturate(dot(R, normalize(inData.eyev))), shininess) * specularColor;
    
    if (isTexture == false)
    {
        //return Id * cos_alpha * diffuseColor + Id * diffuseColor * ambentSource;
        diffuse = diffuseColor * color * dTerm * factor.x;
        ambient = diffuseColor * ambientSource;

    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * color * dTerm * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource;

    }
    
    return diffuse + ambient + specular;
    
    //return g_texture.Sample(g_sampler, inData.uv);
}