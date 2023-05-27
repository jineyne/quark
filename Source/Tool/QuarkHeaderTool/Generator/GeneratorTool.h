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

    String mSource;
    ClangGenerator::Configuration mConfiguration;

public:
    GeneratorTool(String source, TArray<std::string> flags);
    ~GeneratorTool();

public:
    GeneratorTool::AstArray buildAsts();
};