#pragma once
#include "../../BaseInc.h"

// RocketSim 3D vector struct
struct RS_ALIGN_16 Vec {
	float x, y, z;

	float _w; // 4th component to get compiler to use SIMD operations

	Vec() {
		x = y = z = _w = 0;
	}

	Vec(float x, float y, float z, float _w = 0) : x(x), y(y), z(z), _w(_w) {}

	Vec(const btVector3& bulletVec) {
		*(btVector3*)this = bulletVec;
	}

	bool IsZero() const {
		return (x == 0 && y == 0 && z == 0 && _w == 0);
	}

	float LengthSq() const {
		return (x * x + y * y + z * z + _w * _w);
	}

	float Length() const {
		float lengthSq = LengthSq();
		if (lengthSq > 0) {
			return sqrtf(lengthSq);
		} else {
			return 0;
		}
	}

	float Dot(const Vec& other) const {
		return (x * other.x + y * other.y + z * other.z + _w * other._w);
	}

	Vec Cross(const Vec& other) const {
		return Vec(
			 (y * other.z) - (z * other.y),
			-(x * other.z) - (z * other.x),
			 (x * other.y) - (y * other.x)
		);
	}

	float DistSq(const Vec& other) const {
		return (*this - other).LengthSq();
	}

	float Dist(const Vec& other) const {
		return sqrtf(DistSq(other));
	}

	float DistSq2D(const Vec& other) const {
		float dx = this->x - other.x;
		float dy = this->y - other.y;
		return (dx * dx + dy * dy);
	}

	float Dist2D(const Vec& other) const {
		return sqrtf(DistSq2D(other));
	}

	Vec Normalized() const {
		float length = Length();
		if (length > 0) {
			return *this / length;
		} else {
			return Vec();
		}
	}

	float& operator[](uint32_t index) {
		assert(index >= 0 && index < 3);
		return ((float*)this)[index];
	}

	float operator[](uint32_t index) const {
		assert(index >= 0 && index < 3);
		return ((float*)this)[index];
	}

	operator btVector3() const {
		return *(btVector3*)(this);
	}

	RSAPI Vec operator+(const Vec& other) const;
	RSAPI Vec operator-(const Vec& other) const;
	RSAPI Vec operator*(const Vec& other) const;
	RSAPI Vec operator/(const Vec& other) const;

	RSAPI Vec& operator+=(const Vec& other);
	RSAPI Vec& operator-=(const Vec& other);
	RSAPI Vec& operator*=(const Vec& other);
	RSAPI Vec& operator/=(const Vec& other);

	RSAPI Vec operator*(float val) const;
	RSAPI Vec operator/(float val) const;

	RSAPI Vec& operator*=(float val);
	RSAPI Vec& operator/=(float val);

	bool operator<(const Vec& other) const {
		return (x < other.x) && (y < other.y) && (z < other.z);
	}

	bool operator>(const Vec& other) const {
		return (x > other.x) && (y > other.y) && (z > other.z);
	}

	Vec operator-() const {
		return Vec(-x, -y, -z, -_w);
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vec& vec) {
		stream << "[ " << vec.x << ", " << vec.y << ", " << vec.z << " ]";
		return stream;
	}
};

// Vec needs to be equal in both size and structure layout to btVector3, because they are type-punned to and from
static_assert(sizeof(Vec) == sizeof(btVector3), "RocketSim Vec size must match btVector3 size");

// RocketSim 3x3 rotation matrix struct
// NOTE: Column-major
struct RS_ALIGN_16 RotMat {
	Vec forward, right, up;

	RotMat() {
		forward = right = up = Vec();
	}

	RotMat(Vec forward, Vec right, Vec up) : forward(forward), right(right), up(up) {}

	RotMat(const btMatrix3x3& bulletMat) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// NOTE: btMatrix3x3 is row-major, whereas we are column-major
				(*this)[i][j] = bulletMat[j][i];
			}
		}
	}

	static RotMat GetIdentity() {
		return RotMat(
			Vec(1, 0, 0),
			Vec(0, 1, 0),
			Vec(0, 0, 1)
		);
	}

	Vec operator[](uint32_t index) const {
		assert(index >= 0 && index < 3);
		return ((Vec*)(this))[index];
	}

	Vec& operator[](uint32_t index) {
		assert(index >= 0 && index < 3);
		return ((Vec*)(this))[index];
	}

	operator btMatrix3x3() const {
		btMatrix3x3 result;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				// NOTE: btMatrix3x3 is row-major, whereas we are column-major
				result[i][j] = (*this)[j][i];
			}
		}
		return result;
	}

	RSAPI RotMat operator+(const RotMat& other) const;
	RSAPI RotMat operator-(const RotMat& other) const;

	RSAPI RotMat& operator+=(const RotMat& other);
	RSAPI RotMat& operator-=(const RotMat& other);

	RSAPI RotMat operator*(float val) const;
	RSAPI RotMat operator/(float val) const;

	RSAPI RotMat& operator*=(float val);
	RSAPI RotMat& operator/=(float val);

	Vec Dot(const Vec& vec) const {
		return Vec(
			vec.Dot(forward),
			vec.Dot(right),
			vec.Dot(up)
		);
	}

	RotMat Transpose() const {
		RotMat result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result[i][j] = (*this)[j][i];
		return result;
	}

	friend std::ostream& operator<<(std::ostream& stream, const RotMat& mat) {
		stream << "(FRU)(" << std::endl;
		for (int i = 0; i < 3; i++)
			stream << "\t" << mat[i] << (i < 2 ? "," : "") << std::endl;
		stream << ")";
		return stream;
	}
};

// RocketSim euler angle struct
// Has the same order of application as Rotators in Rocket League (YPR), values are in radians
struct Angle {
	float yaw, pitch, roll;

	Angle(float yaw = 0, float pitch = 0, float roll = 0) : yaw(yaw), pitch(pitch), roll(roll) {}

	RSAPI static Angle FromRotMat(RotMat mat);
	RSAPI RotMat ToRotMat() const;

	RSAPI Vec GetForwardVector() const;

	// Limits yaw/pitch/roll to [-pi,pi]/[-pi/2,pi/2]/[-pi,pi] while still representing the same rotation
	RSAPI void NormalizeFix();

	friend std::ostream& operator<<(std::ostream& stream, const Angle& ang) {
		stream << "(YPR)[ " << ang.yaw << ", " << ang.pitch << ", " << ang.roll << " ]";
		return stream;
	}
};