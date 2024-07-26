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

	Plane plane{};
	plane.normal = Normalize({ -0.2f,0.9f,-0.3f });
	plane.distance = 0.0f;

	Ball ball{};
	ball.position = {1.0f, 1.2f, 0.3f};
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;
	ball.acceleration = { 0.0f,-9.8f,0.0f };

	float deltaTime = 1.0f / 60.0f;
	float e = 0.02f;
	bool start = false;

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

	
		if (start) {
			ball.velocity.x += ball.acceleration.x * deltaTime;
			ball.velocity.y += ball.acceleration.y * deltaTime;
			ball.velocity.z += ball.acceleration.z * deltaTime;

			ball.position.x += ball.velocity.x * deltaTime;
			ball.position.y += ball.velocity.y * deltaTime;
			ball.position.z += ball.velocity.z * deltaTime;

			if (IsCollision(Sphere{ ball.position,ball.radius }, plane)) {
				Vector3 normalizedNormal = Normalize(plane.normal);
				float penetrationDepth = (ball.radius - Dot(normalizedNormal, ball.position) + plane.distance);
				
				// 球を平面から押す
				ball.position.x += normalizedNormal.x * penetrationDepth;
				ball.position.y += normalizedNormal.y * penetrationDepth;
				ball.position.z += normalizedNormal.z * penetrationDepth;	
				
				Vector3 reflected = Reflect(ball.velocity, plane.normal);
				Vector3 projectToNormal = Project(reflected, plane.normal);
				Vector3 movingDirection = {
				reflected.x - projectToNormal.x,
				reflected.y - projectToNormal.y,
				reflected.z - projectToNormal.z,
				};

				ball.velocity.x = projectToNormal.x * e + movingDirection.x;
				ball.velocity.y = projectToNormal.y * e + movingDirection.y;
				ball.velocity.z = projectToNormal.z * e + movingDirection.z;
			}
		} 

		ImGui::Begin("Window");	
		ImGui::Checkbox("Start", &start);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		DrawGrid(ViewProjectionMatrix, ViewportMatrix);

		DrawPlane(plane, ViewProjectionMatrix, ViewportMatrix,WHITE);

		DrawSphere(Sphere{ ball.position,ball.radius }, ViewProjectionMatrix, ViewportMatrix, WHITE);

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