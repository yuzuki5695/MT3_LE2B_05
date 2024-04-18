#include <Novice.h>
static const int KRowHeight = 20;
static const int Kcolumnwidth = 60;

const char kWindowTitle[] = "LE2B_05_オノデラ_ユヅキ_タイトル";

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


//1.行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int v = 0; v < 4; ++v) {
			result.m[i][v] = m1.m[i][v] + m2.m[i][v];
		}
	}

	return result;
};


//2.行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int v = 0; v < 4; ++v) {
			result.m[i][v] = m1.m[i][v] - m2.m[i][v];
		}
	}

	return result;
};


//3.行列の積
Matrix4x4 Multiply(const Matrix4x4 m1, const Matrix4x4 m2) {
	Matrix4x4 result;


	result.m[0][0] =
		m1.m[0][0] * m2.m[0][0] +
		m1.m[0][1] * m2.m[1][0] +
		m1.m[0][2] * m2.m[2][0] +
		m1.m[0][3] * m2.m[3][0];
	result.m[0][1] =
		m1.m[0][0] * m2.m[0][1] +
		m1.m[0][1] * m2.m[1][1] +
		m1.m[0][2] * m2.m[2][1] +
		m1.m[0][3] * m2.m[3][1];
	result.m[0][2] =
		m1.m[0][0] * m2.m[0][2] +
		m1.m[0][1] * m2.m[1][2] +
		m1.m[0][2] * m2.m[2][2] +
		m1.m[0][3] * m2.m[3][2];
	result.m[0][3] =
		m1.m[0][0] * m2.m[0][3] +
		m1.m[0][1] * m2.m[1][3] +
		m1.m[0][2] * m2.m[2][3] +
		m1.m[0][3] * m2.m[3][3];


	result.m[1][0] =
		m1.m[1][0] * m2.m[0][0] +
		m1.m[1][1] * m2.m[1][0] +
		m1.m[1][2] * m2.m[2][0] +
		m1.m[1][3] * m2.m[3][0];
	result.m[1][1] =
		m1.m[1][0] * m2.m[0][1] +
		m1.m[1][1] * m2.m[1][1] +
		m1.m[1][2] * m2.m[2][1] +
		m1.m[1][3] * m2.m[3][1];
	result.m[1][2] =
		m1.m[1][0] * m2.m[0][2] +
		m1.m[1][1] * m2.m[1][2] +
		m1.m[1][2] * m2.m[2][2] +
		m1.m[1][3] * m2.m[3][2];
	result.m[1][3] =
		m1.m[1][0] * m2.m[0][3] +
		m1.m[1][1] * m2.m[1][3] +
		m1.m[1][2] * m2.m[2][3] +
		m1.m[1][3] * m2.m[3][3];

	result.m[2][0] =
		m1.m[2][0] * m2.m[0][0] +
		m1.m[2][1] * m2.m[1][0] +
		m1.m[2][2] * m2.m[2][0] +
		m1.m[2][3] * m2.m[3][0];
	result.m[2][1] =
		m1.m[2][0] * m2.m[0][1] +
		m1.m[2][1] * m2.m[1][1] +
		m1.m[2][2] * m2.m[2][1] +
		m1.m[2][3] * m2.m[3][1];
	result.m[2][2] =
		m1.m[2][0] * m2.m[0][2] +
		m1.m[2][1] * m2.m[1][2] +
		m1.m[2][2] * m2.m[2][2] +
		m1.m[2][3] * m2.m[3][2];
	result.m[2][3] =
		m1.m[2][0] * m2.m[0][3] +
		m1.m[2][1] * m2.m[1][3] +
		m1.m[2][2] * m2.m[2][3] +
		m1.m[2][3] * m2.m[3][3];

	result.m[3][0] =
		m1.m[3][0] * m2.m[0][0] +
		m1.m[3][1] * m2.m[1][0] +
		m1.m[3][2] * m2.m[2][0] +
		m1.m[3][3] * m2.m[3][0];
	result.m[3][1] =
		m1.m[3][0] * m2.m[0][1] +
		m1.m[3][1] * m2.m[1][1] +
		m1.m[3][2] * m2.m[2][1] +
		m1.m[3][3] * m2.m[3][1];
	result.m[3][2] =
		m1.m[3][0] * m2.m[0][2] +
		m1.m[3][1] * m2.m[1][2] +
		m1.m[3][2] * m2.m[2][2] +
		m1.m[3][3] * m2.m[3][2];
	result.m[3][3] =
		m1.m[3][0] * m2.m[0][3] +
		m1.m[3][1] * m2.m[1][3] +
		m1.m[3][2] * m2.m[2][3] +
		m1.m[3][3] * m2.m[3][3];

	return result;
};



