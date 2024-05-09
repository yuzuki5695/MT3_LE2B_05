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

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * Kcolumnwidth, y + row * KRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}

}




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
Matrix4x4 MakeAftineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
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



//4.逆行列
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
				int(viewprojectedMatrixB.x), int(viewprojectedMatrixB.y),color);

			Novice::DrawLine(int(viewprojectedMatrixB.x), int(viewprojectedMatrixB.y),
				int(viewprojectedMatrixC.x), int(viewprojectedMatrixC.y), color);

		}

	}
}





void DrawGrid(const Matrix4x4& viewProiectionMatrix, const Matrix4x4& viewproiectionMatrix) {
	const float KGridHalfwidth = 2.0f;
	const uint32_t KSubdivision = 10;
	const float KGridEvery = (KGridHalfwidth * 2.0f) / float(KSubdivision);

	//奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= KSubdivision; ++xIndex) {
		float xCoord = -KGridHalfwidth + xIndex * KGridEvery;

		Vector3 startPoint(xCoord, 0.0f, -KGridHalfwidth);
		Vector3 endPoint(xCoord, 0.0f, KGridHalfwidth);

		Vector3 transformedStartPoint = Transform(startPoint, viewProiectionMatrix);
		Vector3 transformedEndPoint = Transform(endPoint, viewProiectionMatrix);

		Vector3 screenStartPoint = Transform(transformedStartPoint, viewproiectionMatrix);
		Vector3 screenEndPoint = Transform(transformedEndPoint, viewproiectionMatrix);

		Novice::DrawLine(int(screenStartPoint.x), int(screenStartPoint.y), int(screenEndPoint.x), int( screenEndPoint.y), 0XAAAAAAFF);
	}

	for (uint32_t zIndex = 0; zIndex <= KSubdivision; ++zIndex) {
		float zCoord = -KGridHalfwidth + zIndex * KGridEvery;

		Vector3 startPoint(-KGridHalfwidth, 0.0f, zCoord);
		Vector3 endPoint(KGridHalfwidth, 0.0f, zCoord);


		Vector3 transformedStartPoint = Transform(startPoint, viewProiectionMatrix);
		Vector3 transformedEndPoint = Transform(endPoint, viewProiectionMatrix);

		Vector3 screenStartPoint = Transform(transformedStartPoint, viewproiectionMatrix);
		Vector3 screenEndPoint = Transform(transformedEndPoint, viewproiectionMatrix);

		Novice::DrawLine(int(screenStartPoint.x), int(screenStartPoint.y), int( screenEndPoint.x), int(screenEndPoint.y), 0XAAAAAAFF);
	}
}

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

		Vector3  cameraTranslate{ 0.0f,1.9f,-6.49f };
		Vector3  cameraRotare{ 0.26f,0.0f,0.0f };	
		Vector3 cameraPosition{};

		Matrix4x4 worldMatrix = MakeAftineMatrix({ 1.0f,1.0f,1.0f }, cameraRotare, cameraTranslate);
		Matrix4x4 cameraMatrix = MakeAftineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f,0.0f,0.0f }, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Sphere Sphere;

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotare", &cameraRotare.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &Sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &Sphere.radius, 0.01f);
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
	
		
		DrawGrid(viewMatrix, viewMatrix);

		DrawSphere(Sphere, viewMatrix, viewMatrix, 0XFFFFFFFF);


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