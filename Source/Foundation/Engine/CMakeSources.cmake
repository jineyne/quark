set (ENGINE_AI_INC
        "Engine/AI/Decorator/AIBlackboardDecoratorNode.h"
        "Engine/AI/Decorator/AICooldownDecoratorNode.h"

        "Engine/AI/Execute/AIMoveRandomPositionExecuteNode.h"

        "Engine/AI/AIBehaviourTree.h"
        "Engine/AI/AIBlackboard.h"
        "Engine/AI/AICompositeNode.h"
        "Engine/AI/AIDecoratorNode.h"
        "Engine/AI/AIExecuteNode.h"
        "Engine/AI/AINode.h"
        "Engine/AI/AISelectorNode.h"
        "Engine/AI/AISequenceNode.h"
        "Engine/AI/AITypes.h")
set (ENGINE_AI_SRC
        "Engine/AI/Decorator/AIBlackboardDecoratorNode.cpp"
        "Engine/AI/Decorator/AICooldownDecoratorNode.cpp"

        "Engine/AI/Execute/AIMoveRandomPositionExecuteNode.cpp"

        "Engine/AI/AIBehaviourTree.cpp"
        "Engine/AI/AIBlackboard.cpp"
        "Engine/AI/AICompositeNode.cpp"
        "Engine/AI/AIDecoratorNode.cpp"
        "Engine/AI/AIExecuteNode.cpp"
        "Engine/AI/AINode.cpp"
        "Engine/AI/AISelectorNode.cpp"
        "Engine/AI/AISequenceNode.cpp")

set (ENGINE_COMPONENT_INC
        "Engine/Component/BehaviourTreeComponent.h")
set (ENGINE_COMPONENT_SRC
        "Engine/Component/BehaviourTreeComponent.cpp")

set (ENGINE_RESOURCE_INC
        "Engine/Resource/BuiltinResources.h")
set (ENGINE_RESOURCE_SRC
        "Engine/Resource/BuiltinResources.cpp")

set (ENGINE_TEXT_INC
        "Engine/Text/Font.h"
        "Engine/Text/Glyph.h")
set (ENGINE_TEXT_SRC
        "Engine/Text/Font.cpp"
        "Engine/Text/Glyph.cpp")

set (ENGINE_NOFILTER_INC
        "Engine/Application.h"
        "Engine/Prerequisites.h")
set (ENGINE_NOFILTER_SRC
        "Engine/Application.cpp")

set (ENGINE_SRCS
        ${ENGINE_AI_INC} ${ENGINE_AI_SRC}
        ${ENGINE_COMPONENT_INC} ${ENGINE_COMPONENT_SRC}
        ${ENGINE_RESOURCE_INC} ${ENGINE_RESOURCE_SRC}
        ${ENGINE_TEXT_INC} ${ENGINE_TEXT_SRC}
        ${ENGINE_NOFILTER_INC} ${ENGINE_NOFILTER_SRC})