//4.逆行列
Matrix4x4 Inverse(const Matrix4x4& matrix) {
	Matrix4x4 result;

	float det;
	det =
		((matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3]) +
			(matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1]) +
			(matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2]) +

			(matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) +
			(matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3]) +
			(matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0]) +

			(matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3]) +
			(matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0]) +
			(matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1]) +

			(matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) +
			(matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2]) +
			(matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0]) -


			(matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) -
			(matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3]) -
			(matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1]) -

			(matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3]) -
			(matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0]) -
			(matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2]) -

			(matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) -
			(matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3]) -
			(matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0]) -

			(matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2]) -
			(matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0]) -
			(matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1]));


	Matrix4x4 result;
	result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] - matrix.m[2][1] * matrix.m[1][2]) / det;
	result.m[0][1] = (matrix.m[0][2] * matrix.m[2][1] - matrix.m[2][2] * matrix.m[0][1]) / det;
	result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] - matrix.m[1][1] * matrix.m[0][2]) / det;
	result.m[0][3] = (matrix.m[0][1] * matrix.m[1][2] - matrix.m[1][1] * matrix.m[0][2]) / det;

	return result;
};

//5.転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;

	result.m[0][0] = m.m[0][0]; result.m[0][1] = m.m[1][0]; result.m[0][2] = m.m[2][0]; result.m[0][3] = m.m[3][0];
	result.m[1][0] = m.m[0][1]; result.m[1][1] = m.m[1][1]; result.m[1][2] = m.m[2][1]; result.m[1][3] = m.m[3][1];
	result.m[2][0] = m.m[0][2]; result.m[2][1] = m.m[1][2]; result.m[2][2] = m.m[2][2]; result.m[2][3] = m.m[3][2];
	result.m[3][0] = m.m[0][3]; result.m[3][1] = m.m[1][3]; result.m[3][2] = m.m[2][3]; result.m[3][3] = m.m[3][3];
	return result;
};

//6.単位行列の作成
Matrix4x4 MakeTdentity4x4() {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int v = 0; v < 4; ++v) {
			if (v == i) {
				result.m[i][v] = 1;
			}
			else {
				result.m[i][v] = 0;
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


		Matrix4x4 m1 = {
			3.2f,0.7f,9.6f,4.4f,
			5.5f,1.3f,7.8f,2.1f,
			6.9f,8.0f,2.6f,1.0f,
			0.5f,7.2f,5.1f,3.3f };

		Matrix4x4 m2 = {
			4.1f,6.5f,3.3f,2.2f,
			8.8f,0.6f,9.9f,7.7f,
			1.1f,5.5f,6.6f,0.0f,
			3.3f,9.9f,8.8f,2.2f };

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 resultAdd = Add(m1, m2);
		Matrix4x4 resultSubtract = Subtract(m1, m2);
		Matrix4x4 resultMultiply = Multiply(m1, m2);
		//	Matrix4x4 inverseM1 = Inverse(m1);
		//	Matrix4x4 inverseM2 = Inverse(m2);
			//Matrix4x4 transposeM1 = Transpose(m1);
			//Matrix4x4 transposeM2 = Transpose(m2);
		Matrix4x4 identity = MakeTdentity4x4();
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, resultAdd);
		MatrixScreenPrintf(0, KRowHeight * 5, resultSubtract);
		MatrixScreenPrintf(0, KRowHeight * 5 * 2, resultMultiply);
		//	MatrixScreenPrintf(0, KRowHeight * 5 * 3, inverseM1);
		//	MatrixScreenPrintf(0, KRowHeight * 5 * 4, inverseM2);
		//	MatrixScreenPrintf(Kcolumnwidth * 5, 0, transposeM1);
		//	MatrixScreenPrintf(Kcolumnwidth * 5, KRowHeight * 5, transposeM2);
		MatrixScreenPrintf(Kcolumnwidth * 5, KRowHeight * 5 * 2, identity);

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
