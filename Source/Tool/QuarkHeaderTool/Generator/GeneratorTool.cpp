#include "GeneratorTool.h"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>

GeneratorTool::GeneratorTool(String source, TArray<std::string> flags) {
#if DEBUG_MODE
    flags.add("-D_DEBUG");
#endif

    flags.add("-xc++");
    flags.add("-std=c++17");
    flags.add("-ID:\\Projects\\Quark\\Source\\Foundation\\Utility");
    flags.add("-ID:\\Projects\\Quark\\Source\\Foundation\\Core");
    flags.add("-ID:\\Projects\\Quark\\Source\\Foundation\\Engine");
    flags.add("-ID:\\Projects\\Quark\\Source\\Foundation\\Generated");
    flags.add("-D_QHT_GENERATED");
    flags.add("-DUNICODE");
    flags.add("-fsyntax-only");
    flags.add("-Wno-everything");

    mCompilationDatabase = new clang::tooling::FixedCompilationDatabase(".", std::vector(flags.begin(), flags.end()));
    mClangTool = new clang::tooling::ClangTool(*mCompilationDatabase, std::string(TCHAR_TO_ANSI(*source)));
}

GeneratorTool::~GeneratorTool() {
    delete mCompilationDatabase;
    delete mClangTool;
}

GeneratorTool::AstArray GeneratorTool::buildAsts() {
    AstArray array;
    if (mClangTool->buildASTs(array) != 0) {
        return {};
    }

    return array;
}
