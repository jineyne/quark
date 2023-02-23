#include "Matrix4Functions.h"

DirectX::XMMATRIX convertToDXMatrix(const FMatrix4 &m) {
    DirectX::XMMATRIX dxmat;
    dxmat.r[0].m128_f32[0] = m[0][0], dxmat.r[0].m128_f32[1] = m[0][1], dxmat.r[0].m128_f32[2] = m[0][2], dxmat.r[0].m128_f32[3] = m[0][3];
    dxmat.r[1].m128_f32[0] = m[1][0], dxmat.r[1].m128_f32[1] = m[1][1], dxmat.r[1].m128_f32[2] = m[1][2], dxmat.r[1].m128_f32[3] = m[1][3];
    dxmat.r[2].m128_f32[0] = m[2][0], dxmat.r[2].m128_f32[1] = m[2][1], dxmat.r[2].m128_f32[2] = m[2][2], dxmat.r[2].m128_f32[3] = m[2][3];
    dxmat.r[3].m128_f32[0] = m[3][0], dxmat.r[3].m128_f32[1] = m[3][1], dxmat.r[3].m128_f32[2] = m[3][2], dxmat.r[3].m128_f32[3] = m[3][3];
    return dxmat;
}