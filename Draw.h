#pragma once
#include<Matrix.h>

struct Sphere {
	Vector3 center; //!< 中心点
	float radius; //!< 半径
};

struct  Plane {
	Vector3 normal; //!< 法線
	float distance; //!< 距離
};

struct Segment {
	Vector3 origin; //!< 始点 
	Vector3 diff;   //!< 終点への差分ベクトル
};

Vector3 Project(const Vector3& v1, const Vector3& v2) {
	return (Dot(v1, v2) / powf(Length(v2), 2), v2);
};

Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

void DrawGrid(const Matrix4x4& viewProiectionMatrix, const Matrix4x4& ViewportMatrix) {
	const float KGridHalfwidth = 2.0f;
	const uint32_t KSubdivision = 10;
	const float KGridEvery = (KGridHalfwidth * 2.0f) / float(KSubdivision);

	for (uint32_t xIndex = 0; xIndex <= KSubdivision; xIndex++) {
		float posX = -KGridHalfwidth + xIndex * KGridEvery;

		Vector3 startPointX(posX, 0.0f, -KGridHalfwidth);
		Vector3 endPointX(posX, 0.0f, KGridHalfwidth);

		startPointX = Transform(startPointX, Multiply(viewProiectionMatrix, ViewportMatrix));
		endPointX = Transform(endPointX, Multiply(viewProiectionMatrix, ViewportMatrix));
		if (xIndex == 5) {
			Novice::DrawLine((int)startPointX.x, (int)startPointX.y, (int)endPointX.x, (int)endPointX.y, BLACK);
		} else {
			Novice::DrawLine((int)startPointX.x, (int)startPointX.y, (int)endPointX.x, (int)endPointX.y, 0x6F6F6FFF);
		}
	}
	for (uint32_t zIndex = 0; zIndex <= KSubdivision; zIndex++) {
		float posZ = -KGridHalfwidth + KGridEvery * zIndex;

		Vector3 startPointZ = { -KGridHalfwidth, 0.0f, posZ };
		Vector3 endPointZ = { KGridHalfwidth, 0.0f, posZ };
		startPointZ = Transform(startPointZ, Multiply(viewProiectionMatrix, ViewportMatrix));
		endPointZ = Transform(endPointZ, Multiply(viewProiectionMatrix, ViewportMatrix));
		if (zIndex == 5) {
			Novice::DrawLine((int)startPointZ.x, (int)startPointZ.y, (int)endPointZ.x, (int)endPointZ.y, BLACK);
		} else {
			Novice::DrawLine((int)startPointZ.x, (int)startPointZ.y, (int)endPointZ.x, (int)endPointZ.y, 0x6F6F6FFF);
		}
	}

}
Vector3 MultiPly(float scalar, const Vector3& vector) {
	return { scalar * vector.x, scalar * vector.y, scalar * vector.z };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& ViewportMatrix, uint32_t color) {

	Vector3 center = MultiPly(plane.distance, plane.normal);

	// 平面の4つの頂点を計算
	Vector3 Perpendiculars[4];
	Perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	Perpendiculars[1] = { -Perpendiculars[0].x,-Perpendiculars[0].y,-Perpendiculars[0].z };
	Perpendiculars[2] = Cross(plane.normal, Perpendiculars[0]);
	Perpendiculars[3] = { -Perpendiculars[2].x,-Perpendiculars[2].y,-Perpendiculars[2].z };

	Vector3 points[4];
	// ビュープロジェクション行列とビューポート行列で各頂点を変換
	for (uint32_t index = 0; index < 4; ++index) {
		Vector3 extend = MultiPly(2.0f, Perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), ViewportMatrix);
	}

	// 平面の線を描画
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}



// 線と平面の衝突判定
bool  IsCollision(const Segment& segment, const Plane& plane) {


	// 垂直判定を行うために、法線と線分の方向ベクトルの内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 線分が平面に平行（垂直）である場合は衝突しない
	if (dot == 0.0f) {
		return false;
	}

	// 平面と線分の始点から平面までの距離を計算

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// tの値が0から1の間にある場合、線分は平面と交差している
	return (t >= 0.0f && t <= 1.0f);

}

