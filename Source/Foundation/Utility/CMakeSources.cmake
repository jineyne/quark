set (UTILITY_ALLOC_INC
        "Utility/Alloc/GroupAllocator.h")

set (UTILITY_ALLOC_SRC
        "Utility/Alloc/GroupAllocator.cpp")

set (UTILITY_CONTAINER_INC
        "Utility/Container/Array.h"
        "Utility/Container/ContainerMacros.h"
        "Utility/Container/Map.h"
        "Utility/Container/String.h")
set (UTILITY_CONTAINER_SRC
        "Utility/Container/String.cpp")

set (UTILITY_EVENT_INC
        "Utility/Event/Event.h"
        "Utility/Event/EventCombinations.h")
set (UTILITY_EVENT_SRC
        "")

set (UTILITY_EXCEPTION_INC
        "Utility/Exception/CrashHandler.h"
        "Utility/Exception/Exception.h"
        "Utility/Exception/IStackWalker.h")
set (UTILITY_EXCEPTION_SRC
        "Utility/Exception/CrashHandler.cpp"
        "Utility/Exception/Exception.cpp")

set(UTILITY_FILESYSTEM_INC
        "Utility/FileSystem/FileStream.h"
        "Utility/FileSystem/FileSystem.h"
        "Utility/FileSystem/FileSystemFwd.h"
        "Utility/FileSystem/MemoryStream.h"
        "Utility/FileSystem/Path.h"
        "Utility/FileSystem/Stream.h")
set(UTILITY_FILESYSTEM_SRC
        "Utility/FileSystem/FileStream.cpp"
        "Utility/FileSystem/FileSystem.cpp"
        "Utility/FileSystem/MemoryStream.cpp"
        "Utility/FileSystem/Path.cpp"
        "Utility/FileSystem/Stream.cpp")

set (UTILITY_IMAGE_INC
        "Utility/Image/Color.h")
set (UTILITY_IMAGE_SRC
        "Utility/Image/Color.cpp")

set (UTILITY_LOGGING_INC
        "Utility/Logging/LogCategory.h"
        "Utility/Logging/LogDefines.h"
        "Utility/Logging/Logger.h"
        "Utility/Logging/LogMacros.h")
set (UTILITY_LOGGING_SRC
        "Utility/Logging/LogCategory.cpp"
        "Utility/Logging/Logger.cpp"
        "Utility/Logging/LogMacros.cpp")

set (UTILITY_MATH_INC
        "Utility/Math/Degree.h"
        "Utility/Math/Matrix4.h"
        "Utility/Math/Radian.h"
        "Utility/Math/Vector2.h"
        "Utility/Math/Vector3.h")
set (UTILITY_MATH_SRC
        "Utility/Math/Degree.cpp"
        "Utility/Math/Matrix4.cpp"
        "Utility/Math/Radian.cpp"
        "Utility/Math/Vector2.cpp"
        "Utility/Math/Vector3.cpp")

set (UTILITY_MEMORY_INC
        "Utility/Memory/HeapAllocator.h"
        "Utility/Memory/InlineAllocator.h"
        "Utility/Memory/MemoryUtil.h")
set (UTILITY_MEMORY_SRC)

set (UTILITY_MISC_INC
        "Utility/Misc/AssertionMacros.h"
        "Utility/Misc/Char.h"
        "Utility/Misc/CString.h"
        "Utility/Misc/DataBlob.h"
        "Utility/Misc/EnumClassFlags.h"
        "Utility/Misc/LazySingleton.h"
        "Utility/Misc/Misc.h"
        "Utility/Misc/Module.h"
        "Utility/Misc/Platform.h"
        "Utility/Misc/SingletonMacros.h"
        "Utility/Misc/StringBuilder.h"
        "Utility/Misc/Time.h"
        "Utility/Misc/Timer.h"
        "Utility/Misc/UUID.h"
        "Utility/Misc/VarArgs.h")
set (UTILITY_MISC_SRC
        "Utility/Misc/AssertionMacros.cpp"
        "Utility/Misc/Module.cpp"
        "Utility/Misc/Platform.cpp"
        "Utility/Misc/Time.cpp"
        "Utility/Misc/Timer.cpp"
        "Utility/Misc/UUID.cpp")

set (UTILITY_PLUGIN_INC
        "Utility/Plugin/DynLib.h"
        "Utility/Plugin/DynLibManager.h"
        "Utility/Plugin/PluginFactory.h"
        "Utility/Plugin/PluginManager.h")
set (UTILITY_PLUGIN_SRC
        "Utility/Plugin/DynLib.cpp"
        "Utility/Plugin/DynLibManager.cpp"
        "Utility/Plugin/PluginManager.cpp")

set (UTILITY_PREREQUISITES_INC
        "Utility/Prerequisites/CommonTypes.h"
        "Utility/Prerequisites/PlatformDefines.h"
        "Utility/Prerequisites/PrerequisitesUtil.h"
        "Utility/Prerequisites/StdHeaders.h")
set (UTILITY_PREREQUISITES_SRC )

