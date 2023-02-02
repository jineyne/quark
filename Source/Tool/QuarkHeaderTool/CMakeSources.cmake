set (QHT_GENERATOR_INC
        "Generator/Formatter.h"
        "Generator/Generator.h")
set (QHT_GENERATOR_SRC
        "Generator/Formatter.cpp"
        "Generator/Generator.cpp")

set (QHT_PARSER_INC
        "Parser/Node.h"
        "Parser/NodeVisiter.h"
        "Parser/Parser.h"
        "Parser/Token.h"
        "Parser/Tokenzier.h")
set (QHT_PARSER_SRC
        "Parser/Node.cpp"
        "Parser/Parser.cpp"
        "Parser/Tokenzier.cpp")

set (QHT_NOFILTER_INC
        "QHTPrerequisites.h")
set (QHT_NOFILTER_SRC )

set (QHT_SRCS
        ${QHT_GENERATOR_INC} ${QHT_GENERATOR_SRC}
        ${QHT_PARSER_INC} ${QHT_PARSER_SRC}
        ${QHT_NOFILTER_INC} ${QHT_NOFILTER_SRC})