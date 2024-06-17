#include <Novice.h>
#include<cmath>
#include<assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include<imgui.h>
#include<Vector3.h>
#include<Matrix4x4.h>

static const int KRowHeight = 20;
static const int Kcolumnwidth = 60;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

const char kWindowTitle[] = "LE2B_05_オノデラ_ユヅキ_タイトル";

struct Sphere {
	Vector3 center; //!< 中心点
	float radius; //!< 半径
};

struct  Plane {
	Vector3 normal; //!< 法線
	float distance; //!< 距離
};

struct Segment {
	Vector3 origin; //!< 始点 
	Vector3 diff;   //!< 終点への差分ベクトル
};

//加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = (v1.x + v2.x);
	result.y = (v1.y + v2.y);
	result.z = (v1.z + v2.z);
	return result;
};

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

//長さ（ノルム）
float Length(const Vector3& v) {
	return sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));
};




// 線と平面の衝突判定
bool  IsCollision(const Segment& segment, const Plane& plane) {

	// 垂直判定を行うために、法線と線分の方向ベクトルの内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 線分が平面に平行（垂直）である場合は衝突しない
	if (fabs(dot) < 1e-5f) {
		return false;
	}

	// 平面と線分の始点から平面までの距離を計算
	float t = -(Dot(segment.origin, plane.normal) + plane.distance) / dot;

	return t >= -1.0f && t <= 2.0f;	
}


//正規化
Vector3  Normalize(const Vector3& v) {
	Vector3 result{};

	float length = Length(v);

	if (length != 0.0) {
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	}

	return result;
};

// 1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};
	float Ctheta = std::cos(radian);
	float Stheta = std::sin(radian);

	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f;    result.m[0][2] = 0.0f;   result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = Ctheta;  result.m[1][2] = Stheta; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = -Stheta; result.m[2][2] = Ctheta; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f;    result.m[3][2] = 0.0f;   result.m[3][3] = 1.0f;


	return result;
}

//2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};
	float Ctheta = std::cos(radian);
	float Stheta = std::sin(radian);

	result.m[0][0] = Ctheta; result.m[0][1] = 0.0f; result.m[0][2] = -Stheta; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f;   result.m[1][1] = 1.0f; result.m[1][2] = 0.0f;    result.m[1][3] = 0.0f;
	result.m[2][0] = Stheta; result.m[2][1] = 0.0f; result.m[2][2] = Ctheta;  result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;   result.m[3][1] = 0.0f; result.m[3][2] = 0.0f;    result.m[3][3] = 1.0f;

	return result;
}

//3. Z軸回8;転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};
	float Ctheta = std::cos(radian);
	float Stheta = std::sin(radian);

	result.m[0][0] = Ctheta;  result.m[0][1] = Stheta; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = -Stheta; result.m[1][1] = Ctheta; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f;    result.m[2][1] = 0.0f;   result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f;    result.m[3][1] = 0.0f;   result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	return result;

}

//4.合成
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2) {

	Matrix4x4 result{};
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	return result;
};

//5.3次元アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateRMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	result.m[0][0] = scale.x * rotateRMatrix.m[0][0]; result.m[0][1] = scale.x * rotateRMatrix.m[0][1]; result.m[0][2] = scale.x * rotateRMatrix.m[0][2]; 	result.m[0][3] = 0.0f;
	result.m[1][0] = scale.y * rotateRMatrix.m[1][0]; result.m[1][1] = scale.y * rotateRMatrix.m[1][1]; result.m[1][2] = scale.y * rotateRMatrix.m[1][2]; result.m[1][3] = 0.0f;
	result.m[2][0] = scale.z * rotateRMatrix.m[2][0]; result.m[2][1] = scale.z * rotateRMatrix.m[2][1]; result.m[2][2] = scale.z * rotateRMatrix.m[2][2]; result.m[2][3] = 0.0f;
	result.m[3][0] = translate.x; result.m[3][1] = translate.y; result.m[3][2] = translate.z; result.m[3][3] = 1.0f;

	return result;
};

//3次元ベクトルを同次座標として変換する 
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];//PosX
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];//PosY
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];//PosZ

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];;
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
};

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& matrix) {

	float det
		= matrix.m[0][0] * (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2])
		- matrix.m[0][1] * (matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2])
		+ matrix.m[0][2] * (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1])
		- matrix.m[0][3] * (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]);


	Matrix4x4 result;
	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] + matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] - matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) / det;
	result.m[0][1] = (-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) / det;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) / det;
	result.m[0][3] = (-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) / det;

	result.m[1][0] = (-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] - matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) / det;
	result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] + matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) / det;
	result.m[1][2] = (-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] - matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] + matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) / det;
	result.m[1][3] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] + matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) / det;

	result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] + matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] + matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] - matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) / det;
	result.m[2][1] = (-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] - matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] - matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] + matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) / det;
	result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] + matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] + matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] - matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] - matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) / det;
	result.m[2][3] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] - matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] - matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] + matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] + matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) / det;

	result.m[3][0] = (-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] - matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] - matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] + matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] + matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] + matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) / det;
	result.m[3][1] = (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] + matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] + matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] - matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] - matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] - matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) / det;
	result.m[3][2] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] - matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] - matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] + matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] + matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] + matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) / det;
	result.m[3][3] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] + matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] + matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] - matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] - matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] - matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) / det;

	return result;
};

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
};

