set (CONTAINER_TESTS
        "Utility/Container/ArrayTest.cpp"
        "Utility/Container/MapTest.cpp"
        "Utility/Container/StringTest.cpp")

set (FILESYSTEM_TESTS
        "Utility/FileSystem/FileSystemTest.cpp"
        "Utility/FileSystem/StreamTest.cpp")

set (LOGGING_TESTS
        "Utility/Logging/LoggerTest.cpp")

set (MISC_TESTS
        "Utility/Misc/CStringTest.cpp"
        "Utility/Misc/StringBuilderTest.cpp")

set (REFLECTION_TESTS
        "Utility/Reflection/EnumTest.cpp"
        "Utility/Reflection/ReflectionTest.cpp"
        "Utility/Reflection/ClassTest.cpp")

set (SERIALIZATION_TESTS
        "Utility/Serialization/ArchiveTest.cpp"
        "Utility/Serialization/ArchiveTest.h"
        "Utility/Serialization/BaseClass.h"
        "Utility/Serialization/SaveData.h"
        "Utility/Serialization/OtherActor.cpp"
        "Utility/Serialization/OtherActor.h"

        "Utility/Serialization/ArchiveTest.g.cpp"
        "Utility/Serialization/ArchiveTest.g.h"
        "Utility/Serialization/BaseClass.g.cpp"
        "Utility/Serialization/BaseClass.g.h"
        "Utility/Serialization/SaveData.g.cpp"
        "Utility/Serialization/SaveData.g.h"
        "Utility/Serialization/OtherActor.g.cpp"
        "Utility/Serialization/OtherActor.g.h"
        )

set (STRING_TESTS
        "Utility/String/StringConverterTest.cpp")

set (UTILITY_TESTS 
        ${CONTAINER_TESTS}
        ${FILESYSTEM_TESTS}
        ${LOGGING_TESTS}
        ${MISC_TESTS}
        ${REFLECTION_TESTS}
        ${SERIALIZATION_TESTS}
        ${STRING_TESTS})
