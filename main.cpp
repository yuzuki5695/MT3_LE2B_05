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
	
	Sphere sphere{};
	sphere.radius = 0.08f;

	float deltaTime = 1.0f / 60.0f;

	bool start = false;

	ConicalPendulum conicalPendulum;
	conicalPendulum.anchor = { 0.0f,1.0f,0.0f };
	conicalPendulum.lenght = 0.8f;
	conicalPendulum.halfApexAngle = 0.7f;
	conicalPendulum.angle = 0.0f;
	conicalPendulum.angularVelocity = 0.0f;

	Vector3 ball{};

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

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		Matrix4x4 viewWorldMatrix = Inverse(worldMatrix);

		Matrix4x4 cameraMatrxi = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, camaraTranslate);
		Matrix4x4 viewCameraMatrix = Inverse(cameraMatrxi);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewWorldMatrix, Multiply(viewCameraMatrix, projectionMatrix));
		Matrix4x4 ViewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		sphere.center = { ball };
		conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.lenght * std::cos(conicalPendulum.halfApexAngle)));


		if (start) {
			conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;
			float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.lenght;
			float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.lenght;

			ball.x = conicalPendulum.anchor.x + std::cos(conicalPendulum.angle) * radius;
			ball.y = conicalPendulum.anchor.y - height;
			ball.z = conicalPendulum.anchor.z - std::sin(conicalPendulum.angle) * radius;
		}

		ImGui::Begin("Window");	
		ImGui::Checkbox("Start", &start);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		DrawGrid(ViewProjectionMatrix, ViewportMatrix);

		DrawSphere(sphere,ViewProjectionMatrix, ViewportMatrix, WHITE);

		DrawLien(conicalPendulum.anchor, ball, ViewProjectionMatrix, ViewportMatrix, WHITE);

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