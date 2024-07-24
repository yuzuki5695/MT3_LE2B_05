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

	Spring spring{};
	spring.anchor = { 0.0f,0.0f,0.0f };
	spring.naturalLength = 1.0f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 1.2f,0.0f,0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	float deltaTime = 1.0f / 60.0f;

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

		sphere.center = { ball.position };
		sphere.radius = ball.radius;

		Vector3 diff = { 
		ball.position.x - spring.anchor.x,
		ball.position.y - spring.anchor.y, 
		ball.position.z - spring.anchor.z 
		};

		if (start) {
			float length = Length(diff);
			if (length != 0.0f) {
				Vector3 direction = Normalize(diff);
				Vector3 restPosition = {
				 spring.anchor.x + direction.x * spring.naturalLength,
				 spring.anchor.y + direction.y * spring.naturalLength,
				 spring.anchor.z + direction.z * spring.naturalLength,
				};
				Vector3 displacement = {
					length * (ball.position.x - restPosition.x),
					length * (ball.position.y - restPosition.y),
					length * (ball.position.z - restPosition.z),
				};

				Vector3 restoringForce = {
					-spring.stiffness * displacement.x,
					-spring.stiffness * displacement.y,
					-spring.stiffness * displacement.z
				};

				// 減衰抵抗を計算する
				Vector3 dampingForce = {
					-spring.dampingCoefficient * ball.velocity.x,
					-spring.dampingCoefficient * ball.velocity.y,
					-spring.dampingCoefficient * ball.velocity.z
				};
				//	減衰抵抗も加味して、物体にかかる力を決定する
				Vector3 force = {
					restoringForce.x + dampingForce.x,
					restoringForce.y + dampingForce.y,
					restoringForce.z + dampingForce.z
				};

				ball.acceleration.x = force.x / ball.mass;
				ball.acceleration.y = force.y / ball.mass;
				ball.acceleration.z = force.z / ball.mass;

				ball.velocity.x += ball.acceleration.x * deltaTime;
				ball.velocity.y += ball.acceleration.y * deltaTime;
				ball.velocity.z += ball.acceleration.z * deltaTime;

				ball.position.x += ball.velocity.x * deltaTime;
				ball.position.y += ball.velocity.y * deltaTime;
				ball.position.z += ball.velocity.z * deltaTime;
			}
		}

		if (!start){
			spring.anchor = { 0.0f,0.0f,0.0f };
			spring.naturalLength = 1.0f;
			spring.stiffness = 100.0f;
			spring.dampingCoefficient = 2.0f;

			ball.position = { 1.2f,0.0f,0.0f };
			ball.mass = 2.0f;
			ball.radius = 0.05f;
			ball.color = BLUE;
		}

		ImGui::Begin("Window");	
		ImGui::Checkbox("Start", &start);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		DrawGrid(ViewProjectionMatrix, ViewportMatrix);

		DrawSphere(sphere,ViewProjectionMatrix, ViewportMatrix, ball.color);

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