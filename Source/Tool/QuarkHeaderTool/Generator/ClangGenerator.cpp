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

ClangGenerator::ClangGenerator(const Configuration &config, TArray<Symbol*> symbols)
    : mSymbols(symbols), mConfig(config), mHeaderFormatter(config.header), mSourceFormatter(config.source) { }

void ClangGenerator::generate(const clang::TranslationUnitDecl *tuDecl) {
    FStringBuilder sb(1024);
    sb.setDynamic();
    if (!mConfig.package.empty()) {
        sb << mConfig.package << TEXT("_");
    }

    sb << TEXT("Source_");
    sb << mConfig.path.getFilename() << TEXT("_h");

    mCurrentFileId = sb.toString();

    NamedFormatterArgs args;
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

void ClangGenerator::scrapeTranslationUnit(const clang::TranslationUnitDecl *tuDecl) {
    scrapeDeclContext(tuDecl);
}

void ClangGenerator::scrapeDeclContext(const clang::DeclContext *ctxDecl) {
    for (clang::DeclContext::decl_iterator it = ctxDecl->decls_begin(); it != ctxDecl->decls_end(); ++it) {
        const auto &decl = *it;
        if (decl->isInvalidDecl()) {
            continue;
        }

        auto named = clang::dyn_cast<clang::NamedDecl>(decl);
        if (named) {
            scrapeNamedDecl(named);
        } else {
            /*auto kind = ctxDecl->getDeclKind();
            if (kind == clang::Decl::Enum || kind == clang::Decl::EnumConstant) {
                scrapCXXRecordDecl(clang::dyn_cast<clang::CXXRecordDecl>(decl));
            }*/
        }
    }
}

void ClangGenerator::scrapeNamedDecl(const clang::NamedDecl *namedDecl) {
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

    case clang::Decl::Enum:
    case clang::Decl::EnumConstant:
        scrapCXXEnumDecl(clang::dyn_cast<clang::EnumDecl>(namedDecl));
        break;

    default:
        break;
    }
}

void ClangGenerator::scrapCXXRecordDecl(const clang::CXXRecordDecl *cxxRecordDecl) {
    if (cxxRecordDecl->isAnonymousStructOrUnion()) {
        scrapeDeclContext(cxxRecordDecl);
        return;
    }

    String name = ANSI_TO_TCHAR(cxxRecordDecl->getQualifiedNameAsString().c_str());

    auto symbol = mSymbols.findIf([name](Symbol *symbol) { return symbol->name == name; });
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
    } else if (type->isClassType()) {
        generateClass(cxxRecordDecl);
    } else {
        assert(false);
    }
}

void ClangGenerator::scrapCXXFieldDecl(const clang::FieldDecl *fieldDecl) {
    String name = ANSI_TO_TCHAR(fieldDecl->getNameAsString().c_str());

    auto symbol = mCurrentSymbol->children.findIf([name](Symbol *symbol) { return symbol->name == name; });
    if (symbol == nullptr) {
        return;
    }

    LOG(LogQHT, Debug, TEXT("visit field: %ls"), *name);
}

void ClangGenerator::scrapCXXEnumDecl(const clang::EnumDecl *enumDecl) {
    String name = ANSI_TO_TCHAR(enumDecl->getQualifiedNameAsString().c_str());

    auto symbol = mSymbols.findIf([name](Symbol *symbol) { return symbol->name == name; });
    if (symbol == nullptr) {
        return;
    }

    if ((*symbol)->marked) {
        return;
    }

    mCurrentSymbol = *symbol;

    const auto *type = enumDecl->getTypeForDecl();
    assert(type->isEnumeralType());

    generateEnum(enumDecl);
}

void ClangGenerator::setContext(clang::ASTContext *context) {
    mContext = context;
}

