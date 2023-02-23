#pragma once

#include "DX11Prerequisites.h"
#include "Math/Matrix4.h"

#include <DirectXMath.h>

DirectX::XMMATRIX convertToDXMatrix(const FMatrix4 &m);