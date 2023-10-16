set (COMPONENT_INC
        "Source/Component/SpriteAnimationComponent.h")
set (COMPONENT_SRC
        "Source/Component/SpriteAnimationComponent.cpp")

set (NOFILTER_INC
        "Source/RPGPrerequisites.h")
set (NOFILTER_SRC
        "Source/main.cpp")

set (APP_SRCS
        ${COMPONENT_INC} ${COMPONENT_SRC}
        ${NOFILTER_INC} ${NOFILTER_SRC})