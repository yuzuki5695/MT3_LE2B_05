#include <Novice.h>
#include<cmath>
#include<assert.h>
#define _USE_MATH_DEFINES
#include <math.h>


static const int KRowHeight = 20;
static const int Kcolumnwidth = 60;

const char kWindowTitle[] = "LE2B_05_オノデラ_ユヅキ_タイトル";

struct Vector3 {
	float x, y, z;
};

struct Matrix4x4 {
	float m[4][4];
};


void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {

	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * Kcolumnwidth, y + row * KRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}

}

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
};


void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProiectionMatrix, const Matrix4x4& viewproiectionMatrix, uint32_t color) {





}




void DrawGrid(const Matrix4x4& viewProiectionMatrix, const Matrix4x4& viewproiectionMatrix) {
	const float KGridHalfwidth = 2.0f;
	const uint32_t KSubdivision = 10;
	const float KGridEvery = (KGridHalfwidth* 2.0f) / float(KSubdivision);
	//奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= KSubdivision; ++xIndex) {
		float xCoord = -KGridHalfwidth + xIndex * KGridEvery;

		Vector3 startPoint(xCoord, 0.0f, -KGridHalfwidth);
		Vector3 endPoint(xCoord, 0.0f, KGridHalfwidth);

		// 始点と終点をビュー空間および投影空間に変換する
		Vector3 transformedStartPoint = Transform(startPoint, viewProjectionMatrix);
		Vector3 transformedEndPoint = Transform(endPoint, viewProjectionMatrix);

		// 変換された座標をスクリーン座標系に変換する
		Vector3 screenStartPoint = Transform(transformedStartPoint, MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f));
		Vector3 screenEndPoint = Transform(transformedEndPoint, MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f));

		Novice::DrawLine(screenStartPoint.x, screenStartPoint.y, screenEndPoint.x, screenEndPoint.y, color);
	}

	for (uint32_t zIndex = 0; zIndex <= KSubdivision; ++zIndex) {
		float zCoord = -KGridHalfwidth + zIndex * KGridEvery;

		Vector3 startPoint(-KGridHalfwidth, 0.0f, zCoord);
		Vector3 endPoint(KGridHalfwidth, 0.0f, zCoord);

		
		Vector3 transformedStartPoint = Transform(startPoint, viewProjectionMatrix);
		Vector3 transformedEndPoint = Transform(endPoint, viewProjectionMatrix);

		Vector3 screenStartPoint = Transform(transformedStartPoint, MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f));
		Vector3 screenEndPoint = Transform(transformedEndPoint, MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f));

		Novice::DrawLine(screenStartPoint.x, screenStartPoint.y, screenEndPoint.x, screenEndPoint.y, color);
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