#pragma once

#include "QHTPrerequisites.h"

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>

#include "ClangGenerator.h"

class GeneratorTool {
public:
    using AstArray = std::vector<std::unique_ptr<clang::ASTUnit>>;

private:
    clang::tooling::FixedCompilationDatabase *mCompilationDatabase = nullptr;
    clang::tooling::ClangTool *mClangTool = nullptr;

    std::string mSource;
    ClangGenerator::Configuration mConfiguration;

public:
    GeneratorTool(std::string source, std::vector<std::string> flags);
    ~GeneratorTool();

public:
    GeneratorTool::AstArray buildAsts();
};