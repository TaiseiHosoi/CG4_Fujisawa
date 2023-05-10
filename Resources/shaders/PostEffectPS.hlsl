#include"PostEffect.hlsli"


Texture2D<float4> tex0 : register(t0);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex1 : register(t1);  	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      	// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{

	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);

	float4 color = colortex0;

	//1�s�N�Z������UV
	float offsetU = 1 / 1280.0f;
	float offsetV = 1 / 720.0f;

	//�|���鋭�x
	int kernelSize = 4;

	if (fmod(input.uv.y, 0.1f) < 0.0f) {
		// ���ϒl���v�Z���邽�߂̑��a
		float4 sum = float4(0, 0, 0, 0);

		// �J�[�l���T�C�Y�͈͓̔��Ńs�N�Z�����C�e���[�g
		for (int y = -kernelSize; y <= kernelSize; ++y)
		{
			for (int x = -kernelSize; x <= kernelSize; ++x)
			{
				float2 offset = float2(x, y) * float2(offsetU, offsetV);
				float4 colortex1 = tex1.Sample(smp, input.uv + offset);
				sum += colortex1;
			}
		}

		// ���a���J�[�l���T�C�Y�Ŋ��邱�Ƃŕ��ϒl���v�Z
		float kernelArea = (2 * kernelSize + 1) * (2 * kernelSize + 1);
		float4 averageColor = sum / kernelArea;

		color = averageColor;
	}
	return float4(color.rgb, 1);

}