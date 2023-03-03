#include "DX11BufferManager.h"
#include "RenderAPI/DX11GpuBuffer.h"
#include "RenderAPI/DX11IndexBuffer.h"
#include "RenderAPI/DX11VertexBuffer.h"

FVertexBuffer *FDX11BufferManager::createVertexBufferInternal(const FVertexBufferDesc &desc) {
    return new FDX11VertexBuffer(desc);
};

FIndexBuffer *FDX11BufferManager::createIndexBufferInternal(const FIndexBufferDesc &desc) {
    return new FDX11IndexBuffer(desc);
}

FGpuBuffer *FDX11BufferManager::createGpuBufferInternal(const FGpuBufferDesc &desc) {
    return new FDX11GpuBuffer(desc);
}
