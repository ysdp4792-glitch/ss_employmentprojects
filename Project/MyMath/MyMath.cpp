#include "MyMath.h"
#include <math.h>

// ベクトルを作る
// startPos	作るベクトルの始点
// endPos	作るベクトルの終点
VECTOR MyMath::VecCreate(VECTOR startPos, VECTOR endPos)
{
	VECTOR result;

	result.x = endPos.x - startPos.x;
	result.y = endPos.y - startPos.y;
	result.z = endPos.z - startPos.z;

	return result;
}

// ベクトルの足し算
// vecA ＋ vecB
VECTOR MyMath::VecAdd(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.x + vecB.x;
	result.y = vecA.y + vecB.y;
	result.z = vecA.z + vecB.z;

	return result;
}

// ベクトルのスカラー倍
// vecA * scale
VECTOR MyMath::VecScale(VECTOR vecA, float scale)
{
	VECTOR result;

	result.x = vecA.x * scale;
	result.y = vecA.y * scale;
	result.z = vecA.z * scale;

	return result;
}

// ベクトルの内積
// vecA ・ vecB
float MyMath::VecDot(VECTOR vecA, VECTOR vecB)
{
	return vecA.x * vecB.x + vecA.y * vecB.y + vecA.z * vecB.z;
}

// ベクトルの外積(2D)
// vecA × vecB
float MyMath::VecCross2D(VECTOR vecA, VECTOR vecB)
{
	return vecA.x * vecB.y - vecA.y * vecB.x;
}

// ベクトルの外積(3D)
VECTOR MyMath::VecCross3D(VECTOR vecA, VECTOR vecB)
{
	VECTOR result;

	result.x = vecA.y * vecB.z - vecA.z * vecB.y;
	result.y = vecA.z * vecB.x - vecA.x * vecB.z;
	result.z = vecA.x * vecB.y - vecA.y * vecB.x;

	return result;
}

// ベクトルの長さを計算する
float MyMath::VecLong(VECTOR vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

// ベクトルを正規化する
VECTOR MyMath::VecNormalize(VECTOR vec)
{
	float vecLong = VecLong(vec);

	// 長さ0の場合は0ベクトルを返す
	if (vecLong == 0.0f) return VGet(0.0f, 0.0f, 0.0f);

	return VecScale(vec, 1.0f / vecLong);
}

// ZX平面上の前方ベクトル(長さ１)を取得する
VECTOR MyMath::VecForwardZX(float rotY)
{
	VECTOR result;

	// X成分はsin(Θ)
	result.x = sinf(rotY);
	// ZX平面なのでYは無視
	result.y = 0.0f;
	// Z成分はcos(Θ)
	result.z = cosf(rotY);

	return result;
}

VECTOR MyMath::VecRotateY(const VECTOR& vec, float angleY)
{
	float cosY = cosf(angleY);
	float sinY = sinf(angleY);
	
	VECTOR result;
	result.x = vec.x * cosY - vec.z * sinY;
	result.y = vec.y; // Y軸はそのまま
	result.z = vec.x * sinY + vec.z * cosY;

	return result;
}

VECTOR MyMath::VecRight(float yRot)
{
	return VGet(cosf(yRot), 0.0f, -sinf(yRot));  // Forwardと直交するベクトル
}

// 単位行列を生成する
MATRIX MyMath::MatCreate()
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		result.m[i][i] = 1.0f;
	}

	return result;
}

// 行列同士の足し算
MATRIX MyMath::MatAdd(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = matA.m[i][j] + matB.m[i][j];
		}
	}

	return result;
}

// 行列同士の引き算
MATRIX MyMath::MatSubt(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = matA.m[i][j] - matB.m[i][j];
		}
	}

	return result;
}

// 行列のスカラー倍
MATRIX MyMath::MatScale(MATRIX mat, float scale)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = mat.m[i][j] * scale;
		}
	}

	return result;
}

// 行列同士の掛け算
MATRIX MyMath::MatMult(MATRIX matA, MATRIX matB)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += matA.m[i][k] * matB.m[k][j];
			}
		}
	}

	return result;
}

