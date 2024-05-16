#include <Novice.h>
#include<cmath>
#include<assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include<imgui.h>

static const int KRowHeight = 20;
static const int Kcolumnwidth = 60;

const char kWindowTitle[] = "LE2B_05_オノデラ_ユヅキ_タイトル";

struct Vector3 {
	float x, y, z;
};

struct Matrix4x4 {
	float m[4][4];
};


struct Sphere {
	Vector3 center;
	float radius;
};

struct Line {
	Vector3 origin;
	Vector3 diff;
};

struct Ray {
	Vector3 origin;
	Vector3 diff;
};

struct Segmet {
	Vector3 origin;
	Vector3 diff;
};



//加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = (v1.x + v2.x);
	result.y = (v1.y + v2.y);
	result.z = (v1.z + v2.z);
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



void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProiectionMatrix, const Matrix4x4& viewproiectionMatrix, uint32_t color) {

	const uint32_t KSubdivision = 10;
	const float KLonEvery = 2.0f * float(M_PI) / float(KSubdivision);//経度分割1つ分の角度
	const float KLatEvery = float(M_PI) / float(KSubdivision);//緯度分割1つ分の角度

	for (uint32_t latIndex = 0; latIndex <= KSubdivision; ++latIndex) {
		float Lat = -float(M_PI) / 2.0f + KLatEvery * latIndex;
		for (uint32_t LonIndex = 0; LonIndex <= KSubdivision; ++LonIndex) {
			float Lon = KLonEvery * LonIndex;

			Vector3 a, b, c;

			a = { sphere.center.x + sphere.radius * cos(Lon) * cos(Lat),
				sphere.center.y + sphere.radius * sin(Lon) * cos(Lat),
				sphere.center.z + sphere.radius * sin(Lat)
			};
			b = { sphere.center.x + sphere.radius * cos(Lon) * cos(Lat),
				sphere.center.y + sphere.radius * sin(Lon) * cos(Lat),
				sphere.center.z + sphere.radius * sin(Lat)
			};
			c = { sphere.center.x + sphere.radius * cos(Lon) * cos(Lat),
				sphere.center.y + sphere.radius * sin(Lon) * cos(Lat),
				sphere.center.z + sphere.radius * sin(Lat)
			};

			Vector3 viewMatrixA = Transform(a, viewProiectionMatrix);
			Vector3 viewprojectedMatrixA = Transform(viewMatrixA, viewproiectionMatrix);
			Vector3 viewMatrixB = Transform(b, viewProiectionMatrix);
			Vector3 viewprojectedMatrixB = Transform(viewMatrixB, viewproiectionMatrix);
			Vector3 viewMatrixC = Transform(c, viewProiectionMatrix);
			Vector3 viewprojectedMatrixC = Transform(viewMatrixC, viewproiectionMatrix);

			Novice::DrawLine(int(viewprojectedMatrixA.x), int(viewprojectedMatrixA.y),
				int(viewprojectedMatrixB.x), int(viewprojectedMatrixB.y), color);

			Novice::DrawLine(int(viewprojectedMatrixB.x), int(viewprojectedMatrixB.y),
				int(viewprojectedMatrixC.x), int(viewprojectedMatrixC.y), color);

		}

	}
}

//長さ（ノルム）
float Length(const Vector3& v) {
	float result;

	result = sqrtf(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2));

	return result;
};

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


Vector3 Subtract(const Vector3& point, const Vector3& segmet) {
	Vector3 result;
	result.x = point.x - segmet.x;
	result.y = point.y - segmet.y;
	result.z = point.z - segmet.z;
	return result;

	return result;
};


float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float MagnitudeSquared(const Vector3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dotProduct = Dot(v1, v2);
	float magnitudeSquared = MagnitudeSquared(v2);
	Vector3 projection;
	projection = v2 * (dotProduct / magnitudeSquared);

	return projection;
};



Vector3 ClosestPoint(const Vector3& point, const Segmet& segmet) {
	Vector3 result;
	Vector3 direction = segmet.origin - segmet.diff;

	// 線分の始点から点へのベクトル
	Vector3 v1 = point - segmet.diff;

	// 線分の始点から終点へのベクトル
	Vector3 v2 = segmet.origin - segmet.diff;

	// v2 の長さの二乗
	float magnitudeSquared = MagnitudeSquared(v2);

	// v2 を v1 に射影する
	float t = Dot(v1, v2) / magnitudeSquared;

	// t が線分の始点側の外側にある場合
	if (t < 0.0f) {
		return segmet.diff;
	}
	// t が線分の終点側の外側にある場合
	else if (t > 1.0f) {
		return segmet.origin;
	}
	// t が線分の上にある場合
	else {
		// 線分上の点を計算
		return segmet.diff + direction * t;
	}
};




// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		Segmet segmet{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
		Vector3 point{-1.5f,0.6f,0.6f};
		Vector3 project = Project(Subtract(point, segmet.origin),segmet.diff);
		Vector3 closestPoint = ClosestPoint(point, segmet);

		Sphere pointSphere{ point,0.01f };
		Sphere closestPointSphere{ closestPoint,0.01f };

		

		//DrawSphere(pointSphere,);
		//Vector3 start = Transform(Transform(segmet.origin,))


		ImGui::Begin("Window");
		ImGui::InputFloat3("Project",&project.x,"%.3f",ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///


	

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