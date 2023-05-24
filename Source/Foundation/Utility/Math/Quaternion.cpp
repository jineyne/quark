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

FQuaternion::FQuaternion(const FVector3 &xAxis, const FVector3 &yAxis, const FVector3 &zAxis) {
    FMatrix4 mat = FMatrix4::Identity();

    mat[0][0] = xAxis.x;
    mat[1][0] = xAxis.y;
    mat[2][0] = xAxis.z;

    mat[0][1] = yAxis.x;
    mat[1][1] = yAxis.y;
    mat[2][1] = yAxis.z;

    mat[0][2] = zAxis.x;
    mat[1][2] = zAxis.y;
    mat[2][2] = zAxis.z;

    // Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
    // article "Quaternion Calculus and Fast Animation".

    float trace = mat[0][0]+mat[1][1]+mat[2][2];
    float root;

    if (trace > 0.0f) {
        // |w| > 1/2, may as well choose w > 1/2
        root = std::sqrt(trace + 1.0f);  // 2w
        w = 0.5f*root;
        root = 0.5f/root;  // 1/(4w)
        x = (mat[2][1]-mat[1][2])*root;
        y = (mat[0][2]-mat[2][0])*root;
        z = (mat[1][0]-mat[0][1])*root;
    } else {
        // |w| <= 1/2
        static uint32_t nextLookup[3] = { 1, 2, 0 };
        uint32_t i = 0;

        if (mat[1][1] > mat[0][0])
            i = 1;

        if (mat[2][2] > mat[i][i])
            i = 2;

        uint32_t j = nextLookup[i];
        uint32_t k = nextLookup[j];

        root = std::sqrt(mat[i][i]-mat[j][j]-mat[k][k] + 1.0f);

        float* cmpntLookup[3] = { &x, &y, &z };
        *cmpntLookup[i] = 0.5f*root;
        root = 0.5f/root;

        w = (mat[k][j]-mat[j][k])*root;
        *cmpntLookup[j] = (mat[j][i]+mat[i][j])*root;
        *cmpntLookup[k] = (mat[k][i]+mat[i][k])*root;
    }

    normalize();
}

FQuaternion::FQuaternion(float yaw, float roll, float pitch) {
    fromRollPitchYaw(yaw, roll, pitch);
}

FQuaternion FQuaternion::GetRotationFromTo(const FVector3 &from, const FVector3 &dest, const FVector3 &fallbackAxis) {
    // Based on Stan Melax's article in Game Programming Gems
    FQuaternion q;

    FVector3 v0 = from.normalized();
    FVector3 v1 = dest.normalized();

    float d = v0.dot(v1);

    // If dot == 1, vectors are the same
    if (d >= 1.0f) {
        return FQuaternion();
    }

    if (d < (1e-6f - 1.0f)) {
        if (fallbackAxis != FVector3::ZeroVector) {
            // Rotate 180 degrees about the fallback axis
            q.fromAxisAngle(fallbackAxis, FRadian(FMath::PI));
        } else {
            // Generate an axis
            FVector3 axis = FVector3::Forward.cross(from);

            // Pick another if colinear
            if (axis.length() == 0) {
                axis = FVector3::Up.cross(from);
            }

            axis = axis.normalized();
            q.fromAxisAngle(axis, FRadian(FMath::PI));
        }
    } else {
        float s = std::sqrt((1+d)*2);
        float invs = 1 / s;

        FVector3 c = v0.cross(v1);

        q.x = c.x * invs;
        q.y = c.y * invs;
        q.z = c.z * invs;
        q.w = s * 0.5f;
        q.normalize();
    }

    return q;
}

void FQuaternion::fromAxisAngle(const FVector3 &axis, FRadian radian) {
    FRadian half = 0.5f * radian;
    float s = std::sin(half);

    x = s * axis.x;
    y = s * axis.y;
    z = s * axis.z;
    w = std::cos(half);
}

void FQuaternion::fromRollPitchYaw(float yaw, float roll, float pitch) {
    float halfRoll = roll * 0.5f;
    float halfPitch = pitch * 0.5f;
    float halfYaw = yaw * 0.5f;

    float cr = std::cos(halfRoll);
    float sr = std::sin(halfRoll);

    float cp = std::cos(halfPitch);
    float sp = std::sin(halfPitch);

    float cy = std::cos(halfYaw);
    float sy = std::sin(halfYaw);

    float qx = cr * sp * cy + sr * cp * sy;
    float qy = cr * cp * sy - sr * sp * cy;
    float qz = sr * cp * cy - cr * sp * sy;
    float qw = cr * cp * cy + sr * sp * sy;

    *this = FQuaternion(qx, qy, qz, qw);
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
    FQuaternion v = FQuaternion(vec.x, vec.y, vec.z, 0);

    FQuaternion conjugate = { -x, -y, -z, w };
    FQuaternion result = v * conjugate;
    result = *this * result;
    return FVector3(result.x, result.y, result.z);
}

void FQuaternion::lookRotation(const FVector3 &forward) {
    if (forward == FVector3::ZeroVector)
        return;

    FVector3 nrmForwardDir = forward.normalized();
    FVector3 currentForwardDir = -zAxis();

    if ((nrmForwardDir + currentForwardDir).lengthSquared() < 0.00005f)
    {
        *this = FQuaternion(-y, -z, w, x);
    } else {
        FQuaternion rotQuat = GetRotationFromTo(currentForwardDir, nrmForwardDir);
        *this = rotQuat * *this;
    }
}

void FQuaternion::lookRotation(const FVector3 &forward, const FVector3 &up) {
    FVector3 f = forward.normalized();
    FVector3 u = up.normalized();

    if (FMath::ApproxEquals(FVector3::Dot(f, u), 1.0f)) {
        lookRotation(forward);
        return;
    }

    FVector3 x = FVector3::Cross(u, f);
    x = x.normalized();

    FVector3 y = FVector3::Cross(f, x);
    y = y.normalized();

    *this = FQuaternion(x, y, f);
}

float FQuaternion::normalize(float tolerance) {
    float len = std::sqrt(dot(*this));
    if (len > (tolerance * tolerance)) {
        *this = *this * (1.0f / len);
    }

    return len;
}

FQuaternion FQuaternion::normalized() const {
    auto temp = *this;
    temp.normalize();
    return temp;
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

FVector3 FQuaternion::zAxis() const {
    float fTx  = 2.0f*x;
    float fTy  = 2.0f*y;
    float fTz  = 2.0f*z;
    float fTwx = fTx*w;
    float fTwy = fTy*w;
    float fTxx = fTx*x;
    float fTxz = fTz*x;
    float fTyy = fTy*y;
    float fTyz = fTz*y;

    return FVector3(fTxz+fTwy, fTyz-fTwx, 1.0f-(fTxx+fTyy));
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
