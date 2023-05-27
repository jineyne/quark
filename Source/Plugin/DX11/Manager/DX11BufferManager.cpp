#include "DX11BufferManager.h"

#include "RenderAPI/DX11GpuBuffer.h"
#include "RenderAPI/DX11GpuParamBlockBuffer.h"
#include "RenderAPI/DX11IndexBuffer.h"
#include "RenderAPI/DX11VertexBuffer.h"

VertexBuffer *DX11BufferManager::createVertexBufferInternal(const VertexBufferDesc &desc) {
    return q_new<DX11VertexBuffer>(desc);
};

IndexBuffer *DX11BufferManager::createIndexBufferInternal(const FIndexBufferDesc &desc) {
    return q_new<DX11IndexBuffer>(desc);
}

GpuParamBlockBuffer *DX11BufferManager::createGpuParamBlockBufferInternal(uint32_t size, EBufferUsage usage) {
    return q_new<DX11GpuParamBlockBuffer>(size, usage);
}

GpuBuffer *DX11BufferManager::createGpuBufferInternal(const GpuBufferDesc &desc) {
    return q_new<DX11GpuBuffer>(desc);
}
