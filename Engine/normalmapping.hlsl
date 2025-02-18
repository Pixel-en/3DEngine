
//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー
Texture2D g_nTexture : register(t1);    //ノーマルマップテクスチャー

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
    int isTexture; // テクスチャ貼ってあるかどうか
    bool isNormalMapped;    //法線マップが張られているかどうか
};

cbuffer gStage : register(b1)
{
    float4 lightPosition[5];
    float4 eyePosition;
    float4 pLightposition;
    float4 pointLightColor[5];
    float4 spotLightColor;
    float4 direction;
    float4 kTerm[5];
    float4 sptParam;
    int4 pointlistSW[5];
};


//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 pos : SV_POSITION;   //位置
    float2 uv : TEXCOORD;       //UV座標
    float4 eyev : POSITION; //ワールド座標に変換された視線ベクトル
    float4 Neyev : POSITION1; //ノーマルマップ用の接空間に変換された視線ベクトル
    float4 normal : NORMAL; //法線ベクトル
    float4 light : POSITION2; //ライトを接空間に変換したベクトル
    float4 color : COLOR;       //色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL,float4 tangent:TANGENT)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.uv = uv;
    
    float3 tmp = cross(tangent.xyz, normal.xyz);
    //tmp.w = 0;
    float4 binormal = mul(tmp, matNormal);
    binormal = normalize(binormal);
    
    normal = mul(normal, matNormal);
    normal.w = 0;
    outData.normal = normalize(normal);
    
    tangent = mul(tangent, matNormal);
    tangent.w = 0;
    tangent = normalize(tangent);
    
    float4 posw = mul(pos, matW);
    outData.eyev = normalize(posw - eyePosition);
    
    outData.Neyev.x = dot(outData.eyev, tangent);
    outData.Neyev.y = dot(outData.eyev, binormal);
    outData.Neyev.z = dot(outData.eyev, normal);
    outData.Neyev.w = 0;
    
    //normal = mul(normal, matW);
   // normal = normalize(normal);
    
    
    float4 light = lightPosition[0];
    light.w = 0;
    light = normalize(light);
    
    //ライトを接空間に変換
    outData.light.x = mul(light, tangent);
    outData.light.y = mul(light, binormal);
    outData.light.z = mul(light, normal);
    outData.light.w = 0;
    
    outData.color = clamp(dot(outData.normal, light), 0, 1);
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
    
    //float4 lightSource = float4(1.0, 1.0, 1.0, 1.0);
    float4 ambientSource = float4(0.7, 0.7, 0.7, 0.7);
    float4 diffuse;
    float4 ambient;
    
    if (isNormalMapped)
    {
        float4 nmap = g_nTexture.Sample(g_sampler, inData.uv) * 2.0f - 1.0f;
        nmap = normalize(nmap);
        nmap.w = 0;
        
        float4 NL = clamp(dot(normalize(inData.light), nmap), 0, 1);

        if (isTexture == false)
        {
            
            diffuse = diffuseColor * NL * factor.x;
            ambient = diffuseColor * ambientSource * factor.x;

        }
        else
        {
            diffuse = g_texture.Sample(g_sampler, inData.uv) * NL * factor.x;
            ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource * factor.x;

        }
        return diffuse + ambient;
        
    }
    else
    {
        if (isTexture == false)
        {
        //return Id * cos_alpha * diffuseColor + Id * diffuseColor * ambentSource;
            diffuse = diffuseColor * inData.color * factor.x;
            ambient = diffuseColor * ambientSource * factor.x;

        }
        else
        {
            diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.color * factor.x;
            ambient = g_texture.Sample(g_sampler, inData.uv) * ambientSource * factor.x;

        }
        //return Id * Kd * cos_alpha + Id * Kd * ambentSource;
    
        return diffuse + ambient;
    }
    
    //return g_texture.Sample(g_sampler, inData.uv);
}
