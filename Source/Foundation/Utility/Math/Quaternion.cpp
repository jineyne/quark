#include "Quaternion.h"
#include "Matrix4.h"

FQuaternion operator*(float lhs, const FQuaternion &rhs) {
    return FQuaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

FQuaternion FQuaternion::Zero(0, 0, 0, 0);

FQuaternion::FQuaternion(float value) : x(value), y(value), z(value), w(value) {}

FQuaternion::FQuaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

FQuaternion::FQuaternion(const FVector3 &axis, FRadian angle) {
    fromAxisAngle(axis, angle);
}

FQuaternion::FQuaternion(FRadian xAngle, FRadian yAngle, FRadian zAngle) {
    fromEulerAngles(xAngle, yAngle, zAngle);
}

void FQuaternion::fromAxisAngle(const FVector3 &axis, FRadian radian) {
    FRadian half = 0.5f * radian;
    float s = std::sin(half);

    x = s * axis.x;
    y = s * axis.y;
    z = s * axis.z;
    w = std::cos(half);
}

void FQuaternion::fromEulerAngles(FRadian xAngle, FRadian yAngle, FRadian zAngle) {
    float halfX = xAngle * 0.5f;
    float halfY = yAngle * 0.5f;
    float halfZ = zAngle * 0.5f;

    float cx = std::cos(halfX);
    float sx = std::sin(halfX);

    float cy = std::cos(halfY);
    float sy = std::sin(halfY);

    float cz = std::cos(halfZ);
    float sz = std::sin(halfZ);

    FQuaternion quatX(sx, 0.0f, 0.0f, cx);
    FQuaternion quatY(0.0f, sy, 0.0f, cy);
    FQuaternion quatZ(0.0f, 0.0f, sz, cz);

    *this = quatZ * (quatX * quatY);
}

FQuaternion FQuaternion::operator+(const FQuaternion &rhs) const {
    return FQuaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
}

FQuaternion FQuaternion::operator-(const FQuaternion &rhs) const {
    return FQuaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.z);
}

FQuaternion FQuaternion::operator*(const FQuaternion &rhs) const {
    return FQuaternion(
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
            w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
}

FVector3 FQuaternion::operator*(const FVector3 &rhs) const {
    float n1 = x * 2.f;
    float n2 = y * 2.f;
    float n3 = z * 2.f;

    float n4 = x * n1;
    float n5 = y * n2;
    float n6 = z * n3;

    float n7 = x * n2;
    float n8 = x * n3;

    float n9 = y * n3;

    float n10 = w * n1;
    float n11 = w * n2;
    float n12 = w * n3;

    FVector3 result;
    result.x = (1.f - (n5 + n6)) * rhs.x + (n7 - n12) * rhs.y + (n8 + n11) * rhs.z;
    result.y = (n7 + n12) * rhs.x + (1.f - (n4 + n6)) * rhs.y + (n9 - n10) * rhs.z;
    result.z = (n8 - n11) * rhs.x + (n9 + n10) * rhs.y + (1.f - (n4 + n5)) * rhs.z;

    return result;
}

FQuaternion FQuaternion::operator*(float rhs) const {
    return FQuaternion(rhs * x, rhs * y, rhs * z, rhs * w);
}

FQuaternion FQuaternion::operator/(float rhs) const {
    assert(rhs != 0.0);

    float inv = 1.0f / rhs;
    return FQuaternion(x * inv, y * inv, z * inv, w * inv);
}

FQuaternion FQuaternion::operator-() const {
    return FQuaternion(-x, -y, -z, -w);
}

bool FQuaternion::operator==(const FQuaternion &rhs) const {
    return (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (w == rhs.w);
}

bool FQuaternion::operator!=(const FQuaternion &rhs) const {
    return !(*this == rhs);
}

FQuaternion &FQuaternion::operator+=(const FQuaternion &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;

    return *this;
}

FQuaternion &FQuaternion::operator-=(const FQuaternion &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;

    return *this;
}

FQuaternion &FQuaternion::operator*=(const FQuaternion &rhs) {
    float newX = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
    float newY = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
    float newZ = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;
    float newW = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;

    x = newX; y = newY; z = newZ; w = newW;

    return *this;
}

float FQuaternion::dot(const FQuaternion &quat) const {
    return  x * quat.x + y * quat.y + z * quat.z + w * quat.w;
}

FVector3 FQuaternion::rotate(const FVector3 &vec) const {
    auto result = FMatrix4::Rotate(*this) * FVector4(vec, 0);
    return FVector3(result.x, result.y, result.z);
}

float FQuaternion::normalize(float tolerance) {
    float len = std::sqrt(dot(*this));
    if (len > (tolerance * tolerance)) {
        *this = *this * (1.0f / len);
    }

    return len;
}

FQuaternion FQuaternion::inverse() const {
    float fNorm = x * x + y * y + z * z + w * w;
    if (fNorm > 0.0f) {
        float fInvNorm = 1.0f / fNorm;
        return FQuaternion(-x * fInvNorm, -y * fInvNorm, -z * fInvNorm, w * fInvNorm);
    } else {
        return FQuaternion();
    }
}

FVector3 FQuaternion::toEulerAngles() const {
    FVector3 result(0.0f, 0.0, 0.0);

    float sinr_cosp = 2 * (w * x + y * z);
    float cosr_cosp = 1 - 2 * (x * x + y * y);

    result.x = atan2(sinr_cosp, cosr_cosp);

    float sinp = 2 * (w * y - z * x);
    if (abs(sinp) >= 1) {
        result.y = copysign(FMath::PI / 2, sinp);
    } else {
        result.y = asin(sinp);
    }

    float siny_cosp = 2 * (w * z + x * y);
    float cosy_cosp = 1 - 2 * (y * y + z * z);
    result.z = atan2(siny_cosp, cosy_cosp);

    return result;
}
