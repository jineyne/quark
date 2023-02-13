#pragma once

#include "QHTPrerequisites.h"

#include <clang/AST/DeclCXX.h>
#include <clang/AST/ASTImporter.h>

#include <FileSystem/Path.h>
#include <Reflection/Reflection.h>

#include "Scope.h"
#include "Parser/Symbol.h"
#include "Formatter.h"

class FClangGenerator {
public:
    struct Configuration {
        FString annotationRequired;

        FPath path;
        FPath relativePath;
        FString package;

        TSharedPtr<FStream> source;
        TSharedPtr<FStream> header;
    };

private:
    FScope *mTopScope = nullptr;
    TArray<FSymbol *> mSymbols;
    FSymbol *mCurrentSymbol = nullptr;

    clang::ASTContext* mContext;
    Configuration mConfig;

    FFormatter mHeaderFormatter;
    FFormatter mSourceFormatter;

    FString mCurrentFileId = "";

public:
    FClangGenerator(const Configuration& config, TArray<FSymbol *> symbols);

public:
    void generate(const clang::TranslationUnitDecl* tuDecl);

    void scrapeTranslationUnit(const clang::TranslationUnitDecl* tuDecl);
    void scrapeDeclContext(const clang::DeclContext* ctxDecl);
    void scrapeNamedDecl(const clang::NamedDecl* namedDecl);

    void scrapCXXRecordDecl(const clang::CXXRecordDecl *cxxRecordDecl);
    void scrapCXXFieldDecl(const clang::FieldDecl *fieldDecl);

    void setContext(clang::ASTContext *context);

private:
    void pushScope(const FString &name, EScopeType type);
    void popScope();

    void generateStruct(const clang::CXXRecordDecl *record);
    void generateClass(const clang::CXXRecordDecl *record);

    void generateStatics(const clang::CXXRecordDecl *record, EScopeType scope);

    void generateField(clang::FieldDecl *field, FSymbol *symbol);

    QReflection::EPropertyGenFlags getDataType(const clang::QualType *type, const clang::ASTContext &context);

    void generateTemplateArgsType(clang::CXXRecordDecl *record, size_t limit);
};