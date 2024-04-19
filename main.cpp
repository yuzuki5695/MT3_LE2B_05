#include <Novice.h>
#include <math.h>
#include<cmath>

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


//1. X軸回転行列
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
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix,Multiply(rotateYMatrix, rotateZMatrix));

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
		
		Novice::ScreenPrintf(0, 0, "rotateXMatrix");
		MatrixScreenPrintf(0, 18, rotateXMatrix);

		Novice::ScreenPrintf(0, KRowHeight * 5, "rotateYMatrix");
		MatrixScreenPrintf(0,KRowHeight * 5 + 18, rotateYMatrix);

		Novice::ScreenPrintf(0, KRowHeight * 5 * 2, "rotateZMatrix");
		MatrixScreenPrintf(0, KRowHeight * 5 * 2+ 18, rotateZMatrix);


		Novice::ScreenPrintf(0, KRowHeight * 5 * 3, "rotateXYZMatrix");
		MatrixScreenPrintf(0, KRowHeight * 5 * 3 + 18, rotateXYZMatrix);

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
