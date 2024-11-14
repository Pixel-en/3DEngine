//������������������������������������������������������������������������������
// �e�N�X�`�����T���v���[�f�[�^�̃O���[�o���ϐ���`
//������������������������������������������������������������������������������
Texture2D g_texture : register(t0); //�e�N�X�`���[
SamplerState g_sampler : register(s0); //�T���v���[

//������������������������������������������������������������������������������
 // �R���X�^���g�o�b�t�@
// DirectX �����瑗�M����Ă���A�|���S�����_�ȊO�̏����̒�`
//������������������������������������������������������������������������������
cbuffer global
{
    //�ϊ��s��A���_�A����
    float4x4 matW; //���[���h�s��
    //float4x4 matN;
    int2  winSize;
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_�[�o�́��s�N�Z���V�F�[�_�[���̓f�[�^�\����
//������������������������������������������������������������������������������
struct VS_OUT
{
    float4 pos : SV_POSITION; //�ʒu
    float2 uv : TEXCOORD; //UV���W
};

//������������������������������������������������������������������������������
// ���_�V�F�[�_
//������������������������������������������������������������������������������
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�[�֓n�����
    VS_OUT outData;

	//���[�J�����W�ɁA���[���h�E�r���[�E�v���W�F�N�V�����s���������
	//�X�N���[�����W�ɕϊ����A�s�N�Z���V�F�[�_�[��
    //outData.pos = mul(pos, matW);
    outData.pos = pos;
    outData.uv = uv;
    
	//�܂Ƃ߂ďo��
    return outData;
}

//������������������������������������������������������������������������������
// �s�N�Z���V�F�[�_
//������������������������������������������������������������������������������
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