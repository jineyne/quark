#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/ASTContext.h>

#include <Reflection/ReflectionTypes.h>

#include "ClangGenerator.h"

#define APPEND_DEFAULT_ARGS(TARGET) \
    (TARGET).add(("package"), mConfig.package) \
            .add(("path"), mConfig.path.string()) \
            .add(("relativePath"), mConfig.relativePath.string()) \
            .add(("filename"), mConfig.path.filename().string()) \
            .add(("currentFileId"), mCurrentFileId)

ClangGenerator::ClangGenerator(const Configuration &config, std::vector<Symbol*> symbols)
    : mSymbols(symbols), mConfig(config), mHeaderFormatter(config.header), mSourceFormatter(config.source) { }

void ClangGenerator::generate(const clang::TranslationUnitDecl *tuDecl) {
    std::stringstream sb(1024);
    if (!mConfig.package.empty()) {
        sb << mConfig.package << ("_");
    }

    sb << ("Source_");
    sb << mConfig.path.string() << ("_h");

    mCurrentFileId = sb.str();

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

    mSourceFormatter.append((R"(
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

    std::string name = cxxRecordDecl->getQualifiedNameAsString().c_str();

    auto symbol = std::find_if(mSymbols.begin(), mSymbols.end(), [name](Symbol *symbol) { return symbol->name == name; });
    if (symbol == mSymbols.end()) {
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
    std::string name = fieldDecl->getNameAsString();

    auto symbol = std::find_if(mCurrentSymbol->children.begin(), mCurrentSymbol->children.end(), [name](Symbol *symbol) { return symbol->name == name; });
    if (symbol == mCurrentSymbol->children.end()) {
        return;
    }

    std::cout << "visit field" << name << std::endl;
}

void ClangGenerator::scrapCXXEnumDecl(const clang::EnumDecl *enumDecl) {
    std::string name = enumDecl->getQualifiedNameAsString();

    auto symbol = std::find_if(mSymbols.begin(), mSymbols.end(), [name](Symbol *symbol) { return symbol->name == name; });
    if (symbol == mSymbols.end()) {
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

void ClangGenerator::pushScope(const std::string &name, EScopeType type) {
    auto scope = new Scope();
    scope->currentName = name;
    scope->type = type;
    if (mTopScope == nullptr) {
        mTopScope = scope;
        mTopScope->parent = nullptr;
        mTopScope->fullName = name;
    } else {
        scope->parent = mTopScope;
        scope->fullName = mTopScope->fullName + ("::") + name;
        scope->parent = mTopScope;
        mTopScope = scope;
    }

    std::cout << "Scope pushed: " << mTopScope->fullName << std::endl;
}

void ClangGenerator::popScope() {
    auto temp = mTopScope;
    mTopScope = temp->parent;

    std::cout << "Scope popped: " << temp->fullName << std::endl;
    delete temp;
}

void ClangGenerator::generateStruct(const clang::CXXRecordDecl *record) {
    std::string name = record->getQualifiedNameAsString().c_str();
    pushScope(name, EScopeType::Struct);

    auto generated = mCurrentSymbol->extras.find(GENERATED);
    if (generated == mCurrentSymbol->extras.end()) {
        // error;
        return;
    }

    mCurrentSymbol->marked = true;
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add("name", name);
    args.add("lineNo", generated->second);

    mHeaderFormatter.append((R"(
#define {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS                        \
        friend struct Generated_Struct_{{name}}_Statics;                        \
        static Struct *StaticStruct();

#define {{currentFileId}}_{{lineNo}}_GENERATED_BODY                             \
        {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS
)"), args);

    mSourceFormatter.append((R"(
Struct *Generated_Initializer_Struct_{{name}}();
Struct *{{name}}::StaticStruct() {
    static Struct *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Struct_{{name}}();
    }
    return instance;
}

static FInitStructOnStart Generated_InitClassOnStart_Struct_{{name}}(&Generated_Initializer_Struct_{{name}}, &{{name}}::StaticStruct, ("{{name}}"), ("{{relativePath}}"));
)"), args);

    generateStatics(record, EScopeType::Struct);

    mSourceFormatter.append((R"(
Struct *Generated_Initializer_Struct_{{name}}() {
    static Struct *instance = nullptr;
    if (!instance) {
        CreateStruct(instance, Generated_Struct_{{name}}_Statics::StructDesc);
    }
    return instance;
}
)"), args);

    popScope();
}

void ClangGenerator::generateClass(const clang::CXXRecordDecl *record) {
    std::string name = record->getQualifiedNameAsString().c_str();
    pushScope(name, EScopeType::Class);

    auto generated = mCurrentSymbol->extras.find(GENERATED);
    if (generated == mCurrentSymbol->extras.end()) {
        // error;
        return;
    }

    mCurrentSymbol->marked = true;
    bool bIsAbstract = mCurrentSymbol->metas.find(("abstract")) != mCurrentSymbol->metas.end();

    std::string base = "";
    if (!record->bases().empty()) {
        for (auto it = record->bases_begin(); it != record->bases_end(); ++it) {
            if (!base.empty()) {
                // error
                std::cerr << "Multiple base is not support!" << std::endl;
                break;
            }

            auto baseRecord = (*it).getType()->getAsCXXRecordDecl();
            base = baseRecord->getQualifiedNameAsString().c_str();
        }
    }

    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add(("name"), name);
    args.add(("lineNo"), generated->second);
    args.add(("base"), base);

    mHeaderFormatter.append((R"(
#define {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS \
private: \
    friend struct Generated_Class_{{name}}_Statics; \
    static void StaticRegisterNative{{name}}(); \
public: \
    DECLARE_CLASS({{name}}, {{base}}, ) \
)"), args);
    if (!bIsAbstract) {
        mHeaderFormatter.
            appendLine((R"(    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL({{name}}) \)"), args);
    }
    mHeaderFormatter.append((R"(    DECLARE_SERIALIZER({{name}})

#define {{currentFileId}}_{{lineNo}}_GENERATED_BODY \
        {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS
)"), args);

    mSourceFormatter.appendLine(("void {{name}}::StaticRegisterNative{{name}}() {"), args);
    mSourceFormatter.addIndent();

    // function declares

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(("}"));

    if (bIsAbstract) {
        mSourceFormatter.append((R"(IMPLEMENT_CLASS_NO_CTR({{name}});)"), args);
    } else {
        mSourceFormatter.append((R"(IMPLEMENT_CLASS({{name}});)"), args);
    }

    mSourceFormatter.append((R"(
Class *Generated_Initializer_Class_{{name}}();
static InitClassOnStart Generated_InitClassOnStart_Class_{{name}}(&Generated_Initializer_Class_{{name}}, &{{name}}::StaticClass, ("{{name}}"), ("{{relativePath}}"));

)"), args);

    generateStatics(record, EScopeType::Class);

    mSourceFormatter.append((R"(
Class *Generated_Initializer_Class_{{name}}() {
    static Class *instance = nullptr;
    if (!instance) {
        CreateClass(instance, Generated_Class_{{name}}_Statics::ClassDesc);
    }
    return instance;
}
)"), args);

    popScope();
}

void ClangGenerator::generateEnum(const clang::EnumDecl *record) {
    std::string name = record->getQualifiedNameAsString();
    pushScope(name, EScopeType::Class);

    mCurrentSymbol->marked = true;
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);
    args.add(("name"), name);

    mSourceFormatter.append((R"(
Enum *Generated_Initializer_Enum_{{name}}();
Enum *{{name}}_StaticEnum() {
    static Enum *instance = nullptr;
    if (!instance) {
        instance = Generated_Initializer_Enum_{{name}}();
    }
    return instance;
}

static FInitEnumOnStart Generated_InitClassOnStart_Enum_{{name}}({{name}}_StaticEnum, ("{{name}}"), ("{{relativePath}}"));

Enum *Generated_Initializer_Enum_{{name}}() {
    static Enum *instance = nullptr;
    if (!instance) {
        static const std::vector<EnumEntry> entires = {)"), args);

    // generateStatics(record, EScopeType::Struct);

    auto children = mCurrentSymbol->children;
    for (auto it = record->enumerator_begin(); it != record->enumerator_end(); ++it) {
        std::string fieldName = (*it)->getNameAsString();
        auto found = std::find_if(children.begin(), children.end(), [fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == children.end()) {
            continue;
        }

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(("name"), name);
        fieldArgs.add(("fieldName"), fieldName);

        mSourceFormatter.append((R"(
            { ("{{name}}::{{fieldName}}"), (int64_t) {{name}}::{{fieldName}} },)"), fieldArgs);
    }

    mSourceFormatter.append((R"(
        };

        static const std::vector<MetaDataPairDesc> metas = {
        };

        static const EnumDesc desc = {
                ("ETestEnum"),
                ("enum"),
                entires,
                metas,
        };

        CreateEnum(instance, desc);
    }
    return instance;
}
)"), args);

    popScope();
}

void ClangGenerator::generateStatics(const clang::CXXRecordDecl *record, EScopeType scope) {
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    std::string name = record->getQualifiedNameAsString();

    args.add("name", name);
    args.add("type", (std::string) (scope == EScopeType::Class ? "Class" : "Struct"));

    mSourceFormatter.appendLine(("struct Generated_{{type}}_{{name}}_Statics {"), args);
    mSourceFormatter.addIndent();

    auto children = mCurrentSymbol->children;
    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        std::string fieldName = (*it)->getNameAsString();
        auto found = std::find_if(children.begin(), children.end(), [fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == children.end()) {
            continue;
        }

        auto symbol = *found;
        auto fieldType = (*it)->getType();

        auto typeName = fieldType->getTypeClassName();

        auto type = (EPropertyGenFlags) getDataType(&fieldType, record->getASTContext());
        args.add(("genFlags"), static_cast<int>(type));

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(("name"), fieldName);

        if (fieldType->isBuiltinType()) {
            mSourceFormatter.appendLine(
                ("static const GenericPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
        } else {
            switch (type) {
            case EPropertyGenFlags::Object:
                mSourceFormatter.appendLine(
                    ("static const ObjectPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case EPropertyGenFlags::Class:
                mSourceFormatter.appendLine(
                        ("static const ClassPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case EPropertyGenFlags::Struct:
                mSourceFormatter.appendLine(
                    ("static const StructPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case EPropertyGenFlags::Array:
                mSourceFormatter.appendLine(
                    ("static const ArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case EPropertyGenFlags::Map:
                mSourceFormatter.appendLine(
                    ("static const MapPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            case EPropertyGenFlags::Set:
                mSourceFormatter.appendLine(
                    ("static const ArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;

            default:
                mSourceFormatter.appendLine(
                    ("static const GenericPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
                break;
            }
        }

        mSourceFormatter.appendLine(
            ("static const std::vector<MetaDataPairDesc> {{name}}_MetaData;"), fieldArgs, true);
    }

    mSourceFormatter.append((R"(
    static const std::vector<MetaDataPairDesc> {{type}}MetaData;
    static const std::vector<PropertyDescBase const*> {{type}}Properties;
    static const {{type}}Desc {{type}}Desc;
};
)"), args);

    mSourceFormatter.removeIndent();
    pushScope(name, scope);

    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        std::string fieldName = (*it)->getNameAsString();
        auto found = std::find_if(children.begin(), children.end(), [fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == children.end()) {
            continue;
        }

        generateField((*it), *found);
    }

    popScope();

    mSourceFormatter.append((R"(
const std::vector<MetaDataPairDesc> Generated_{{type}}_{{name}}_Statics::{{type}}MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    uint64_t flags = 0;
    for (auto entry : mCurrentSymbol->metas) {
        NamedFormatterArgs metaArgs;
        metaArgs.add(("key"), entry.first).add(("mValue"), entry.second);

        mSourceFormatter.appendLine((R"({("{{key}}"), ("{{mValue}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(R"(};)");

    mSourceFormatter.appendLine(
        (
            "const std::vector<PropertyDescBase const*> Generated_{{type}}_{{name}}_Statics::{{type}}Properties = {"),
        args, true);
    mSourceFormatter.addIndent();

    for (auto it = record->field_begin(); it != record->field_end(); ++it) {
        std::string fieldName = (*it)->getNameAsString();
        auto found = std::find_if(children.begin(), children.end(), [fieldName](Symbol *symbol) { return symbol->name == fieldName; });
        if (found == children.end()) {
            continue;
        }

        auto symbol = *found;
        auto fieldType = (*it)->getType();

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(("name"), fieldName)
                 .add(("typeName"), name)
                 .add(("type"), scope == EScopeType::Class ? ("Class") : ("Struct"));

        mSourceFormatter.appendLine(
            (
                "(const PropertyDescBase *) &Generated_{{type}}_{{typeName}}_Statics::{{name}}_PropertyDesc,"),
            fieldArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine("};");

    args.add(("flags"), 0);

    mSourceFormatter.append((R"(
const {{type}}Desc Generated_{{type}}_{{name}}_Statics::{{type}}Desc = {
    ("{{name}}"),
    {{name}}::Static{{type}},
    (E{{type}}Flags) {{flags}},
)"), args);

    if (scope == EScopeType::Struct) {
        mSourceFormatter.append((R"(    sizeof({{name}}),)"), args);
    }

    mSourceFormatter.append((R"(
    Generated_{{type}}_{{name}}_Statics::{{type}}Properties,
    Generated_{{type}}_{{name}}_Statics::{{type}}MetaData,
};
)"), args);
}

void replace(std::string &string, std::string from, std::string to) {
    string.replace(string.find(from), from.length(), to);
}

inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v") {
    s.erase(0, s.find_first_not_of(t));
    return s;
}

inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v") {
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}


inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v") {
    return ltrim(rtrim(s, t), t);
}

void ClangGenerator::generateField(clang::FieldDecl *field, Symbol *symbol) {
    NamedFormatterArgs args;
    APPEND_DEFAULT_ARGS(args);

    std::string name = field->getNameAsString();
    args.add(("name"), name);

    auto policy = clang::PrintingPolicy(field->getASTContext().getLangOpts());
    policy.SuppressTagKeyword = true;

    auto fieldType = field->getType();
    std::string fieldTypeName = fieldType.getAsString(policy);
    replace(fieldTypeName, "*", "");
    replace(fieldTypeName, "&", "");
    trim(fieldTypeName);

    args.add(("scopeName"), mTopScope->currentName);
    args.add(("fieldTypeName"), fieldTypeName);

    bool bIsClass = mTopScope->type == EScopeType::Class;
    std::string keywordName = bIsClass ? ("Class") : ("Struct");
    args.add(("keywordName"), keywordName);

    uint64_t flags = 0;
    std::map<std::string, std::string> metas;

    auto type = (EPropertyGenFlags) getDataType(&fieldType, field->getASTContext());
    args.add(("genFlags"), static_cast<int>(type));

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
            metas.insert(std::make_pair(entry.first, entry.second));
            continue;
        }

        if (entry.first.find((".")) != std::string::npos) {
            metas.insert(std::make_pair(entry.first, entry.second));
            continue;
        }

        if (entry.first == "NonSerialized") {
            flags |= PropertyFlags_NonSerialized;
            continue;
        }

        metas.insert(std::make_pair(entry.first, entry.second));
    }

    args.add(("flags"), flags);

    if (fieldType->isBuiltinType()) {
        mSourceFormatter.append((R"(
const GenericPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
    } else {
        switch (type) {
        case EPropertyGenFlags::Object:
            mSourceFormatter.append((R"(
const ObjectPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case EPropertyGenFlags::Struct:
            mSourceFormatter.append((R"(
const StructPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    {{fieldTypeName}}::StaticStruct,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case EPropertyGenFlags::Class:
            mSourceFormatter.append((R"(
const ClassPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    {{fieldTypeName}}::StaticClass,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case EPropertyGenFlags::Array:
        case EPropertyGenFlags::Set:
            mSourceFormatter.append((R"(
const ArrayPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
)"), args);
            generateTemplateArgsType(field->getType()->getAsCXXRecordDecl(), 1);
            mSourceFormatter.append((R"(
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case EPropertyGenFlags::Map:
            mSourceFormatter.append((R"(
const MapPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
)"), args);
            pushScope(mTopScope->currentName + ("_") + name, EScopeType::Class);
            generateTemplateArgsType(field->getType()->getAsCXXRecordDecl(), 2);
            popScope();
            mSourceFormatter.append((R"(
    nullptr,
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        default:
            mSourceFormatter.append((R"(
const GenericPropertyDesc Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    ("{{name}}"),
    (EPropertyFlags) {{flags}},
    (EPropertyGenFlags) {{genFlags}},
    sizeof({{scopeName}}::{{name}}),
    1,
    offsetof({{scopeName}}, {{name}}),
    Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;
        }
    }

    mSourceFormatter.append((R"(
const std::vector<MetaDataPairDesc> Generated_{{keywordName}}_{{scopeName}}_Statics::{{name}}_MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    for (auto entry : symbol->metas) {
        NamedFormatterArgs metaArgs;
        metaArgs.add(("key"), entry.first).add(("mValue"), entry.second);

        /*if (!entry.value.empty()) {
            mSourceFormatter.appendLine((R"({("{{key}}"), ("{{mValue}}")},)"), metaArgs, true);
            continue;
        }

        if (entry.key.contains(("."))) {
            mSourceFormatter.appendLine((R"({("{{key}}"), ("{{mValue}}")},)"), metaArgs, true);
            continue;
        }*/

        mSourceFormatter.appendLine((R"({("{{key}}"), ("{{mValue}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(("};"), args);
}

bool startsWith(const std::string& fullString, const std::string& prefix) {
    if (fullString.length() < prefix.length()) {
        return false;
    }
    return fullString.compare(0, prefix.length(), prefix) == 0;
}

bool endsWith(const std::string& fullString, const std::string& suffix) {
    if (fullString.length() < suffix.length()) {
        return false;
    }
    return fullString.compare(fullString.length() - suffix.length(), suffix.length(), suffix) == 0;
}

int ClangGenerator::getDataType(const clang::QualType *type, const clang::ASTContext &context) {
    clang::PrintingPolicy policy(context.getLangOpts());
    // policy.FullyQualifiedName = true;
    // policy.PrintInjectedClassNameWithArguments = false;
    policy.SuppressTagKeyword = true;

    std::string name = type->getAsString(policy);

    // remove prefix
    replace(name, ("&"), "");

    if (startsWith(name, ("std::vector"))) {
        return (int) EPropertyGenFlags::Array;
    }
    if (startsWith(name, ("TMap")) || startsWith(name, ("TUnorderedMap"))) {
        return (int) EPropertyGenFlags::Map;
    }
    if (startsWith(name, ("TSet")) || startsWith(name, ("TUnorderedSet"))) {
        return (int) EPropertyGenFlags::Set;
    }
    if (startsWith(name, ("TResourceHandle"))) {
        return (int) EPropertyGenFlags::Resource;
    }

    if (endsWith(name, "int8_t") || endsWith(name, "char")) {
        return (int) EPropertyGenFlags::Int8; // maybe byte?
    }
    if (endsWith(name, "uint8_t") || endsWith(name, "unsigned char")) {
        return (int) EPropertyGenFlags::UInt8;
    }
    if (endsWith(name, "int32_t") || endsWith(name, "int")) {
        return (int) EPropertyGenFlags::Int32;
    }
    if (endsWith(name, "uint32_t") || endsWith(name, "unsigned int")) {
        return (int) EPropertyGenFlags::UInt32;
    }
    if (endsWith(name, "int64_t")) {
        return (int) EPropertyGenFlags::Int64;
    }
    if (endsWith(name, "uint64_t")) {
        return (int) EPropertyGenFlags::UInt64;
    }
    if (endsWith(name, "float")) {
        return (int) EPropertyGenFlags::Float;
    }
    if (endsWith(name, "double")) {
        return (int) EPropertyGenFlags::Double;
    }
    if (endsWith(name, "std::string")) {
        return (int) EPropertyGenFlags::String;
    }

    auto typePtr = type->getTypePtr();

    if (typePtr->isEnumeralType()) {
        return (int) EPropertyGenFlags::Int32;
    }

    if (typePtr->isClassType()) {
        return (int) EPropertyGenFlags::Class;
    }

    if (typePtr->isStructureType()) {
        return (int) EPropertyGenFlags::Struct;
    }

    if (typePtr->isPointerType()) {
        auto ptrType = typePtr->getPointeeCXXRecordDecl();
        if (ptrType->isClass()) {
            return (int) EPropertyGenFlags::Class;
        } else if (ptrType->isStruct()) {
            return (int) EPropertyGenFlags::Struct;
        }
    }

    return (int) EPropertyGenFlags::None;
}

void ClangGenerator::generateTemplateArgsType(clang::CXXRecordDecl *record, size_t limit) {
    // TODO:
    auto specialization = clang::dyn_cast<clang::ClassTemplateSpecializationDecl>(record);
    if (specialization == nullptr) {
        return;
    }

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
        auto type = (EPropertyGenFlags) getDataType(&arg, context);

        std::string name = arg.getAsString(policy).c_str();
        replace(name, "*", "");
        replace(name, "&", "");
        trim(name);

        std::string property = ("IntProperty");
        std::string staticClass = ("nullptr");

        switch (type) {
        case EPropertyGenFlags::Byte:
        case EPropertyGenFlags::Bool:
        case EPropertyGenFlags::Int8:
        case EPropertyGenFlags::UInt8:
            property = ("Int8Property");
            break;
        case EPropertyGenFlags::Int32:
        case EPropertyGenFlags::UInt32:
            property = ("IntProperty");
            break;
        case EPropertyGenFlags::Int64:
        case EPropertyGenFlags::UInt64:
            property = ("Int64Property");
            break;
        case EPropertyGenFlags::Float:
            property = ("FloatProperty");
            break;
        case EPropertyGenFlags::Double:
            property = ("DoubleProperty");
            break;
        case EPropertyGenFlags::String:
            property = ("StringProperty");
            break;
        case EPropertyGenFlags::Class:
            property = ("ClassProperty");
            staticClass = name + ("::StaticClass()");
            break;
        case EPropertyGenFlags::Object:
            property = ("ObjectProperty");
            staticClass = name + ("::StaticClass()");
            break;
        case EPropertyGenFlags::Struct:
            property = ("StructProperty");
            staticClass = name + ("::StaticStruct()");
            break;
        case EPropertyGenFlags::Resource:
            property = ("ResourceProperty");
            break;
        case EPropertyGenFlags::NativeArray:
        case EPropertyGenFlags::Array:
        case EPropertyGenFlags::Set:
            property = ("ArrayProperty");
            break;

        case EPropertyGenFlags::Map:
            property = ("MapProperty");
            break;

        default:
            break;
        }

        uint64_t flags = 0;
        if (arg.getTypePtr()->isPointerType()) {
            flags |= PropertyFlags_Pointer;
        }

        NamedFormatterArgs args;
        args.add(("name"), name);
        args.add(("property"), property);
        args.add(("staticClass"), staticClass);
        args.add(("className"), mTopScope->currentName);
        args.add(("flags"), flags);

        mSourceFormatter.append(
            ("    q_new<{{property}}>({{staticClass}}, (\"{{className}}_{{name}}_Template\"), {{flags}}, 0),"), args);
    }
}
