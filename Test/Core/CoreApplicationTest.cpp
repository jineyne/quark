#include "CoreApplication.h"
#include "TestUtility.h"

#include "Math/Vector3.h"
#include "RenderAPI/VertexBuffer.h"
#include "RenderAPI/IndexBuffer.h"

struct FVertex {
    FVector3 position;
};

TEST(QCoreApplicationTest, sycle) {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");

    EXPECT_NO_THROW(QCoreApplication::StartUp(desc));

    QCoreApplication::Instance().runMainLoop();

    FVertexBufferDesc vd {};
    vd.usage = EBufferUsage::Static;
    vd.vertexSize = sizeof(FVertex);
    vd.vertexCount = 3;
    auto vertex = FVertexBuffer::New(vd);

    FIndexBufferDesc id {};
    id.usage = EBufferUsage::Static;
    id.indexType = EIndexType::_32bit;
    id.indexCount = 3;
    auto index = FIndexBuffer::New(id);

    delete vertex;
    delete index;

    EXPECT_NO_THROW(QCoreApplication::ShutDown());
}