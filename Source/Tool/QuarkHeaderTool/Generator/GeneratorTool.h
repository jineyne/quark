#pragma once

#include "QHTPrerequisites.h"

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CompilationDatabase.h>

#include "ClangGenerator.h"

class FGeneratorTool {
public:
    using FAstArray = std::vector<std::unique_ptr<clang::ASTUnit>>;

private:
    clang::tooling::FixedCompilationDatabase *mCompilationDatabase = nullptr;
    clang::tooling::ClangTool *mClangTool = nullptr;

    FString mSource;
    FClangGenerator::Configuration mConfiguration;

public:
    FGeneratorTool(FString source, TArray<std::string> flags);
    ~FGeneratorTool();

public:
    FGeneratorTool::FAstArray buildAsts();
};