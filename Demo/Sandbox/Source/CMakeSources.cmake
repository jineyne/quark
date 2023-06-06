set (SANDBOX_AI_INC
        "Source/AI/AIAttackTargetExecuteNode.h"
        "Source/AI/AILookTargetExecuteNode.h"
        "Source/AI/AIRunawayExecuteNode.h")
set (SANDBOX_AI_INC
        "Source/AI/AIAttackTargetExecuteNode.cpp"
        "Source/AI/AILookTargetExecuteNode.cpp"
        "Source/AI/AIRunawayExecuteNode.cpp")

set (SANDBOX_COMPONENT_INC
        "Source/Component/BattleFleetAIComponent.h"
        "Source/Component/BulletComponent.h"
        "Source/Component/FighterAIComponent.h"
        "Source/Component/FollowTargetComponent.h"
        "Source/Component/PlayerInputComponent.h"
        "Source/Component/PlayerShipComponent.h"
        "Source/Component/ShipAIComponent.h"
        "Source/Component/SpawnerComponent.h")
set (SANDBOX_COMPONENT_SRC
        "Source/Component/BattleFleetAIComponent.cpp"
        "Source/Component/BulletComponent.cpp"
        "Source/Component/FighterAIComponent.cpp"
        "Source/Component/FollowTargetComponent.cpp"
        "Source/Component/PlayerInputComponent.cpp"
        "Source/Component/PlayerShipComponent.cpp"
        "Source/Component/ShipAIComponent.cpp"
        "Source/Component/SpawnerComponent.cpp")

set (SANDBOX_UTIL_INC
        "Source/Utility/SandboxResourceUtil.h")
set (SANDBOX_UTIL_SRC
        "Source/Utility/SandboxResourceUtil.cpp")

set (SANDBOX_NOFILTER_INC
        )
set (SANDBOX_NOFILTER_SRC
        "Source/App.cpp")

set (APP_SRCS
        ${SANDBOX_AI_INC} ${SANDBOX_AI_SRC}
        ${SANDBOX_COMPONENT_INC} ${SANDBOX_COMPONENT_SRC}
        ${SANDBOX_UTIL_INC} ${SANDBOX_UTIL_SRC}
        ${SANDBOX_NOFILTER_INC} ${SANDBOX_NOFILTER_SRC})