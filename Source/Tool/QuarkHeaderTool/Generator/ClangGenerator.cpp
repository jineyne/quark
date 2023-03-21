#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/ASTContext.h>

#include "ClangGenerator.h"
#include "FileSystem/FileSystem.h"
#include "Misc/StringBuilder.h"

#define APPEND_DEFAULT_ARGS(TARGET)                                                                                 \
    (TARGET).add(TEXT("package"), mConfig.package)                                                                  \
            .add(TEXT("path"), mConfig.path.toString())                                                             \
            .add(TEXT("relativePath"), mConfig.relativePath.toString())                                             \
            .add(TEXT("filename"), mConfig.path.getFilename())                                                      \
            .add(TEXT("currentFileId"), mCurrentFileId)

FClangGenerator::FClangGenerator(const Configuration& config, TArray<FSymbol *> symbols)
        : mConfig(config), mSymbols(symbols), mSourceFormatter(config.source), mHeaderFormatter(config.header) { }

void FClangGenerator::generate(const clang::TranslationUnitDecl* tuDecl) {
    FStringBuilder sb(512);
    if (!mConfig.package.empty()) {
        sb << mConfig.package << TEXT("_");
    }

    sb << TEXT("Source_");
    sb << mConfig.path.getFilename() << TEXT("_h");

    mCurrentFileId = sb.toString();

    FNamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    mHeaderFormatter.append(R"(
// DO NOT MODIFY MANUALLY!

#ifdef {{package}}_{{filename}}_g_h
#error "{{filename}}.g.h already included. missing '#pragma once' in {{filename}}.h"
#endif

#define {{package}}_{{filename}}_g_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID {{currentFileId}}

#include "Reflection/GeneratedPrerequisites.h"

)", args);

    mSourceFormatter.append(TEXT(R"(
#include "{{relativePath}}"
#include "Reflection/GeneratedPrerequisites.h"

)"), args);

    scrapeTranslationUnit(tuDecl);
}

void FClangGenerator::scrapeTranslationUnit(const clang::TranslationUnitDecl* tuDecl) {
    scrapeDeclContext(tuDecl);
}

void FClangGenerator::scrapeDeclContext(const clang::DeclContext* ctxDecl) {
    for (clang::DeclContext::decl_iterator it = ctxDecl->decls_begin(); it != ctxDecl->decls_end(); ++it) {
        const auto &decl = *it;
        if (decl->isInvalidDecl()) {
            continue;
        }

        auto named = clang::dyn_cast<clang::NamedDecl>(decl);
        if (named) {
            scrapeNamedDecl(named);
        }
    }
}

void FClangGenerator::scrapeNamedDecl(const clang::NamedDecl* namedDecl) {
    auto kind = namedDecl->getKind();
    switch (kind) {
        case clang::Decl::Namespace:
            // TODO: support?
            break;

        case clang::Decl::CXXRecord:
            scrapCXXRecordDecl(clang::dyn_cast<clang::CXXRecordDecl>(namedDecl));
            break;

        case clang::Decl::Field:
            scrapCXXFieldDecl(clang::dyn_cast<clang::FieldDecl>(namedDecl));
            break;

        default:
            break;
    }
}

void FClangGenerator::scrapCXXRecordDecl(const clang::CXXRecordDecl *cxxRecordDecl) {
    if (cxxRecordDecl->isAnonymousStructOrUnion()) {
        scrapeDeclContext(cxxRecordDecl);
        return;
    }

    FString name = ANSI_TO_TCHAR(cxxRecordDecl->getQualifiedNameAsString().c_str());

    auto symbol = mSymbols.findIf([name](FSymbol *symbol) { return symbol->name == name; });
    if (symbol == nullptr) {
        return;
    }

    if ((*symbol)->marked) {
        return;
    }

    mCurrentSymbol = *symbol;

    const auto *type = cxxRecordDecl->getTypeForDecl();

    if (type->isStructureType()) {
        generateStruct(cxxRecordDecl);
    } else {
        generateClass(cxxRecordDecl);
    }
}

void FClangGenerator::scrapCXXFieldDecl(const clang::FieldDecl *fieldDecl) {
    FString name = ANSI_TO_TCHAR(fieldDecl->getNameAsString().c_str());

    auto symbol = mCurrentSymbol->children.findIf([name](FSymbol *symbol) { return symbol->name == name; });
    if (symbol == nullptr) {
        return;
    }

    LOG(LogQHT, Debug, TEXT("visit field: %ls"), *name);
}

void FClangGenerator::setContext(clang::ASTContext *context) {
    mContext = context;
}

void FClangGenerator::pushScope(const FString &name, EScopeType type) {
    auto scope = q_new<FScope>();
    scope->currentName = name;
    scope->type = type;
    if (mTopScope == nullptr) {
        mTopScope = scope;
        mTopScope->parent = nullptr;
        mTopScope->fullName = name;
    } else {
        scope->parent = mTopScope;
        scope->fullName = mTopScope->fullName + TEXT("::") + name;
        scope->parent = mTopScope;
        mTopScope = scope;
    }

    LOG(LogQHT, Debug, TEXT("Scope pushed: %ls"), *mTopScope->fullName);
}

void FClangGenerator::popScope() {
    auto temp = mTopScope;
    mTopScope = temp->parent;

    LOG(LogQHT, Debug, TEXT("Scope popped: %ls"), *temp->fullName);
    q_delete(temp);
}

void FClangGenerator::generateStruct(const clang::CXXRecordDecl *record) {
    FString name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());
    pushScope(name, EScopeType::Struct);

    auto generated = mCurrentSymbol->extras.find(GENERATED);
    if (generated == nullptr) {
        // error;
        return;
    }

    FNamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add(TEXT("name"), name);
    args.add(TEXT("lineNo"), *generated);

    mHeaderFormatter.append(TEXT(R"(
#define {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS                        \
        friend struct Generated_Struct_{{name}}_Statics;                        \
        static QStruct *StaticStruct();

#define {{currentFileId}}_{{lineNo}}_GENERATED_BODY                             \
        {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS
)"), args);

    mSourceFormatter.append(TEXT(R"(
QStruct *Generated_Initializer_Struct_{{name}}();
QStruct *{{name}}::StaticStruct() {
    static QStruct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_{{name}}();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_{{name}}(&Generated_Initializer_Struct_{{name}}, &{{name}}::StaticStruct, TEXT("{{name}}"), TEXT("{{relativePath}}"));


)"), args);

    generateStatics(record, EScopeType::Struct);

    mSourceFormatter.append(TEXT(R"(
QStruct *Generated_Initializer_Struct_{{name}}() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_{{name}}_Statics::StructDesc);
    }
    return instance;
}
)"), args);

    popScope();
}

void FClangGenerator::generateClass(const clang::CXXRecordDecl *record) {
    FString name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());
    pushScope(name, EScopeType::Class);

    auto generated = mCurrentSymbol->extras.find(GENERATED);
    if (generated == nullptr) {
        // error;
        return;
    }

    mCurrentSymbol->marked = true;
    bool bIsAbstract = mCurrentSymbol->metas.find(TEXT("abstract")) != nullptr;

    FString base = FString::Empty;
    if (!record->bases().empty()) {
        for (auto it = record->bases_begin(); it != record->bases_end(); ++it) {
            if (!base.empty()) {
                // error
                LOG(LogQHT, Error, TEXT("Multiple base is not support!"));
                break;
            }

            auto baseRecord = (*it).getType()->getAsCXXRecordDecl();
            base = ANSI_TO_TCHAR(baseRecord->getQualifiedNameAsString().c_str());
        }
    }

    FNamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add(TEXT("name"), name);
    args.add(TEXT("lineNo"), *generated);
    args.add(TEXT("base"), base);

    mHeaderFormatter.append(TEXT(R"(
#define {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS \
private: \
    friend struct Generated_Class_{{name}}_Statics; \
    static void StaticRegisterNative{{name}}(); \
public: \
    DECLARE_CLASS({{name}}, {{base}}, ) \
)"), args);
    if (!bIsAbstract) {
        mHeaderFormatter.appendLine(TEXT(R"(    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL({{name}}) \)"), args);
    }
    mHeaderFormatter.append(TEXT(R"(    DECLARE_SERIALIZER({{name}})

#define {{currentFileId}}_{{lineNo}}_GENERATED_BODY \
        {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS
)"), args);

    mSourceFormatter.appendLine(TEXT("void {{name}}::StaticRegisterNative{{name}}() {"), args);
    mSourceFormatter.addIndent();

    // function declares

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(TEXT("}"));

    if (bIsAbstract) {
        mSourceFormatter.append(TEXT(R"(IMPLEMENT_CLASS_NO_CTR({{name}});)"), args);
    } else {
        mSourceFormatter.append(TEXT(R"(IMPLEMENT_CLASS({{name}});)"), args);
    }

    mSourceFormatter.append(TEXT(R"(
QClass *Generated_Initializer_Class_{{name}}();
static FInitClassOnStart Generated_InitClassOnStart_Class_{{name}}(&Generated_Initializer_Class_{{name}}, &{{name}}::StaticClass, TEXT("{{name}}"), TEXT("{{relativePath}}"));

)"), args);

    generateStatics(record, EScopeType::Class);

    mSourceFormatter.append(TEXT(R"(
QClass *Generated_Initializer_Class_{{name}}() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_{{name}}_Statics::ClassDesc);
    }
    return instance;
}
)"), args);

    popScope();
}

void FClangGenerator::generateStatics(const clang::CXXRecordDecl *record, EScopeType scope) {
    FNamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    FString name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());

    args.add(TEXT("name"), name);
    args.add(TEXT("type"), scope == EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

    mSourceFormatter.appendLine(TEXT("struct Generated_{{type}}_{{name}}_Statics {"), args);
    mSourceFormatter.addIndent();

    auto children = mCurrentSymbol->children;
    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        FString fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](FSymbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        auto symbol = *found;
        auto fieldType = (*it)->getType();

        auto typeName = fieldType->getTypeClassName();

        QReflection::EPropertyGenFlags type = getDataType(&fieldType, record->getASTContext());
        args.add(TEXT("genFlags"), (int) type);

        FNamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), fieldName);

        if (fieldType->isBuiltinType()) {
            mSourceFormatter.appendLine(TEXT("static const QReflection::FGenericPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
        } else {
            switch (type) {
                case QReflection::EPropertyGenFlags::Object:
                    mSourceFormatter.appendLine(TEXT("static const QReflection::FObjectPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                    break;

                case QReflection::EPropertyGenFlags::Struct:
                    mSourceFormatter.appendLine(TEXT("static const QReflection::FStructPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                    break;

                case QReflection::EPropertyGenFlags::Array:
                    mSourceFormatter.appendLine(TEXT("static const QReflection::FArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                    break;

                case QReflection::EPropertyGenFlags::Map:
                    mSourceFormatter.appendLine(TEXT("static const QReflection::FArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                    break;

                case QReflection::EPropertyGenFlags::Set:
                    mSourceFormatter.appendLine(TEXT("static const QReflection::FArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                    break;

                default:
                    mSourceFormatter.appendLine(TEXT("static const QReflection::FGenericPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                    break;
            }
        }

        mSourceFormatter.appendLine(TEXT("static const TArray<QReflection::FMetaDataPairDesc> {{name}}_MetaData;"), fieldArgs, true);
    }

    mSourceFormatter.append(TEXT(R"(
    static const TArray<QReflection::FMetaDataPairDesc> {{type}}MetaData;
    static const TArray<QReflection::FPropertyDescBase const*> {{type}}Properties;
    static const QReflection::F{{type}}Desc {{type}}Desc;
};
)"), args);

    mSourceFormatter.removeIndent();
    pushScope(name, scope);

    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        FString fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](FSymbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        generateField((*it), *found);
    }

    popScope();

    mSourceFormatter.append(TEXT(R"(
const TArray<QReflection::FMetaDataPairDesc> Generated_{{type}}_{{name}}_Statics::{{type}}MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    uint64_t flags = 0;
    for (auto entry : mCurrentSymbol->metas) {
        FNamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.first).add(TEXT("mValue"), entry.second);

        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(R"(};)");

    mSourceFormatter.appendLine(TEXT("const TArray<QReflection::FPropertyDescBase const*> Generated_{{type}}_{{name}}_Statics::{{type}}Properties = {"), args, true);
    mSourceFormatter.addIndent();

    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        FString fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](FSymbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        auto symbol = *found;
        auto fieldType = (*it)->getType();

        FNamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), fieldName)
                .add(TEXT("typeName"), name)
                .add(TEXT("type"), scope == EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

        mSourceFormatter.appendLine(TEXT("(const QReflection::FPropertyDescBase *) &Generated_{{type}}_{{typeName}}_Statics::{{name}}_PropertyDesc,"), fieldArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine("};");

    args.add(TEXT("flags"), 0);

    mSourceFormatter.append(TEXT(R"(
const QReflection::F{{type}}Desc Generated_{{type}}_{{name}}_Statics::{{type}}Desc = {
    TEXT("{{name}}"),
    {{name}}::Static{{type}},
    (E{{type}}Flags) {{flags}},
)"), args);

    if (scope == EScopeType::Struct) {
        mSourceFormatter.append(TEXT(R"(    sizeof({{name}}),)"), args);
    }

    mSourceFormatter.append(TEXT(R"(
    Generated_{{type}}_{{name}}_Statics::{{type}}Properties,
    Generated_{{type}}_{{name}}_Statics::{{type}}MetaData,
};
)"), args);
}

void FClangGenerator::generateField(clang::FieldDecl *field, FSymbol *symbol) {
    FNamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    FString name = ANSI_TO_TCHAR(field->getNameAsString().c_str());
    args.add(TEXT("name"), name);

    auto fieldType = field->getType();
    FString fieldTypeName = ANSI_TO_TCHAR(fieldType.getAsString().c_str());

    args.add(TEXT("scopeName"), mTopScope->currentName);
    args.add(TEXT("fieldTypeName"), fieldTypeName);

    bool bIsClass = mTopScope->type == EScopeType::Class;
    FString keywordName = bIsClass ? TEXT("Class") : TEXT("Struct");
    args.add(TEXT("keywordName"), keywordName);

    uint64_t flags = 0;
    TMap<FString, FString> metas;

    QReflection::EPropertyGenFlags type = getDataType(&fieldType, field->getASTContext());
    args.add(TEXT("genFlags"), (int) type);

    switch (field->getAccess()) {
        case clang::AS_public:
            flags |= PropertyFlags_Public;
            break;

        case clang::AS_protected:
            flags |= PropertyFlags_Protected;
            break;

        case clang::AS_private:
            flags |= PropertyFlags_Private;
        default:
            break;
    }


    for (auto entry : symbol->metas) {
        if (!entry.second.empty()) {
            metas.add(entry.first, entry.second);
            continue;
        }

        if (entry.first.contains(TEXT("."))) {
            metas.add(entry.first, entry.second);
            continue;
        }

        if (entry.first == "NonSerialized") {
            flags |= PropertyFlags_NonSerialized;
            continue;
        }

        metas.add(entry.first, entry.second);
    }

    args.add(TEXT("flags"), flags);

    if (fieldType->isBuiltinType()) {
        mSourceFormatter.append(TEXT(R"(
const QReflection::FGenericPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
    } else {
        switch (type) {
            case QReflection::EPropertyGenFlags::Object:
                mSourceFormatter.append(TEXT(R"(
const QReflection::FObjectPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
                break;

            case QReflection::EPropertyGenFlags::Struct:
                mSourceFormatter.append(TEXT(R"(
const QReflection::FStructPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    {{name}}::StaticStruct,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
                break;

            case QReflection::EPropertyGenFlags::Class:
                mSourceFormatter.append(TEXT(R"(
const QReflection::FStructPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    {{scopeName}}::StaticClass,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);

            case QReflection::EPropertyGenFlags::Array:
            case QReflection::EPropertyGenFlags::Map:
            case QReflection::EPropertyGenFlags::Set:
                mSourceFormatter.append(TEXT(R"(
const QReflection::FArrayPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
)"), args);
                generateTemplateArgsType(field->getType()->getAsCXXRecordDecl(), 1);
                mSourceFormatter.append(TEXT(R"(
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
                break;

            default:
                mSourceFormatter.append(TEXT(R"(
const QReflection::FGenericPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
                break;
        }
    }

    mSourceFormatter.append(TEXT(R"(
const TArray<QReflection::FMetaDataPairDesc> Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    for (auto entry : symbol->metas) {
        FNamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.first).add(TEXT("mValue"), entry.second);

        /*if (!entry.second.empty()) {
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
            continue;
        }

        if (entry.first.contains(TEXT("."))) {
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
            continue;
        }*/

        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(TEXT("};"), args);
}

QReflection::EPropertyGenFlags FClangGenerator::getDataType(const clang::QualType *type, const clang::ASTContext &context) {
    clang::PrintingPolicy policy(context.getLangOpts());
    policy.FullyQualifiedName = true;
    policy.PrintInjectedClassNameWithArguments = false;

    FString name = ANSI_TO_TCHAR(type->getAsString(policy).c_str());

    // remove prefix
    name.replace(TEXT("&"), FString::Empty);

    if (name.startWith(TEXT("TArray"))) {
        return QReflection::EPropertyGenFlags::Array;
    } else if (name.startWith(TEXT("TMap")) || name.startWith(TEXT("TUnorderedMap"))) {
        return QReflection::EPropertyGenFlags::Map;
    } else if (name.startWith(TEXT("TSet")) || name.startWith(TEXT("TUnorderedSet"))) {
        return QReflection::EPropertyGenFlags::Set;
    }

    if (name.endWith("int8_t") || name.endWith("char")) {
        return QReflection::EPropertyGenFlags::Int8; // maybe byte?
    } else if (name.endWith("uint8_t") || name.endWith("unsigned char")) {
        return QReflection::EPropertyGenFlags::UInt8;
    } else if (name.endWith("int32_t") || name.endWith("int")) {
        return QReflection::EPropertyGenFlags::Int32;
    } else if (name.endWith("uint32_t") || name.endWith("unsigned int")) {
        return QReflection::EPropertyGenFlags::UInt32;
    } else if (name.endWith("int64_t")) {
        return QReflection::EPropertyGenFlags::Int64;
    } else if (name.endWith("uint64_t")) {
        return QReflection::EPropertyGenFlags::UInt64;
    } else if (name.endWith("float")) {
        return QReflection::EPropertyGenFlags::Float;
    } else if (name.endWith("double")) {
        return QReflection::EPropertyGenFlags::Double;
    } else if (name.endWith("FString")) {
        return QReflection::EPropertyGenFlags::String;
    }

    auto typePtr = type->getTypePtr();

    if (typePtr->isClassType() || typePtr->isPointerType()) {
        return QReflection::EPropertyGenFlags::Class;
    }

    if (typePtr->isStructureType()) {
        return QReflection::EPropertyGenFlags::Struct;
    }

    if (name.startWith("F")) {
        return QReflection::EPropertyGenFlags::Struct;
    }

    // TODO:
    /*if (type->isClas) {
        return QReflection::EPropertyGenFlags::Class;
    }*/

    return QReflection::EPropertyGenFlags::None;
}

void FClangGenerator::generateTemplateArgsType(clang::CXXRecordDecl *record, size_t limit) {
    // TODO:
    auto specialization = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(record);
    auto args = &specialization->getTemplateArgs();

    if (args->size() > limit) {
        return;
    }

    const clang::ASTContext &context = record->getASTContext();
    clang::PrintingPolicy policy(context.getLangOpts());
    policy.FullyQualifiedName = true;
    policy.PrintInjectedClassNameWithArguments = false;

    for (auto i = 0; i < limit; i++) {
        auto arg = args->get(i).getAsType();
        auto type = getDataType(&arg, context);

        FString name = ANSI_TO_TCHAR(arg.getAsString(policy).c_str());
        name.replace(TEXT("*"), FString::Empty);
        name.replace(TEXT("&"), FString::Empty);
        name.trim();

        FString property = TEXT("QIntProperty");
        FString staticClass = TEXT("nullptr");

        switch (type) {
            case QReflection::EPropertyGenFlags::Byte:
            case QReflection::EPropertyGenFlags::Bool:
            case QReflection::EPropertyGenFlags::Int8:
            case QReflection::EPropertyGenFlags::UInt8:
                property = TEXT("QInt8Property");
                break;
            case QReflection::EPropertyGenFlags::Int32:
            case QReflection::EPropertyGenFlags::UInt32:
                property = TEXT("QIntProperty");
                break;
            case QReflection::EPropertyGenFlags::Int64:
            case QReflection::EPropertyGenFlags::UInt64:
                property = TEXT("QInt64Property");
                break;
            case QReflection::EPropertyGenFlags::Float:
                property = TEXT("QFloatProperty");
                break;
            case QReflection::EPropertyGenFlags::Double:
                property = TEXT("QDoubleProperty");
                break;
            case QReflection::EPropertyGenFlags::String:
                property = TEXT("QStringProperty");
                break;
            case QReflection::EPropertyGenFlags::Class:
                property = TEXT("QClassProperty");
                staticClass = name + TEXT("::StaticClass()");
                break;
            case QReflection::EPropertyGenFlags::Object:
                property = TEXT("QObjectProperty");
                staticClass = name + TEXT("::StaticClass()");
                break;
            case QReflection::EPropertyGenFlags::Struct:
                property = TEXT("QStructProperty");
                staticClass = name + TEXT("::StaticStruct()");
                break;
            case QReflection::EPropertyGenFlags::NativeArray:
            case QReflection::EPropertyGenFlags::Array:
            case QReflection::EPropertyGenFlags::Map:
            case QReflection::EPropertyGenFlags::Set:
                property = TEXT("QArrayProperty");
                break;

            default:
                break;
        }

        FNamedFormatterArgs args;
        args.add(TEXT("name"), name);
        args.add(TEXT("property"), property);
        args.add(TEXT("staticClass"), staticClass);
        args.add(TEXT("className"), mTopScope->currentName);

        mSourceFormatter.append(TEXT("    q_new<{{property}}>({{staticClass}}, TEXT(\"{{className}}_{{name}}_Template\"), 0),"), args);
    }
}
