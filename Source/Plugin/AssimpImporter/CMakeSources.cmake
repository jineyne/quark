set (PLUGIN_IMPORTER_INC
        "Importer/AssimpImporter.h")
set (PLUGIN_IMPORTER_SRC
        "Importer/AssimpImporter.cpp")

set (PLUGIN_NOFILTER_INC
        "AIPrerequisites.h")
set (PLUGIN_NOFILTER_SRC
        "Plugin.cpp")

set (PLUGIN_SRCS
        ${PLUGIN_IMPORTER_INC} ${PLUGIN_IMPORTER_SRC}
        ${PLUGIN_NOFILTER_INC} ${PLUGIN_NOFILTER_SRC})
