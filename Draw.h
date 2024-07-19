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


static void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 12;							//分割数
	const float kLatStep = (float)M_PI / kSubdivision;			//緯度のステップ
	const float kLonStep = 2.0f * (float)M_PI / kSubdivision;	//経度のステップ

	// 緯度のループ
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = -0.5f * (float)M_PI + latIndex * kLatStep;	//現在の緯度

		//次の緯度
		float nextLat = lat + kLatStep;

		//経度のループ
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			//現在の経度
			float lon = lonIndex * kLonStep;

			//次の経度
			float nextLon = lon + kLonStep;

			// 球面座標の計算
			Vector3 pointA
			{
				sphere.center.x + sphere.radius * cos(lat) * cos(lon),
				sphere.center.y + sphere.radius * sin(lat),
				sphere.center.z + sphere.radius * cos(lat) * sin(lon)
			};

			Vector3 pointB
			{
				sphere.center.x + sphere.radius * cos(nextLat) * cos(lon),
				sphere.center.y + sphere.radius * sin(nextLat),
				sphere.center.z + sphere.radius * cos(nextLat) * sin(lon)
			};

			Vector3 pointC
			{
				sphere.center.x + sphere.radius * cos(lat) * cos(nextLon),
				sphere.center.y + sphere.radius * sin(lat),
				sphere.center.z + sphere.radius * cos(lat) * sin(nextLon)
			};

			// スクリーン座標に変換
			pointA = Transform(pointA, Multiply(viewProjectionMatrix, viewportMatrix));
			pointB = Transform(pointB, Multiply(viewProjectionMatrix, viewportMatrix));
			pointC = Transform(pointC, Multiply(viewProjectionMatrix, viewportMatrix));

			// 線分の描画
			Novice::DrawLine((int)pointA.x, (int)pointA.y, (int)pointB.x, (int)pointB.y, color);
			Novice::DrawLine((int)pointA.x, (int)pointA.y, (int)pointC.x, (int)pointC.y, color);
		}
	}
}

// AABBと球の衝突判定
bool  IsCollision(const AABB& aabb, const  Sphere& sphere) {

	// 球の中心に最も近いAABBの点を計算
	Vector3 closestPoint = { 
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};

	// 球の中心とこの点との距離を計算
	float distanceX = sphere.center.x - closestPoint.x;
	float distanceY = sphere.center.y - closestPoint.y;
	float distanceZ = sphere.center.z - closestPoint.z;

	float distanceSquared = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;

	if (distanceSquared <= (sphere.radius * sphere.radius)) {
		return true;
	}	

	return false;
}