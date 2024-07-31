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

struct Triangle {
	Vector3 vertices[3]; //!< 頂点
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
Vector3 Multiply(float scalar, const Vector3& vector) {
	return { scalar * vector.x, scalar * vector.y, scalar * vector.z };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& ViewportMatrix, uint32_t color) {

	Vector3 center = Multiply(plane.distance, plane.normal);

	// 平面の4つの頂点を計算
	Vector3 Perpendiculars[4];
	Perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	Perpendiculars[1] = { -Perpendiculars[0].x,-Perpendiculars[0].y,-Perpendiculars[0].z };
	Perpendiculars[2] = Cross(plane.normal, Perpendiculars[0]);
	Perpendiculars[3] = { -Perpendiculars[2].x,-Perpendiculars[2].y,-Perpendiculars[2].z };

	Vector3 points[4];
	// ビュープロジェクション行列とビューポート行列で各頂点を変換
	for (uint32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, Perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), ViewportMatrix);
	}

	// 平面の線を描画
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void DrawTriangle(const Triangle& triangle,const Matrix4x4& viewProjectionMatrixer, const Matrix4x4& ViewportMatrix, uint32_t color) {

	Vector3 transformedVertices[3];
	for (int i = 0; i < 3; ++i) {
		transformedVertices[i] = Transform(triangle.vertices[i], viewProjectionMatrixer);
	}
	
	Vector2 projectedVertices[3];
	for (int i = 0; i < 3; ++i) {
		projectedVertices[i] = ProjectTo2D(transformedVertices[i], ViewportMatrix);
	}

	// 三角形の3辺を描画
	Novice::DrawLine(int(projectedVertices[0].x), int(projectedVertices[0].y), int(projectedVertices[1].x), int(projectedVertices[1].y), color);
	Novice::DrawLine(int(projectedVertices[1].x), int(projectedVertices[1].y), int(projectedVertices[2].x), int(projectedVertices[2].y), color);
	Novice::DrawLine(int(projectedVertices[2].x), int(projectedVertices[2].y), int(projectedVertices[0].x), int(projectedVertices[0].y), color);
}


// 線と三角形の衝突判定
bool  IsCollision(const Triangle& triangle, const Segment& segment) {

	// 三角形の法線ベクトルを計算
	Vector3 v0v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v0v2 = Subtract(triangle.vertices[2], triangle.vertices[0]);

	Vector3 normal = Cross(v0v1, v0v2);
	float d = -Dot(normal, triangle.vertices[0]);

	float startDistance = Dot(normal, segment.origin) + d;
	float endDistance = Dot(normal,Add(segment.origin,segment.diff)) + d;

	if (startDistance * endDistance > 0) {
		return false;
	}

	float t = startDistance / (startDistance - endDistance);
	Vector3 intersection = {
		segment.origin.x + segment.diff.x * t,
		segment.origin.y + segment.diff.y * t,
		segment.origin.z + segment.diff.z * t
	};

	Vector3 v0p = Subtract(intersection,triangle.vertices[0]);	
	Vector3 v1p = Subtract(intersection, triangle.vertices[1]);
	Vector3 v2p = Subtract(intersection, triangle.vertices[2]);

	Vector3 cross01 = Cross(v0v1, v0p);
	Vector3 cross12 = Cross(Subtract(triangle.vertices[2], triangle.vertices[1]), v1p);
	Vector3 cross20 = Cross(Subtract(triangle.vertices[0], triangle.vertices[2]),v2p);

	if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;
}


void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 start = Transform(segment.origin, viewProjectionMatrix);
	Vector3 end = Transform({ segment.origin.x + segment.diff.x,segment.origin.y + segment.diff.y,segment.origin.z + segment.diff.z }, viewProjectionMatrix);
	Vector2 projectedStart = ProjectTo2D(start, viewportMatrix);
	Vector2 projectedEnd = ProjectTo2D(end, viewportMatrix);

	Novice::DrawLine(int(projectedStart.x), int(projectedStart.y), int(projectedEnd.x), int(projectedEnd.y), color);
}