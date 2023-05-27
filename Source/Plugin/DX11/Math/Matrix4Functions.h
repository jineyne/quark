#pragma once

#include "DX11Prerequisites.h"
#include "Math/Matrix4.h"

#include <DirectXMath.h>

DirectX::XMMATRIX convertToDXMatrix(const Matrix4 &m);