// 行列の転置
MATRIX MyMath::MatTransposition(MATRIX mat)
{
	MATRIX result = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = mat.m[j][i];
		}
	}

	return result;
}

// 平行移動行列を取得
MATRIX MyMath::MatTranslation(VECTOR trans)
{
	MATRIX result = MatCreate();

	result.m[0][3] = trans.x;
	result.m[1][3] = trans.y;
	result.m[2][3] = trans.z;

	return result;
}

// 拡縮行列を取得
MATRIX MyMath::MatScale(VECTOR scale)
{
	MATRIX result = MatCreate();

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

// ピッチ行列を取得
MATRIX MyMath::MatRotationPitch(float rot)
{
	MATRIX result = MatCreate();

	result.m[1][1] = cosf(rot);
	result.m[1][2] = -sinf(rot);
	result.m[2][1] = sinf(rot);
	result.m[2][2] = cosf(rot);

	return result;
}

// ヨー行列を取得
MATRIX MyMath::MatRotationYaw(float rot)
{
	MATRIX result = MatCreate();

	result.m[0][0] = cosf(rot);
	result.m[0][2] = sinf(rot);
	result.m[2][0] = -sinf(rot);
	result.m[2][2] = cosf(rot);

	return result;
}

// ロール行列を取得
MATRIX MyMath::MatRotationRoll(float rot)
{
	MATRIX result = MatCreate();

	result.m[0][0] = cosf(rot);
	result.m[0][1] = -sinf(rot);
	result.m[1][0] = sinf(rot);
	result.m[1][1] = cosf(rot);

	return result;
}

// 座標に変換行列を掛けて変換後の座標を計算する
VECTOR MyMath::MatTransform(MATRIX transMat, VECTOR pos)
{
	VECTOR result = { 0 };


	// 計算をfor文で回したいため、いったん配列に格納する
	float posBuffer[4] = {pos.x, pos.y, pos.z, 1.0f};
	// 計算結果もいったん配列に格納する
	float resultBuffer[4] = { 0 };

	// 行列変換の計算
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			resultBuffer[i] += transMat.m[i][j] * posBuffer[j];
		}
	}

	// 計算結果をベクトルに格納
	result.x = resultBuffer[0];
	result.y = resultBuffer[1];
	result.z = resultBuffer[2];

	return result;
}

float MyMath::SegmentToSegmentDistance(const VECTOR& p1, const VECTOR& p2, const VECTOR& q1, const VECTOR& q2)
{
	// 2つの線分の最近接点同士の距離を返す

	// 線分PQに関するベクトル
	VECTOR d1 = VSub(p2, p1);  // P線分方向
	VECTOR d2 = VSub(q2, q1);  // Q線分方向
	VECTOR r = VSub(p1, q1);

	float a = VDot(d1, d1); // |d1|^2
	float e = VDot(d2, d2); // |d2|^2
	float f = VDot(d2, r);

	float s, t;

	if (a <= 0.00001f && e <= 0.00001f)
	{
		// 両方の線分が点
		return VSize(VSub(p1, q1));
	}
	if (a <= 0.00001f)
	{
		// Pが点
		s = 0.0f;
		t = Clamp(f / e, 0.0f, 1.0f);
	}
	else
	{
		float c = VDot(d1, r);
		if (e <= 0.00001f)
		{
			// Qが点
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = VDot(d1, d2);
			float denom = a * e - b * b;

			if (denom != 0.0f)
			{
				s = Clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else
			{
				s = 0.0f;
			}

			t = (b * s + f) / e;

			if (t < 0.0f)
			{
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f)
			{
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	VECTOR c1 = VAdd(p1, VScale(d1, s));
	VECTOR c2 = VAdd(q1, VScale(d2, t));
	return VSize(VSub(c1, c2));
}

float MyMath::Clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
// 角度補間関数
float MyMath::LerpAngle(float current, float target, float t)
{
	float diff = target - current;

	// -π ～ π の範囲にラップ
	while (diff > DX_PI) diff -= DX_PI * 2.0f;
	while (diff < -DX_PI) diff += DX_PI * 2.0f;

	return current + diff * t;
}
