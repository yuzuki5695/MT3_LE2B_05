#pragma once
#include<Matrix.h>
#include<algorithm>

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

struct Triangle {
	Vector3 vertices[3]; //!< 頂点
};

struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
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


void DrawAABB(const AABB& aabb, const Matrix4x4& viewProiectionMatrix, const Matrix4x4& ViewportMatrix,uint32_t color) {

	// 頂点
	Vector3 vertices[8];
	vertices[0] = { aabb.min.x, aabb.min.y, aabb.min.z };
	vertices[1] = { aabb.min.x, aabb.min.y, aabb.max.z };
	vertices[2] = { aabb.min.x, aabb.max.y, aabb.min.z };
	vertices[3] = { aabb.min.x, aabb.max.y, aabb.max.z };
	vertices[4] = { aabb.max.x, aabb.min.y, aabb.min.z };
	vertices[5] = { aabb.max.x, aabb.min.y, aabb.max.z };
	vertices[6] = { aabb.max.x, aabb.max.y, aabb.min.z };
	vertices[7] = { aabb.max.x, aabb.max.y, aabb.max.z };

	// 頂点を変換する
	Vector3 transformedVertices[8];
	for (int i = 0; i < 8; ++i) {
		transformedVertices[i] = Transform(vertices[i], viewProiectionMatrix);
	}

	Vector2 projectedVertices[8];
	for (int i = 0; i < 8; ++i) {
		projectedVertices[i] = ProjectTo2D(transformedVertices[i], ViewportMatrix);
	}

	// 描画
	Novice::DrawLine(int(projectedVertices[0].x), int(projectedVertices[0].y), int(projectedVertices[1].x), int(projectedVertices[1].y), color); // Front bottom
	Novice::DrawLine(int(projectedVertices[1].x), int(projectedVertices[1].y), int(projectedVertices[3].x), int(projectedVertices[3].y), color); // Front top
	Novice::DrawLine(int(projectedVertices[3].x), int(projectedVertices[3].y), int(projectedVertices[2].x), int(projectedVertices[2].y), color); // Front top
	Novice::DrawLine(int(projectedVertices[2].x), int(projectedVertices[2].y), int(projectedVertices[0].x), int(projectedVertices[0].y), color); // Front bottom

	Novice::DrawLine(int(projectedVertices[4].x), int(projectedVertices[4].y), int(projectedVertices[5].x), int(projectedVertices[5].y), color); // Back bottom
	Novice::DrawLine(int(projectedVertices[5].x), int(projectedVertices[5].y), int(projectedVertices[7].x), int(projectedVertices[7].y), color); // Back top
	Novice::DrawLine(int(projectedVertices[7].x), int(projectedVertices[7].y), int(projectedVertices[6].x), int(projectedVertices[6].y), color); // Back top
	Novice::DrawLine(int(projectedVertices[6].x), int(projectedVertices[6].y), int(projectedVertices[4].x), int(projectedVertices[4].y), color); // Back bottom

	Novice::DrawLine(int(projectedVertices[0].x), int(projectedVertices[0].y), int(projectedVertices[4].x), int(projectedVertices[4].y), color); // Bottom
	Novice::DrawLine(int(projectedVertices[1].x), int(projectedVertices[1].y), int(projectedVertices[5].x), int(projectedVertices[5].y), color); // Bottom
	Novice::DrawLine(int(projectedVertices[2].x), int(projectedVertices[2].y), int(projectedVertices[6].x), int(projectedVertices[6].y), color); // Top
	Novice::DrawLine(int(projectedVertices[3].x), int(projectedVertices[3].y), int(projectedVertices[7].x), int(projectedVertices[7].y), color); // Top
}


// AABBと線の衝突判定
bool  IsCollision(const AABB& aabb, const Segment& segment) {

	Vector3 segmentDir = segment.diff;
	Vector3 segmentOrig = segment.origin;

	// AABBの面の最小値と最大値
	Vector3 aabbMin = aabb.min;
	Vector3 aabbMax = aabb.max;

	// 線分の方向ベクトルの逆数
	float invDirX = 1.0f / segmentDir.x;
	float invDirY = 1.0f / segmentDir.y;
	float invDirZ = 1.0f / segmentDir.z;

	// AABBの最小面と最大面で線分のパラメータ t の範囲を求める
	float t1 = (aabbMin.x - segmentOrig.x) * invDirX;
	float t2 = (aabbMax.x - segmentOrig.x) * invDirX;
	float t3 = (aabbMin.y - segmentOrig.y) * invDirY;
	float t4 = (aabbMax.y - segmentOrig.y) * invDirY;
	float t5 = (aabbMin.z - segmentOrig.z) * invDirZ;
	float t6 = (aabbMax.z - segmentOrig.z) * invDirZ;

	// t の範囲を計算
	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	// 線分がAABBに交差するかどうかを判断
	if (tmax >= std::max(0.0f, tmin)) {
		return true;
	}

	return false;
}