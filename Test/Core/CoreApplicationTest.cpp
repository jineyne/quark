#include "CoreApplication.h"
#include "TestUtility.h"

TEST(QCoreApplicationTest, sycle) {
    FApplicationStartUpDesc desc{};
    desc.renderAPI = TEXT("quark-dx11");

    EXPECT_NO_THROW(QCoreApplication::StartUp(desc));
    EXPECT_NO_THROW(QCoreApplication::ShutDown());
}