void ClangGenerator::pushScope(const String &name, EScopeType type) {
    auto scope = q_new<Scope>();
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

void ClangGenerator::popScope() {
    auto temp = mTopScope;
    mTopScope = temp->parent;

    LOG(LogQHT, Debug, TEXT("Scope popped: %ls"), *temp->fullName);
    q_delete(temp);
}

void ClangGenerator::generateStruct(const clang::CXXRecordDecl *record) {
    String name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());
    pushScope(name, EScopeType::Struct);

    auto generated = mCurrentSymbol->extras.find(GENERATED);
    if (generated == nullptr) {
        // error;
        return;
    }

    mCurrentSymbol->marked = true;
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add(TEXT("name"), name);
    args.add(TEXT("lineNo"), *generated);

    mHeaderFormatter.append(TEXT(R"(
#define {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS                        \
        friend struct Generated_Struct_{{name}}_Statics;                        \
        static Struct *StaticStruct();

#define {{currentFileId}}_{{lineNo}}_GENERATED_BODY                             \
        {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS
)"), args);

    mSourceFormatter.append(TEXT(R"(
Struct *Generated_Initializer_Struct_{{name}}();
Struct *{{name}}::StaticStruct() {
    static Struct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_{{name}}();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_{{name}}(&Generated_Initializer_Struct_{{name}}, &{{name}}::StaticStruct, TEXT("{{name}}"), TEXT("{{relativePath}}"));
)"), args);

    generateStatics(record, EScopeType::Struct);

    mSourceFormatter.append(TEXT(R"(
Struct *Generated_Initializer_Struct_{{name}}() {
    static Struct *instance = nullptr;
    if (!instance) {
        Reflection::CreateStruct(instance, Generated_Struct_{{name}}_Statics::StructDesc);
    }
    return instance;
}
)"), args);

    popScope();
}

void ClangGenerator::generateClass(const clang::CXXRecordDecl *record) {
    String name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());
    pushScope(name, EScopeType::Class);

    auto generated = mCurrentSymbol->extras.find(GENERATED);
    if (generated == nullptr) {
        // error;
        return;
    }

    mCurrentSymbol->marked = true;
    bool bIsAbstract = mCurrentSymbol->metas.find(TEXT("abstract")) != nullptr;

    String base = String::Empty;
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

    NamedFormatterArgs args;
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
        mHeaderFormatter.
            appendLine(TEXT(R"(    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL({{name}}) \)"), args);
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
Class *Generated_Initializer_Class_{{name}}();
static InitClassOnStart Generated_InitClassOnStart_Class_{{name}}(&Generated_Initializer_Class_{{name}}, &{{name}}::StaticClass, TEXT("{{name}}"), TEXT("{{relativePath}}"));

)"), args);

    generateStatics(record, EScopeType::Class);

    mSourceFormatter.append(TEXT(R"(
Class *Generated_Initializer_Class_{{name}}() {
    static Class *instance = nullptr;
    if (!instance) {
        Reflection::CreateClass(instance, Generated_Class_{{name}}_Statics::ClassDesc);
    }
    return instance;
}
)"), args);

    popScope();
}

void ClangGenerator::generateEnum(const clang::EnumDecl *record) {
    String name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());
    pushScope(name, EScopeType::Class);

    mCurrentSymbol->marked = true;
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add(TEXT("name"), name);

    mSourceFormatter.append(TEXT(R"(
Enum *Generated_Initializer_Enum_{{name}}();
Enum *{{name}}_StaticEnum() {
    static Enum *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Enum_{{name}}();
    }
    return instance;
}

static FInitEnumOnStart Generated_InitClassOnStart_Enum_{{name}}({{name}}_StaticEnum, TEXT("{{name}}"), TEXT("{{relativePath}}"));

Enum *Generated_Initializer_Enum_{{name}}() {
    static Enum *instance = nullptr;
    if (!instance) {
        static const TArray<Reflection::EnumEntry> entires = {)"), args);

    // generateStatics(record, EScopeType::Struct);

    auto children = mCurrentSymbol->children;
    for (auto it = record->enumerator_begin(); it != record->enumerator_end(); ++it) {
        String fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), name);
        fieldArgs.add(TEXT("fieldName"), fieldName);

        mSourceFormatter.append(TEXT(R"(
            { TEXT("{{name}}::{{fieldName}}"), (int64_t) {{name}}::{{fieldName}} },)"), fieldArgs);
    }

    mSourceFormatter.append(TEXT(R"(
        };

        static const TArray<Reflection::MetaDataPairDesc> metas = {
        };

        static const Reflection::EnumDesc desc = {
                TEXT("ETestEnum"),
                TEXT("enum"),
                entires,
                metas,
        };

        Reflection::CreateEnum(instance, desc);
    }
    return instance;
}
)"), args);

    popScope();
}

void ClangGenerator::generateStatics(const clang::CXXRecordDecl *record, EScopeType scope) {
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    String name = ANSI_TO_TCHAR(record->getQualifiedNameAsString().c_str());

    args.add(TEXT("name"), name);
    args.add(TEXT("type"), scope == EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

    mSourceFormatter.appendLine(TEXT("struct Generated_{{type}}_{{name}}_Statics {"), args);
    mSourceFormatter.addIndent();

    auto children = mCurrentSymbol->children;
    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        String fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        auto symbol = *found;
        auto fieldType = (*it)->getType();

        auto typeName = fieldType->getTypeClassName();

        Reflection::EPropertyGenFlags type = getDataType(&fieldType, record->getASTContext());
        args.add(TEXT("genFlags"), static_cast<int>(type));

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), fieldName);

        if (fieldType->isBuiltinType()) {
            mSourceFormatter.appendLine(
                TEXT("static const Reflection::GenericPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
        } else {
            switch (type) {
            case Reflection::EPropertyGenFlags::Object:
                mSourceFormatter.appendLine(
                    TEXT("static const Reflection::ObjectPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case Reflection::EPropertyGenFlags::Struct:
                mSourceFormatter.appendLine(
                    TEXT("static const Reflection::StructPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case Reflection::EPropertyGenFlags::Array:
                mSourceFormatter.appendLine(
                    TEXT("static const Reflection::ArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case Reflection::EPropertyGenFlags::Map:
                mSourceFormatter.appendLine(
                    TEXT("static const Reflection::MapPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case Reflection::EPropertyGenFlags::Set:
                mSourceFormatter.appendLine(
                    TEXT("static const Reflection::ArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            default:
                mSourceFormatter.appendLine(
                    TEXT("static const Reflection::GenericPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;
            }
        }

        mSourceFormatter.appendLine(
            TEXT("static const TArray<Reflection::MetaDataPairDesc> {{name}}_MetaData;"), fieldArgs, true);
    }

    mSourceFormatter.append(TEXT(R"(
    static const TArray<Reflection::MetaDataPairDesc> {{type}}MetaData;
    static const TArray<Reflection::PropertyDescBase const*> {{type}}Properties;
    static const Reflection::{{type}}Desc {{type}}Desc;
};
)"), args);

    mSourceFormatter.removeIndent();
    pushScope(name, scope);

    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        String fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        generateField((*it), *found);
    }

    popScope();

    mSourceFormatter.append(TEXT(R"(
const TArray<Reflection::MetaDataPairDesc> Generated_{{type}}_{{name}}_Statics::{{type}}MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    uint64_t flags = 0;
    for (auto entry : mCurrentSymbol->metas) {
        NamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.key).add(TEXT("mValue"), entry.value);

        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(R"(};)");

    mSourceFormatter.appendLine(
        TEXT(
            "const TArray<Reflection::PropertyDescBase const*> Generated_{{type}}_{{name}}_Statics::{{type}}Properties = {"),
        args, true);
    mSourceFormatter.addIndent();

    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        String fieldName = ANSI_TO_TCHAR((*it)->getNameAsString().c_str());
        auto found = children.findIf([fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == nullptr) {
            continue;
        }

        auto symbol = *found;
        auto fieldType = (*it)->getType();

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), fieldName)
                 .add(TEXT("typeName"), name)
                 .add(TEXT("type"), scope == EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

        mSourceFormatter.appendLine(
            TEXT(
                "(const Reflection::PropertyDescBase *) &Generated_{{type}}_{{typeName}}_Statics::{{name}}_PropertyDesc,"),
            fieldArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine("};");

    args.add(TEXT("flags"), 0);

    mSourceFormatter.append(TEXT(R"(
const Reflection::{{type}}Desc Generated_{{type}}_{{name}}_Statics::{{type}}Desc = {
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

void ClangGenerator::generateField(clang::FieldDecl *field, Symbol *symbol) {
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    String name = ANSI_TO_TCHAR(field->getNameAsString().c_str());
    args.add(TEXT("name"), name);

    auto policy = clang::PrintingPolicy(field->getASTContext().getLangOpts());
    policy.SuppressTagKeyword = true;

    auto fieldType = field->getType();
    String fieldTypeName = ANSI_TO_TCHAR(fieldType.getAsString(policy).c_str());

    args.add(TEXT("scopeName"), mTopScope->currentName);
    args.add(TEXT("fieldTypeName"), fieldTypeName);

    bool bIsClass = mTopScope->type == EScopeType::Class;
    String keywordName = bIsClass ? TEXT("Class") : TEXT("Struct");
    args.add(TEXT("keywordName"), keywordName);

    uint64_t flags = 0;
    TMap<String, String> metas;

    Reflection::EPropertyGenFlags type = getDataType(&fieldType, field->getASTContext());
    args.add(TEXT("genFlags"), static_cast<int>(type));

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
        if (!entry.value.empty()) {
            metas.add(entry.key, entry.value);
            continue;
        }

        if (entry.key.contains(TEXT("."))) {
            metas.add(entry.key, entry.value);
            continue;
        }

        if (entry.key == "NonSerialized") {
            flags |= PropertyFlags_NonSerialized;
            continue;
        }

        metas.add(entry.key, entry.value);
    }

    args.add(TEXT("flags"), flags);

    if (fieldType->isBuiltinType()) {
        mSourceFormatter.append(TEXT(R"(
const Reflection::GenericPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
    } else {
        switch (type) {
        case Reflection::EPropertyGenFlags::Object:
            mSourceFormatter.append(TEXT(R"(
const Reflection::ObjectPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case Reflection::EPropertyGenFlags::Struct:
            mSourceFormatter.append(TEXT(R"(
const Reflection::StructPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    {{fieldTypeName}}::StaticStruct,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case Reflection::EPropertyGenFlags::Class:
            mSourceFormatter.append(TEXT(R"(
const Reflection::StructPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    {{fieldTypeName}}::StaticClass,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);

        case Reflection::EPropertyGenFlags::Array:
        case Reflection::EPropertyGenFlags::Set:
            mSourceFormatter.append(TEXT(R"(
const Reflection::ArrayPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
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

        case Reflection::EPropertyGenFlags::Map:
            mSourceFormatter.append(TEXT(R"(
const Reflection::ArrayPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
)"), args);
            generateTemplateArgsType(field->getType()->getAsCXXRecordDecl(), 2);
            mSourceFormatter.append(TEXT(R"(
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        default:
            mSourceFormatter.append(TEXT(R"(
const Reflection::GenericPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
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
const TArray<Reflection::MetaDataPairDesc> Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    for (auto entry : symbol->metas) {
        NamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.key).add(TEXT("mValue"), entry.value);

        /*if (!entry.value.empty()) {
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
            continue;
        }

        if (entry.key.contains(TEXT("."))) {
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
            continue;
        }*/

        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{mValue}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(TEXT("};"), args);
}

Reflection::EPropertyGenFlags ClangGenerator::getDataType(const clang::QualType *type,
                                                          const clang::ASTContext &context) {
    clang::PrintingPolicy policy(context.getLangOpts());
    // policy.FullyQualifiedName = true;
    // policy.PrintInjectedClassNameWithArguments = false;
    policy.SuppressTagKeyword = true;

    String name = ANSI_TO_TCHAR(type->getAsString(policy).c_str());

    // remove prefix
    name.replace(TEXT("&"), String::Empty);

    if (name.startWith(TEXT("TArray"))) {
        return Reflection::EPropertyGenFlags::Array;
    }
    if (name.startWith(TEXT("TMap")) || name.startWith(TEXT("TUnorderedMap"))) {
        return Reflection::EPropertyGenFlags::Map;
    }
    if (name.startWith(TEXT("TSet")) || name.startWith(TEXT("TUnorderedSet"))) {
        return Reflection::EPropertyGenFlags::Set;
    }

    if (name.endWith("int8_t") || name.endWith("char")) {
        return Reflection::EPropertyGenFlags::Int8; // maybe byte?
    }
    if (name.endWith("uint8_t") || name.endWith("unsigned char")) {
        return Reflection::EPropertyGenFlags::UInt8;
    }
    if (name.endWith("int32_t") || name.endWith("int")) {
        return Reflection::EPropertyGenFlags::Int32;
    }
    if (name.endWith("uint32_t") || name.endWith("unsigned int")) {
        return Reflection::EPropertyGenFlags::UInt32;
    }
    if (name.endWith("int64_t")) {
        return Reflection::EPropertyGenFlags::Int64;
    }
    if (name.endWith("uint64_t")) {
        return Reflection::EPropertyGenFlags::UInt64;
    }
    if (name.endWith("float")) {
        return Reflection::EPropertyGenFlags::Float;
    }
    if (name.endWith("double")) {
        return Reflection::EPropertyGenFlags::Double;
    }
    if (name.endWith("String")) {
        return Reflection::EPropertyGenFlags::String;
    }

    auto typePtr = type->getTypePtr();

    if (typePtr->isStructureType()) {
        return Reflection::EPropertyGenFlags::Struct;
    }

    if (typePtr->isClassType() || typePtr->isPointerType()) {
        return Reflection::EPropertyGenFlags::Class;
    }

    /*if (name.startWith("F")) {
        return Reflection::EPropertyGenFlags::Struct;
    }*/

    // TODO:
    /*if (type->isClas) {
        return Reflection::EPropertyGenFlags::Class;
    }*/

    return Reflection::EPropertyGenFlags::None;
}

void ClangGenerator::generateTemplateArgsType(clang::CXXRecordDecl *record, size_t limit) {
    // TODO:
    auto specialization = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(record);
    auto args = &specialization->getTemplateArgs();

    /*if (args->size() > limit) {
        return;
    }*/

    const clang::ASTContext &context = record->getASTContext();
    clang::PrintingPolicy policy(context.getLangOpts());
    policy.FullyQualifiedName = true;
    // policy.PrintInjectedClassNameWithArguments = false;

    for (auto i = 0; i < std::min<int>(args->size(), limit); i++) {
        auto arg = args->get(i).getAsType();
        auto type = getDataType(&arg, context);

        String name = ANSI_TO_TCHAR(arg.getAsString(policy).c_str());
        name.replace(TEXT("*"), String::Empty);
        name.replace(TEXT("&"), String::Empty);
        name.trim();

        String property = TEXT("IntProperty");
        String staticClass = TEXT("nullptr");

        switch (type) {
        case Reflection::EPropertyGenFlags::Byte:
        case Reflection::EPropertyGenFlags::Bool:
        case Reflection::EPropertyGenFlags::Int8:
        case Reflection::EPropertyGenFlags::UInt8:
            property = TEXT("Int8Property");
            break;
        case Reflection::EPropertyGenFlags::Int32:
        case Reflection::EPropertyGenFlags::UInt32:
            property = TEXT("IntProperty");
            break;
        case Reflection::EPropertyGenFlags::Int64:
        case Reflection::EPropertyGenFlags::UInt64:
            property = TEXT("Int64Property");
            break;
        case Reflection::EPropertyGenFlags::Float:
            property = TEXT("FloatProperty");
            break;
        case Reflection::EPropertyGenFlags::Double:
            property = TEXT("DoubleProperty");
            break;
        case Reflection::EPropertyGenFlags::String:
            property = TEXT("StringProperty");
            break;
        case Reflection::EPropertyGenFlags::Class:
            property = TEXT("ClassProperty");
            staticClass = name + TEXT("::StaticClass()");
            break;
        case Reflection::EPropertyGenFlags::Object:
            property = TEXT("ObjectProperty");
            staticClass = name + TEXT("::StaticClass()");
            break;
        case Reflection::EPropertyGenFlags::Struct:
            property = TEXT("StructProperty");
            staticClass = name + TEXT("::StaticStruct()");
            break;
        case Reflection::EPropertyGenFlags::NativeArray:
        case Reflection::EPropertyGenFlags::Array:
        case Reflection::EPropertyGenFlags::Set:
            property = TEXT("ArrayProperty");
            break;

        case Reflection::EPropertyGenFlags::Map:
            property = TEXT("MapProperty");
            break;

        default:
            break;
        }

        NamedFormatterArgs args;
        args.add(TEXT("name"), name);
        args.add(TEXT("property"), property);
        args.add(TEXT("staticClass"), staticClass);
        args.add(TEXT("className"), mTopScope->currentName);

        mSourceFormatter.append(
            TEXT("    q_new<{{property}}>({{staticClass}}, TEXT(\"{{className}}_{{name}}_Template\"), 0),"), args);
    }
}
