//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
// �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������

cbuffer gModel : register(b0)
{
    //�ϊ��s��A���_�A����
    float4x4 matWVP; // ���[���h�E�r���[�E�v���W�F�N�V�����̍����s��
    float4x4 matW; //�@�������[���h���W�ɑΉ�������s�񁁉�]���X�P�[���̋t�s��i���s�ړ��͖����j
    float4x4 matNormal; //���[���h�s��
    float4 diffuseColor; // �f�B�t���[�Y�J���[�i�}�e���A���̐F�j�g�U���ˌW��
    //float4 lightPosition; //���s�����̃x�N�g��
    float4 factor; //�f�B�t���[�Y�̔��˂̋���
    float4 ambientColor;
    float4 specularColor;
    float4 shininess;
    bool isTexture; // �e�N�X�`���\���Ă��邩�ǂ���
};

cbuffer gStage : register(b1)
{
    float4 lightPosition;
    float4 eyePosition;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
    //float4 color : COLOR; //�F�i���邳�j
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    float4 spos = mul(pos, matWVP);
    float4 wpos = mul(pos, matW);   //���[���h���W�ϊ�
    float4 wnormal = mul(normal, matNormal);
    
    outData.pos = spos;
    outData.wpos = wpos;
    outData.uv = uv.xy;
    outData.normal = wnormal;
    outData.eyev = eyePosition - wpos;
   // float4 dir = normalize(lightVec - wpos);
    //outData.color = clamp(dot(normalize(wnormal), dir), 0, 1);
    
	//�܂Ƃ߂ďo��
    return outData;
    
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
float4 PS(VS_OUT inData) : SV_Target
{
    //float4 Id = { 1.0, 1.0, 1.0, 0.0 }; //���̋���
    //float4 Kd = g_texture.Sample(g_sampler, inData.uv); //�e�N�X�`���̐F�̃T���v�����O
    //float cos_alpha = inData.cos_alpha; //�g�U���ˌW��
    //float4 ambentSource = { 0.3, 0.3, 0.3, 0.0 }; //�����̋���
    
    float4 ambientSource = float4(0.2, 0.2, 0.2, 1.0);
    float4 diffuse;
    float4 ambient;
    float3 dir = normalize(lightPosition.xyz - inData.wpos.xyz); //�s�N�Z����̃|���S���̂R�������W�ϊ� = wpos
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