set(UTILITY_REFLECTION_INC
        "Utility/Reflection/Class.h"
        "Utility/Reflection/Enum.h"
        "Utility/Reflection/Field.h"
        "Utility/Reflection/GeneratedPrerequisites.h"
        "Utility/Reflection/MetaData.h"
        "Utility/Reflection/Object.h"
        "Utility/Reflection/ObjectHash.h"
        "Utility/Reflection/Property.h"
        "Utility/Reflection/Reflection.h"
        "Utility/Reflection/ReflectionMacros.h"
        "Utility/Reflection/Struct.h")
set(UTILITY_REFLECTION_SRC
        "Utility/Reflection/Class.cpp"
        "Utility/Reflection/Enum.cpp"
        "Utility/Reflection/Field.cpp"
        "Utility/Reflection/MetaData.cpp"
        "Utility/Reflection/Object.cpp"
        "Utility/Reflection/ObjectHash.cpp"
        "Utility/Reflection/Property.cpp"
        "Utility/Reflection/Reflection.cpp"
        "Utility/Reflection/Struct.cpp")

set (UTILITY_SERIALIZATION_INC
        "Utility/Serialization/Archive.h"
        "Utility/Serialization/BinaryArchive.h"
        "Utility/Serialization/TextArchive.h")
set (UTILITY_SERIALIZATION_SRC
        "Utility/Serialization/Archive.cpp"
        "Utility/Serialization/BinaryArchive.cpp"
        "Utility/Serialization/TextArchive.cpp")

set (UTILITY_STRING_INC
        "Utility/String/StringConverter.h")
set (UTILITY_STRING_SRC
        "Utility/String/StringConverter.cpp")

set (UTILITY_THIRDPARTY_INC
        "Utility/ThirdParty/argparser.h")
set (UTILITY_THIRDPARTY_SRC )

set (UTILITY_TRAITS_INC
        "Utility/Traits/CharTraits.h"
        "Utility/Traits/IntTraits.h")
set (UTILITY_TRAITS_SRC)

set (UTILITY_NOFILTER_INC
        "Utility/UtilityCore.h")
set (UTILITY_NOFILTER_SRC
        "")

set (UTILITY_THIRDPARTY_INC
        "Utility/Mustache/Mustache.h")
set (UTILITY_THIRDPARTY_SRC)

set (UTILITY_WIN32_INC
        "Utility/Win32/IWindowMessageHandler.h"
        "Utility/Win32/StackWalker.h"
        "Utility/Win32/Win32.h"
        "Utility/Win32/Win32Platform.h"
        "Utility/Win32/Win32StackWalker.h"
        "Utility/Win32/Win32Window.h")
set (UTILITY_WIN32_SRC
        "Utility/Win32/StackWalker.cpp"
        "Utility/Win32/Win32FileSystem.cpp"
        "Utility/Win32/Win32Platform.cpp"
        "Utility/Win32/Win32StackWalker.cpp"
        "Utility/Win32/Win32Window.cpp")

set (UTILITY_SRCS
        ${UTILITY_ALLOC_INC} ${UTILITY_ALLOC_SRC}
        ${UTILITY_CONTAINER_INC} ${UTILITY_CONTAINER_SRC}
        ${UTILITY_EVENT_INC} ${UTILITY_EVENT_SRC}
        ${UTILITY_DYNLIB_INC} ${UTILITY_DYNLIB_SRC}
        ${UTILITY_EXCEPTION_INC} ${UTILITY_EXCEPTION_SRC}
        ${UTILITY_FILESYSTEM_INC} ${UTILITY_FILESYSTEM_SRC}
        ${UTILITY_IMAGE_INC} ${UTILITY_IMAGE_SRC}
        ${UTILITY_LOGGING_INC} ${UTILITY_LOGGING_SRC}
        ${UTILITY_MATH_INC} ${UTILITY_MATH_SRC}
        ${UTILITY_MEMORY_INC} ${UTILITY_MEMORY_SRC}
        ${UTILITY_MISC_INC} ${UTILITY_MISC_SRC}
        ${UTILITY_PLUGIN_INC} ${UTILITY_PLUGIN_SRC}
        ${UTILITY_PREREQUISITES_INC} ${UTILITY_PREREQUISITES_SRC}
        ${UTILITY_REFLECTION_INC} ${UTILITY_REFLECTION_SRC}
        ${UTILITY_SERIALIZATION_INC} ${UTILITY_SERIALIZATION_SRC}
        ${UTILITY_STRING_INC} ${UTILITY_STRING_SRC}
        ${UTILITY_THIRDPARTY_INC} ${UTILITY_THIRDPARTY_SRC}
        ${UTILITY_TRAITS_INC} ${UTILITY_TRAITS_SRC}
        ${UTILITY_NOFILTER_INC} ${UTILITY_NOFILTER_SRC}
        ${UTILITY_THIRDPARTY_INC} ${UTILITY_THIRDPARTY_SRC})

if (WIN32)
    list (APPEND UTILITY_SRCS ${UTILITY_WIN32_INC} ${UTILITY_WIN32_SRC})
endif ()