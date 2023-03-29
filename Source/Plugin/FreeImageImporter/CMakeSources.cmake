set (PLUGIN_IMPORTER_INC
        "Importer/FreeImgImporter.h")
set (PLUGIN_IMPORTER_SRC
        "Importer/FreeImgImporter.cpp")

set (PLUGIN_NOFILTER_INC
        "FIPrerequisites.h")
set (PLUGIN_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${PLUGIN_IMPORTER_INC} ${PLUGIN_IMPORTER_SRC}
        ${PLUGIN_NOFILTER_INC} ${PLUGIN_NOFILTER_SRC})