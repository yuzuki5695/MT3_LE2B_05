#include <Novice.h>
#include<cmath>
#include<assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include<imgui.h>
#include<Draw.h>

static const int KRowHeight = 20;
static const int Kcolumnwidth = 60;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

const char kWindowTitle[] = "LE2B_05_オノデラ_ユヅキ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 camaraTranslate = { 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate = { 0.26f,0.0f,0.0f };
	
	Vector3 translates[3] = {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f}
	};
	Vector3 rotates[3] = {
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f}
	};
	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f}
	};

	Sphere spheres[3] = {
	  { {0.0f, 0.0f, 0.0f}, 0.05f },
	  { {0.0f, 0.0f, 0.0f}, 0.05f },
	  { {0.0f, 0.0f, 0.0f}, 0.05f }	
	};

	uint32_t color[3]{
		RED,
		GREEN,
		BLUE
	};

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
		/// 

		ImGui::Begin("Window");
		ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotates[0]", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotates[1]", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scales[1]", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotates[2]", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scales[2]", &scales[0].x, 0.01f);
		ImGui::End();

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		Matrix4x4 viewWorldMatrix = Inverse(worldMatrix);

		Matrix4x4 cameraMatrxi = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, camaraTranslate);
		Matrix4x4 viewCameraMatrix = Inverse(cameraMatrxi);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewWorldMatrix, Multiply(viewCameraMatrix, projectionMatrix));
		Matrix4x4 ViewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		// SRTの作成
		Matrix4x4 worldMatrices[3];
		for (int i = 0; i < 3; ++i) {
			Matrix4x4 scaleMatrix = MakeScaleMatrix(scales[i]);
			Matrix4x4 translationMatrix = MakeTranslateMatrix(translates[i]);
			Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotates[i].x);
			Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotates[i].y);
			Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotates[i].z);
			Matrix4x4 rotateRMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

			Matrix4x4 localMatrix = Multiply(scaleMatrix, Multiply(rotateRMatrix, translationMatrix));
			if (i > 0) {
				worldMatrices[i] = Multiply(localMatrix,worldMatrices[i - 1]);
			} else {
				worldMatrices[i] = localMatrix;
			}

			spheres[i].center.x = worldMatrices[i].m[3][0];
			spheres[i].center.y = worldMatrices[i].m[3][1];
			spheres[i].center.z = worldMatrices[i].m[3][2];
		}

		Vector3 startLine[2]; 
		startLine[0] = Transform(Transform(spheres[0].center, ViewProjectionMatrix), ViewportMatrix);
		startLine[1] = Transform(Transform(spheres[1].center, ViewProjectionMatrix), ViewportMatrix);
		Vector3 endLine[2];
		endLine[0] = Transform(Transform(spheres[1].center, ViewProjectionMatrix), ViewportMatrix);
		endLine[1] = Transform(Transform(spheres[2].center, ViewProjectionMatrix), ViewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(ViewProjectionMatrix, ViewportMatrix);

		// 各球体,線を描画
		for (int i = 0; i < 3; ++i) {
			DrawSphere(spheres[i], ViewProjectionMatrix, ViewportMatrix, color[i]);
			
			if (i < 2) {
				Novice::DrawLine((int)startLine[i].x, (int)startLine[i].y, (int)endLine[i].x, (int)endLine[i].y, WHITE);
			}
		}

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