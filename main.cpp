#include <Novice.h>
#include <math.h>
#include<cmath>
;

static const int Kcolumnwidth = 60;

const char kWindowTitle[] = "LE2B_05_オノデラ_ユヅキ_タイトル";

struct Vector3 {
	float x, y, z;
};

struct Matrix4x4 {
	float m[4][4];
};

//1. X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result{};
	float Ctheta = std::cos(radian);
	float Stheta = std::sin(radian);

	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = Ctheta; result.m[1][2] = Stheta; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = -Stheta; result.m[2][2] = Ctheta; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;


	return result;
}

//2. Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result{};
	float Ctheta = std::cos(radian);
	float Stheta = std::sin(radian);

	result.m[0][0] = Ctheta; result.m[0][1] = 0.0f; result.m[0][2] = -Stheta; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = Stheta; result.m[1][3] = 0.0f;
	result.m[2][0] = Stheta; result.m[2][1] = 0.0f; result.m[2][2] = Ctheta; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	return result;
}

//3. Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result{};
	float Ctheta = std::cos(radian);
	float Stheta = std::sin(radian);

	result.m[0][0] = Ctheta; result.m[0][1] = Stheta; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = -Stheta; result.m[1][1] = Ctheta; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;

	return result;

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

		Vector3 rotate{0.4f,1.43f,-0.8f};

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateXMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateXMatrix(rotate.z);

		//Matrix4x4 rotateMatrix = Multiply(rotateX,Multiply(rotateY,rotateZ));


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
