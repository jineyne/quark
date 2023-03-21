#include "DX11BufferManager.h"

#include "RenderAPI/DX11GpuBuffer.h"
#include "RenderAPI/DX11GpuParamBlockBuffer.h"
#include "RenderAPI/DX11IndexBuffer.h"
#include "RenderAPI/DX11VertexBuffer.h"

FVertexBuffer *FDX11BufferManager::createVertexBufferInternal(const FVertexBufferDesc &desc) {
    return q_new<FDX11VertexBuffer>(desc);
};

FIndexBuffer *FDX11BufferManager::createIndexBufferInternal(const FIndexBufferDesc &desc) {
    return q_new<FDX11IndexBuffer>(desc);
}

FGpuParamBlockBuffer *FDX11BufferManager::createGpuParamBlockBufferInternal(uint32_t size, EBufferUsage usage) {
    return q_new<FDX11GpuParamBlockBuffer>(size, usage);
}

FGpuBuffer *FDX11BufferManager::createGpuBufferInternal(const FGpuBufferDesc &desc) {
    return q_new<FDX11GpuBuffer>(desc);
}
