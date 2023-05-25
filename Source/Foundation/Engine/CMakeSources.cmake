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
        ${ENGINE_RESOURCE_INC} ${ENGINE_RESOURCE_SRC}
        ${ENGINE_TEXT_INC} ${ENGINE_TEXT_SRC}
        ${ENGINE_NOFILTER_INC} ${ENGINE_NOFILTER_SRC})