//1.透視投影行列　
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farclip) {
	Matrix4x4 result{};


	result.m[0][0] = (1.0f / aspectRatio) / tanf(fovY / 2); result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = (1.0f / tanf(fovY / 2)); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = farclip / (farclip - nearClip); result.m[2][3] = 1.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = (-nearClip * farclip) / (farclip - nearClip); result.m[3][3] = 0.0f;


	return result;
};

//2.正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float facDlip) {
	Matrix4x4 result{};

	result.m[0][0] = 2.0f / (right - left); result.m[0][1] = 0; result.m[0][2] = 0; result.m[0][3] = 0;
	result.m[1][0] = 0; result.m[1][1] = 2.0f / (top - bottom); result.m[1][2] = 0; result.m[1][3] = 0;
	result.m[2][0] = 0; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f / (facDlip - nearClip); result.m[2][3] = 0;
	result.m[3][0] = (left + right) / (left - right); result.m[3][1] = (top + bottom) / (bottom - top); result.m[3][2] = nearClip / (nearClip - facDlip); result.m[3][3] = 1.0f;

	return result;
};

//3.ビューボート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float  width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};

	result.m[0][0] = (width / 2.0f); result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = -(height / 2.0f); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = (maxDepth - minDepth); result.m[2][3] = 0.0f;
	result.m[3][0] = left + (width / 2); result.m[3][1] = top + (height / 2.0f); result.m[3][2] = minDepth; result.m[3][3] = 1.0f;
	return result;
};

void DrawGrid(const Matrix4x4& viewProiectionMatrix, const Matrix4x4& ViewportMatrix) {
	const float KGridHalfwidth = 2.0f;
	const uint32_t KSubdivision = 10;
	const float KGridEvery = (KGridHalfwidth * 2.0f) / float(KSubdivision);

	for (uint32_t xIndex = 0; xIndex <= KSubdivision; xIndex++) {
		float posX = -KGridHalfwidth + xIndex * KGridEvery;

		Vector3 startPointX(posX, 0.0f, -KGridHalfwidth);
		Vector3 endPointX(posX, 0.0f, KGridHalfwidth);

		startPointX = Transform(startPointX, Multiply(viewProiectionMatrix, ViewportMatrix));
		endPointX = Transform(endPointX, Multiply(viewProiectionMatrix, ViewportMatrix));
		Novice::DrawLine((int)startPointX.x, (int)startPointX.y, (int)endPointX.x, (int)endPointX.y, 0x6F6F6FFF);
	}
	for (uint32_t zIndex = 0; zIndex <= KSubdivision; zIndex++) {
		float posZ = -KGridHalfwidth + KGridEvery * zIndex;

		Vector3 startPointZ = { -KGridHalfwidth, 0.0f, posZ };
		Vector3 endPointZ = { KGridHalfwidth, 0.0f, posZ };
		startPointZ = Transform(startPointZ, Multiply(viewProiectionMatrix, ViewportMatrix));
		endPointZ = Transform(endPointZ, Multiply(viewProiectionMatrix, ViewportMatrix));
		Novice::DrawLine((int)startPointZ.x, (int)startPointZ.y, (int)endPointZ.x, (int)endPointZ.y, 0x6F6F6FFF);
	}

}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return (Dot(v1, v2) / powf(Length(v2), 2), v2);
};

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

Vector3 MultiPly(float scalar, const Vector3& vector) {
	return { scalar * vector.x, scalar * vector.y, scalar * vector.z };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& ViewportMatrix, uint32_t color) {

	Vector3 center = MultiPly(plane.distance, plane.normal);

	// 平面の4つの頂点を計算
	Vector3 Perpendiculars[4];
	Perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	Perpendiculars[1] = { -Perpendiculars[0].x,-Perpendiculars[0].y,-Perpendiculars[0].z };
	Perpendiculars[2] = Cross(plane.normal, Perpendiculars[0]);
	Perpendiculars[3] = { -Perpendiculars[2].x,-Perpendiculars[2].y,-Perpendiculars[2].z };

	Vector3 points[4];
	// ビュープロジェクション行列とビューポート行列で各頂点を変換
	for (uint32_t index = 0; index < 4; ++index) {
		Vector3 extend = MultiPly(2.0f, Perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), ViewportMatrix);
	}

	// 平面の線を描画
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 camaraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };



	Segment segment{};
	segment.diff.x = 1.2f;
	segment.diff.y = 0.5f;

	Plane plane{};
	plane.normal = { 0.0f,1.0f,0.0f };
	plane.distance = 1.0f;

	bool fige = false;

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		Matrix4x4 viewWorldMatrix = Inverse(worldMatrix);

		Matrix4x4 cameraMatrxi = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, camaraTranslate);
		Matrix4x4 viewCameraMatrix = Inverse(cameraMatrxi);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewWorldMatrix, Multiply(viewCameraMatrix, projectionMatrix));
		Matrix4x4 ViewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		
		Vector3 start = Transform(Transform(segment.origin, ViewProjectionMatrix), ViewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), ViewProjectionMatrix), ViewportMatrix);
		
		// 線と面の衝突判定
		if (IsCollision(segment, plane)) {
			// 衝突したら
			fige = true;
		} else {
			// 衝突しなかったら
			fige = false;
		}

		ImGui::Begin("Window");
		ImGui::DragFloat3("segment.segment", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("Plane.distance", &plane.distance, 0.01f);

		plane.normal = Normalize(plane.normal);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(ViewProjectionMatrix, ViewportMatrix);

		DrawPlane(plane, ViewProjectionMatrix, ViewportMatrix, WHITE);

		if (fige == true) {
			Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, RED);

		} else if (fige == false) {
			Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, WHITE);


		}

		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}