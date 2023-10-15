set (QHT_GENERATOR_INC
        "Generator/ClangGenerator.h"
        "Generator/Formatter.h"
        "Generator/GeneratorTool.h"
        "Generator/Scope.h")
set (QHT_GENERATOR_SRC
        "Generator/ClangGenerator.cpp"
        "Generator/Formatter.cpp"
        "Generator/GeneratorTool.cpp")

set (QHT_PARSER_INC
        "Parser/Symbol.h"
        "Parser/SymbolParser.h"
        "Parser/Token.h"
        "Parser/Tokenzier.h")
set (QHT_PARSER_SRC
        "Parser/Symbol.cpp"
        "Parser/SymbolParser.cpp"
        "Parser/Tokenzier.cpp")

set (QHT_NOFILTER_INC
        "argparser.h"
        "QHTPrerequisites.h")
set (QHT_NOFILTER_SRC )

set (QHT_SRCS
#        ${QHT_GENERATOR_INC} ${QHT_GENERATOR_SRC}
#        ${QHT_PARSER_INC} ${QHT_PARSER_SRC}
        ${QHT_NOFILTER_INC} ${QHT_NOFILTER_SRC})