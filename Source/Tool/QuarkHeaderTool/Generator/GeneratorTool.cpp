#include "GeneratorTool.h"

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/Tooling.h>

GeneratorTool::GeneratorTool(std::string source, std::vector<std::string> flags) {
#if DEBUG_MODE
    flags.push_back("-D_DEBUG");
#endif

    flags.push_back("-xc++");
    flags.push_back("-std=c++17");
    flags.push_back("-ID:\\Projects\\Quark\\Source\\Foundation\\Utility");
    flags.push_back("-ID:\\Projects\\Quark\\Source\\Foundation\\Core");
    flags.push_back("-ID:\\Projects\\Quark\\Source\\Foundation\\Engine");
    flags.push_back("-ID:\\Projects\\Quark\\Source\\Foundation\\Generated");
    flags.push_back("-D_QHT_GENERATED");
    flags.push_back("-DUNICODE");
    flags.push_back("-fsyntax-only");
    flags.push_back("-Wno-everything");

    mCompilationDatabase = new clang::tooling::FixedCompilationDatabase(".", flags);
    mClangTool = new clang::tooling::ClangTool(*mCompilationDatabase, source);
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
