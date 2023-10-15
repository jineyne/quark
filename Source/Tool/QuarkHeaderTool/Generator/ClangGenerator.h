#pragma once

#include "QHTPrerequisites.h"

#include <clang/AST/DeclCXX.h>
#include <clang/AST/ASTImporter.h>

#include "Scope.h"
#include "Parser/Symbol.h"
#include "Formatter.h"

class ClangGenerator {
public:
    struct Configuration {
        std::string annotationRequired;

        std::filesystem::path path;
        std::filesystem::path relativePath;
        std::string package;

        std::shared_ptr<std::ofstream> source;
        std::shared_ptr<std::ofstream> header;
    };

private:
    Scope *mTopScope = nullptr;
    std::vector<Symbol *> mSymbols;
    Symbol *mCurrentSymbol = nullptr;

    clang::ASTContext* mContext;
    Configuration mConfig;

    Formatter mHeaderFormatter;
    Formatter mSourceFormatter;

    std::string mCurrentFileId = "";

public:
    ClangGenerator(const Configuration& config, std::vector<Symbol *> symbols);

public:
    void generate(const clang::TranslationUnitDecl* tuDecl);

    void scrapeTranslationUnit(const clang::TranslationUnitDecl* tuDecl);
    void scrapeDeclContext(const clang::DeclContext* ctxDecl);
    void scrapeNamedDecl(const clang::NamedDecl* namedDecl);

    void scrapCXXRecordDecl(const clang::CXXRecordDecl *cxxRecordDecl);
    void scrapCXXFieldDecl(const clang::FieldDecl *fieldDecl);
    void scrapCXXEnumDecl(const clang::EnumDecl *enumDecl);

    void setContext(clang::ASTContext *context);

private:
    void pushScope(const std::string &name, EScopeType type);
    void popScope();

    void generateStruct(const clang::CXXRecordDecl *record);
    void generateClass(const clang::CXXRecordDecl *record);
    void generateEnum(const clang::EnumDecl *record);

    void generateStatics(const clang::CXXRecordDecl *record, EScopeType scope);

    void generateField(clang::FieldDecl *field, Symbol *symbol);

    int getDataType(const clang::QualType *type, const clang::ASTContext &context);

    void generateTemplateArgsType(clang::CXXRecordDecl *record, size_t limit);
};