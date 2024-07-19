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

	AABB aabb;
	aabb.min = { -0.5f,-0.5f,-0.5f };
	aabb.max = { 0.5f,0.5f,0.5f };

	Segment segment{};
	segment.origin ={ -0.7f,0.3f,0.0f};


	bool fige = false;

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

		if (keys[DIK_D]) {
			camaraTranslate.x += 0.01f;
		}
		if (keys[DIK_E]) {
			camaraTranslate.y += 0.01f;
		}
		if (keys[DIK_Q]) {
			camaraTranslate.z += 0.01f;
		}
		if (keys[DIK_W]) {
			cameraRotate.y -= 0.01f;
		}
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
		Matrix4x4 viewWorldMatrix = Inverse(worldMatrix);

		Matrix4x4 cameraMatrxi = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, camaraTranslate);
		Matrix4x4 viewCameraMatrix = Inverse(cameraMatrxi);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewWorldMatrix, Multiply(viewCameraMatrix, projectionMatrix));
		Matrix4x4 ViewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		
		Vector3 start = Transform(Transform(segment.origin, ViewProjectionMatrix), ViewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), ViewProjectionMatrix), ViewportMatrix);

		
		// AABBと線の衝突判定
		if (IsCollision(aabb,segment)) {
			// 衝突したら
			fige = true;
		} else {
			// 衝突しなかったら
			fige = false;
		}

		ImGui::Begin("Window");
		ImGui::DragFloat3("AABB.Min", &aabb.min.x, 0.01f);
		ImGui::DragFloat3("AABB.Max", &aabb.max.x, 0.01f);
		ImGui::DragFloat3("Segment.segment", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Segment.diff", &segment.diff.x, 0.01f);

		ImGui::Checkbox("fige",&fige);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(ViewProjectionMatrix, ViewportMatrix);

		Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, WHITE);

		if (fige == true) {
			DrawAABB(aabb, ViewProjectionMatrix, ViewportMatrix, RED);
		} else if (fige == false) {
			DrawAABB(aabb, ViewProjectionMatrix, ViewportMatrix, WHITE);
		}


		ImGui::End();

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