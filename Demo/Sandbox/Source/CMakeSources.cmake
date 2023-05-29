set (SANDBOX_COMPONENT_INC
        "Source/Component/BulletComponent.h"
        "Source/Component/FighterAIComponent.h"
        "Source/Component/PlayerInputComponent.h"
        "Source/Component/ShipComponent.h")
set (SANDBOX_COMPONENT_SRC
        "Source/Component/BulletComponent.cpp"
        "Source/Component/FighterAIComponent.cpp"
        "Source/Component/PlayerInputComponent.cpp"
        "Source/Component/ShipComponent.cpp")

set (SANDBOX_UTIL_INC
        "Source/Utility/SandboxResourceUtil.h")
set (SANDBOX_UTIL_SRC
        "Source/Utility/SandboxResourceUtil.cpp")

set (SANDBOX_NOFILTER_INC
        )
set (SANDBOX_NOFILTER_SRC
        "Source/App.cpp")

set (APP_SRCS
        ${SANDBOX_COMPONENT_INC} ${SANDBOX_COMPONENT_SRC}
        ${SANDBOX_UTIL_INC} ${SANDBOX_UTIL_SRC}
        ${SANDBOX_NOFILTER_INC} ${SANDBOX_NOFILTER_SRC})