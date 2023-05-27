#pragma once

#include "DX11Prerequisites.h"
#include "RenderAPI/DX11GpuProgram.h"

class DX11_EXPORT DX11InputLayoutManager {
private:
    /**	Key uniquely identifying vertex declaration and vertex shader combination. */
    struct VertexDeclarationKey {
        uint32_t vertxDeclId = 0;
        uint32_t vertexProgramId = 0;
    };

    /**	Creates a hash from vertex declaration key. */
    class HashFunc {
    public:
        size_t operator()(const VertexDeclarationKey &key) const;
    };

    /**	Compares two vertex declaration keys. */
    class EqualFunc {
    public:
        bool operator()(const VertexDeclarationKey &a, const VertexDeclarationKey &b) const;
    };

    /**	Contains data about a single instance of DX11 input layout object. */
    struct InputLayoutEntry {
        InputLayoutEntry() {}

        ID3D11InputLayout *inputLayout;
        uint32_t lastUsedIdx;
    };

private:
    static const int DECLARATION_BUFFER_SIZE = 1024;
    static const int NUM_ELEMENTS_TO_PRUNE = 64;

    TMap<VertexDeclarationKey, InputLayoutEntry *, HashFunc, EqualFunc> mInputLayoutMap;

    bool mWarningShown = false;
    uint32_t mLastUsedCounter = 0;

public:
    DX11InputLayoutManager() = default;
    ~DX11InputLayoutManager();

public:
    /**
     * Finds an existing or creates a new D3D11 input layout. Input layout maps a vertex declaration
     * from a vertex buffer to vertex program input declaration
     *
     * @param[in]	vertexShaderDecl	Vertex declaration describing vertex program input parameters.
     * @param[in]	vertexBufferDecl	Vertex declaration describing structure of a vertex buffer to be bound as input
     *									to the GPU program.
     * @param[in]	vertexProgram		Instance of the vertex program we are creating input layout for.
     *
     * @note	Error will be thrown if the vertex buffer doesn't provide all the necessary data that the shader expects.
     */
    ID3D11InputLayout *retrieveInputLayout(VertexDeclaration *vertexShaderDecl, VertexDeclaration *vertexBufferDecl, DX11GpuProgram *vertexProgram);

private:
    /**	Creates a new input layout using the specified parameters and stores it in the input layout map. */
    void addNewInputLayout(VertexDeclaration *vertexShaderDecl, VertexDeclaration *vertexBufferDecl, DX11GpuProgram *vertexProgram);

    /**	Destroys least used input layout. */
    void removeLeastUsed();
};