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

//1.透視投影行列　
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farclip) {
	Matrix4x4 result{};


	result.m[0][0] = (1.0f / aspectRatio) / tanf(fovY/ 2);result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = (1.0f /tanf(fovY / 2)); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = farclip /(farclip - nearClip) ; result.m[2][3] = 1.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = (-nearClip * farclip) / (farclip - nearClip ); result.m[3][3] = 0.0f;


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
Matrix4x4 MakeViewportMatrix(float left, float top, float  width,float height, float minDepth, float maxDepth) {
	Matrix4x4 result{};

	result.m[0][0] = (width / 2.0f); result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = -(height / 2.0f); result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = (maxDepth - minDepth); result.m[2][3] = 0.0f;
	result.m[3][0] = left + (width / 2); result.m[3][1] = top + (height / 2.0f); result.m[3][2] = minDepth; result.m[3][3] = 1.0f;



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

		Matrix4x4 orthographicMatrix = MakeOrthographicMatrix(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);
		Matrix4x4 perspectiveFovMatrix = MakePerspectiveFovMatrix(0.63f,1.33f,0.1f,1000.0f);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);
	

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

		Novice::ScreenPrintf(0, 0, "orthographicMatrix");
		MatrixScreenPrintf(0, 18, orthographicMatrix);

		Novice::ScreenPrintf(0, KRowHeight * 5, "perspectiveFovMatrix");
		MatrixScreenPrintf(0, KRowHeight * 5 + 18, perspectiveFovMatrix);

		Novice::ScreenPrintf(0, KRowHeight * 10, "viewportMatrix");
		MatrixScreenPrintf(0, KRowHeight * 10 + 18, viewportMatrix);

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