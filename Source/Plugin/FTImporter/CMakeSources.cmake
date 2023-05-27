set (FT_IMPORTER_INC
        "Importer/FreeTypeImporter.h")
set (FT_IMPORTER_SRC
        "Importer/FreeTypeImporter.cpp")

set (FT_TEXT_INC
        "Text/FreeTypeFont.h")
set (FT_TEXT_SRC
        "Text/FreeTypeFont.cpp")

set (FT_NOFILTER_INC
        "FTPrerequisites.h")
set (FT_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${FT_IMPORTER_INC} ${FT_IMPORTER_SRC}
        ${FT_TEXT_INC} ${FT_TEXT_SRC}
        ${FT_NOFILTER_INC} ${FT_NOFILTER_SRC})
