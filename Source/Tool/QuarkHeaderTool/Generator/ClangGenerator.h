#pragma once

#include "QHTPrerequisites.h"

#include <clang/AST/DeclCXX.h>
#include <clang/AST/ASTImporter.h>

#include <FileSystem/Path.h>
#include <Reflection/Reflection.h>

#include "Scope.h"
#include "Parser/Symbol.h"
#include "Formatter.h"

class ClangGenerator {
public:
    struct Configuration {
        String annotationRequired;

        Path path;
        Path relativePath;
        String package;

        TSharedPtr<Stream> source;
        TSharedPtr<Stream> header;
    };

private:
    Scope *mTopScope = nullptr;
    TArray<Symbol *> mSymbols;
    Symbol *mCurrentSymbol = nullptr;

    clang::ASTContext* mContext;
    Configuration mConfig;

    Formatter mHeaderFormatter;
    Formatter mSourceFormatter;

    String mCurrentFileId = "";

public:
    ClangGenerator(const Configuration& config, TArray<Symbol *> symbols);

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
    void pushScope(const String &name, EScopeType type);
    void popScope();

    void generateStruct(const clang::CXXRecordDecl *record);
    void generateClass(const clang::CXXRecordDecl *record);
    void generateEnum(const clang::EnumDecl *record);

    void generateStatics(const clang::CXXRecordDecl *record, EScopeType scope);

    void generateField(clang::FieldDecl *field, Symbol *symbol);

    Reflection::EPropertyGenFlags getDataType(const clang::QualType *type, const clang::ASTContext &context);

    void generateTemplateArgsType(clang::CXXRecordDecl *record, size_t limit);
};