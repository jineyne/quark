#include "Generator.h"
#include "Misc/StringBuilder.h"

#include <FileSystem/FileStream.h>
#include <FileSystem/FileSystem.h>

#define DEFINE_DEFAULT_ARGS(TARGET)                                                                                 \
    (TARGET).add(TEXT("name"), node->token.token)                                                                   \
            .add(TEXT("package"), mPackage)                                                                         \
            .add(TEXT("path"), mPath.toString())                                                                    \
            .add(TEXT("relativePath"), mRelativePath.toString())                                                    \
            .add(TEXT("filename"), mPath.getFilename())                                                             \
            .add(TEXT("lineNo"), node->token.line + 1)                                                              \
            .add(TEXT("currentFileId"), mCurrentFileId)

#define DEFINE_DEFAULT_ARGS2(TARGET, NODE)                                                                          \
    (TARGET).add(TEXT("name"), (NODE)->token.token)                                                                 \
            .add(TEXT("package"), mPackage)                                                                         \
            .add(TEXT("path"), mPath.toString())                                                                    \
            .add(TEXT("relativePath"), mRelativePath.toString())                                                    \
            .add(TEXT("filename"), mPath.getFilename())                                                             \
            .add(TEXT("lineNo"), (NODE)->token.line + 1)                                                            \
            .add(TEXT("currentFileId"), mCurrentFileId)

FGenerator::FGenerator(const FPath &path) : mPath(path) {
    auto sourcePath = path;
    auto headerPath = path;

    auto fileName = path.getFilename();

    sourcePath.setFilename(fileName + TEXT(".g.cpp"));
    headerPath.setFilename(fileName + TEXT(".g.h"));

    mSourceFormatter = FFileSystem::CreateAndOpenFile(sourcePath);
    mHeaderFormatter = FFileSystem::CreateAndOpenFile(headerPath);
}

FGenerator::FGenerator(const TSharedPtr<FStream> &source, const TSharedPtr<FStream> &header, const FPath &path)
        : mSourceFormatter(source), mHeaderFormatter(header), mPath(path) {}

FGenerator::FGenerator(const FGeneratorDesc &desc)
        : mPath(desc.path), mRelativePath(desc.relativePath), mPackage(desc.package)
        , mSourceFormatter(desc.source) , mHeaderFormatter(desc.header) {
    if (desc.source == nullptr || desc.header == nullptr) {
        auto sourcePath = mPath;
        auto headerPath = mPath;

        auto fileName = mPath.getFilename();

        sourcePath.setFilename(fileName + ".g.cpp");
        headerPath.setFilename(fileName + ".g.h");

        mSourceFormatter = FFileSystem::CreateAndOpenFile(sourcePath);
        mHeaderFormatter = FFileSystem::CreateAndOpenFile(headerPath);
    }
}

bool FGenerator::generate(FNode *node) {
    FStringBuilder sb(512);
    if (!mPackage.empty()) {
        sb << mPackage << TEXT("_");
    }

    sb << TEXT("Source_");
    sb << mPath.getFilename() << TEXT("_h");

    mCurrentFileId = sb.toString();

    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    mHeaderFormatter.append(R"(
// DO NOT MODIFY MANUALLY!

#ifdef {{package}}_{{filename}}_g_h
#error "{{filename}}.g.h already included. missing '#pragma once' in {{filename}}.h"
#endif

#define {{package}}_{{filename}}_g_h

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID {{currentFileId}}

#include "{{relativePath}}"
#include "Reflection/GeneratedPrerequisites.h"

)", args);

    mSourceFormatter.append(TEXT(R"(
#include "{{relativePath}}"
#include "Reflection/GeneratedPrerequisites.h"

)"), args);

    auto result = visit(node);

    // TODO: ERROR?
    return true;
}

FLiteral FGenerator::visitNamespace(FNamespaceNode *node) {
    // TODO: MAKE SCOPE
    // mFormatter.addIndent();
    mPackage = node->token.token;
    pushScope(node->token.token, FScope::EScopeType::Namespace);

    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    mSourceFormatter.appendLine(TEXT("namespace {{name}} {{"), args);
    mSourceFormatter.addIndent();

    auto result = INodeVisiter::visitNamespace(node);

    // mFormatter.removeIndent();
    mPackage = "";
    popScope();

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(TEXT("}"));

    return result;
}

FLiteral FGenerator::visitDirective(FDirectiveNode *node) {
    return INodeVisiter::visitDirective(node);
}

FLiteral FGenerator::visitVariableDeclare(FVariableDeclareNode *node) {
    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    mSourceFormatter.appendLine(TEXT("// VARIABLE {name} DECLARE"), args);
    return INodeVisiter::visitVariableDeclare(node);
}

FLiteral FGenerator::visitFunctionDeclare(FFunctionDeclareNode *node) {
    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    if (node->isMacro) {
        mSourceFormatter.appendLine(TEXT("// FUNCTION MACRO {{name}} DECLARE"), args, true);
    } else {
        mSourceFormatter.appendLine(TEXT("// FUNCTION {{name}} DECLARE"), args, true);
    }
    return INodeVisiter::visitFunctionDeclare(node);
}

FLiteral FGenerator::visitStructDeclare(FStructDeclareNode *node) {
    FFunctionDeclareNode *generated = node->generated;

    if (generated != nullptr) {
        printGenerateStructBody(node, generated);
    }

    return INodeVisiter::visitStructDeclare(node);
}

FLiteral FGenerator::visitClassDeclare(FClassDeclareNode *node) {
    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    FFunctionDeclareNode *generated = node->generated;
    pushScope(node->token.token, FScope::EScopeType::Class);

    if (generated != nullptr) {
        printGenerateBody(node, generated);
    }

    popScope();

    mSourceFormatter.append(TEXT(R"(

// CLASS {{name}} DECLARE END

)"), args);

    return INodeVisiter::visitClassDeclare(node);
}

FLiteral FGenerator::visitEnumDeclare(FEnumDeclareNode *node) {
    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    mHeaderFormatter.append(TEXT(R"(
static QEnum *{{name}}_StaticEnum();
static QEnum* Generated_Initializer_Enum_{{name}}();
)"), args);

    mSourceFormatter.append(TEXT(R"(
QEnum *Generated_Initializer_Enum_{{name}}();

static QEnum *{{name}}_StaticEnum() {
    static QEnum *instance = nullptr;
    if (!instance) {
        instance = getStaticEnum(Generated_Initializer_Enum_{{name}}, "{{name}}");
    }
    return instance;
}

static FInitEnumOnStart Generated_InitEnumOnStart_Enum_{{name}}({{name}}_StaticEnum, "{{name}}", "{{path}}");

static QEnum* Generated_Initializer_Enum_{{name}}() {
    static QEnum* instance = nullptr;
    if (!instance) {
        static const TVector<QReflection::FEnumEntry> entries = {
)"), args);
    mSourceFormatter.addIndent(3);

    pushScope(node->token.token, FScope::EScopeType::Enum);

    for (auto field : node->fields) {
        visit(field);
    }

    popScope();

    mSourceFormatter.removeIndent(3);

    mSourceFormatter.append(TEXT(R"(
        };

        static const TVector<QReflection::FMetaDataPairDesc> metas = {
)"), args);
    mSourceFormatter.addIndent(3);

    for (auto entry : node->meta) {
        FNamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.first).add(TEXT("value"), entry.second);
        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), "{{value}}"},)"), metaArgs, true);
    }

    for (auto field : node->fields) {
        for (auto entry : field->meta) {
            FNamedFormatterArgs metaArgs;
            metaArgs.add(TEXT("name"), field->token.token).add(TEXT("key"), entry.first).add(TEXT("value"), entry.second);
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{name}}.{{key}}"), "{{value}}"},)"), metaArgs, true);
        }
    }

    mSourceFormatter.removeIndent(3);
    mSourceFormatter.append(TEXT(R"(
        };

        static const QReflection::FEnumDesc desc = {
            TEXT("{{name}}")
)"), args);
    mSourceFormatter.addIndent(3);
    if (mTopScope == nullptr) {
        mSourceFormatter.appendLine(TEXT(R"(TEXT("{{name}}"),)"), args, true);
    } else {
        args.add(TEXT("scopeName"), mTopScope == nullptr ? TEXT("") : mTopScope->fullName);
        mSourceFormatter.appendLine(TEXT("\"{scopeName}::{name}\","), args, true);
    }

    mSourceFormatter.removeIndent(3);
    mSourceFormatter.append(TEXT(R"(
            entries,
            metas,
        };

        QReflection::CreateEnum(instance, desc);
    }
    return instance;
}

)"), args);


    return INodeVisiter::visitEnumDeclare(node);
}

FLiteral FGenerator::visitProperty(FPropertyNode *node) {
    if (!node->bHasPrefixMacro) {
        // no QPROPERTY macro. skip
        return INodeVisiter::visitProperty(node);
    }

    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);
    args.add(TEXT("scopeName"), mTopScope->currentName);
    args.add(TEXT("className"), mTopScope->currentName);
    args.add(TEXT("type"), node->dataType->token.token);

    // fmt::arg("scopeName", mTopScope->currentName), fmt::arg("prefix", prefix))

    bool bIsClass = mTopScope->type == FScope::EScopeType::Class;
    FString typeName = bIsClass ? TEXT("Class") : TEXT("Struct");
    args.add(TEXT("typeName"), typeName);

    QReflection::EPropertyGenFlags type = QReflection::EPropertyGenFlags::None;
    if (node->bHasPrefixMacro) {
        type = getDataType(node->dataType);
    }

    args.add(TEXT("genFlags"), (int) type);

    uint64_t flags = 0;
    TMap<FString, FString> metas;

    switch (node->access) {
        case EAccessControlType::Public:
            flags |= PropertyFlags_Public;
            break;

        case EAccessControlType::Protected:
            flags |= PropertyFlags_Protected;
            break;

        case EAccessControlType::Private:
            flags |= PropertyFlags_Private;
            break;

        default:
            break;
    }

    for (auto entry : node->meta) {
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

    // ignore property
    if (!node->bHasPrefixMacro) {
        flags |= PropertyFlags_NonSerialized;
    }

    args.add(TEXT("flags"), flags);

    switch (type) {
        case QReflection::EPropertyGenFlags::Object:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FObjectPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case QReflection::EPropertyGenFlags::Struct:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FStructPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    {{name}}::StaticStruct,
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case QReflection::EPropertyGenFlags::Class:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FStructPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    {{className}}::StaticClass,
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case QReflection::EPropertyGenFlags::Array:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FArrayPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
)"), args);
            renderPropertyType(((FTemplateTypeNode *) node->dataType)->arguments[0]);
            mSourceFormatter.append(TEXT(R"(
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;

        case QReflection::EPropertyGenFlags::Map:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FArrayPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
};
)"), args);
            break;

        case QReflection::EPropertyGenFlags::Set:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FArrayPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
};
)"), args);
            break;

        default:
            mSourceFormatter.append(TEXT(R"(
const QReflection::FGenericPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (QReflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
            break;
    }

    mSourceFormatter.append(TEXT(R"(
const TArray<QReflection::FMetaDataPairDesc> Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    for (auto entry : node->meta) {
        FNamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.first).add(TEXT("value"), entry.second);

        /*if (!entry.second.empty()) {
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{value}}")},)"), metaArgs, true);
            continue;
        }

        if (entry.first.contains(TEXT("."))) {
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{value}}")},)"), metaArgs, true);
            continue;
        }*/

        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{value}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(TEXT("};"), args);

    return INodeVisiter::visitProperty(node);
}

FLiteral FGenerator::visitEnumField(FEnumFieldNode *node) {
    FNamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    args.add(TEXT("type"), mTopScope->currentName);
    mSourceFormatter.appendLine(TEXT(R"({TEXT("{{type}}::{{name}}"), (int64_t) {{type}}::{{name}}},)"), args, true);

    for (auto entry : node->meta) {
        FStringBuilder ss(512);
        ss << node->token.token << TEXT("/") << entry.first;
        mMetaData[ss.toString()] = entry.second;
    }

    return INodeVisiter::visitEnumField(node);
}

FLiteral FGenerator::visitLiteral(FLiteralNode *node) {
    return INodeVisiter::visitLiteral(node);
}

void FGenerator::pushScope(const FString &name, FGenerator::FScope::EScopeType type) {
    auto scope = new FScope();
    scope->currentName = name;
    scope->type = type;
    if (mTopScope == nullptr) {
        mTopScope = scope;
        mTopScope->parent = nullptr;
        mTopScope->fullName = name;
    } else {
        scope->parent = mTopScope;
        scope->fullName = mTopScope->fullName + "::" + name;
        scope->parent = mTopScope;
        mTopScope = scope;
    }

    LOG(LogQHT, Debug, TEXT("Scope pushed: %ls"), *mTopScope->fullName);
}

void FGenerator::popScope() {
    auto temp = mTopScope;
    mTopScope = temp->parent;

    LOG(LogQHT, Debug, TEXT("Scope popped: %ls"), *temp->fullName);
    delete temp;
}

void FGenerator::printGenerateBody(FClassDeclareNode *node, FFunctionDeclareNode *generated) {
    FNamedFormatterArgs args;
    args.add(TEXT("name"), node->token.token)
        .add(TEXT("package"), mPackage)
        .add(TEXT("path"), mPath.toString())
        .add(TEXT("relativePath"), mRelativePath.toString())
        .add(TEXT("filename"), mPath.getFilename())
        .add(TEXT("lineNo"), generated->token.line + 1)
        .add(TEXT("currentFileId"), mCurrentFileId)
        .add(TEXT("base"), node->base->token.token);

    mHeaderFormatter.append(TEXT(R"(
#define {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS \
private: \
    friend struct Generated_Class_{{name}}_Statics; \
    static void StaticRegisterNative{{name}}(); \
public: \
    DECLARE_CLASS({{name}}, {{base}}, ) \
    DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL({{name}}) \
    DECLARE_SERIALIZER({{name}})

#define {{currentFileId}}_{{lineNo}}_GENERATED_BODY \
        {{currentFileId}}_{{lineNo}}_GENERATED_FUNCTIONS
)"), args);

    mSourceFormatter.appendLine(TEXT("void {{name}}::StaticRegisterNative{{name}}() {"), args);
    mSourceFormatter.addIndent();

    for (auto function : node->functions) {
        visit(function);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(TEXT("}"));

    mSourceFormatter.append(TEXT(R"(
IMPLEMENT_CLASS({{name}});
QClass *Generated_Initializer_Class_{{name}}();
static FInitClassOnStart Generated_InitClassOnStart_Class_{{name}}(&Generated_Initializer_Class_{{name}}, &{{name}}::StaticClass, TEXT("{{name}}"), TEXT("{{relativePath}}"));

)"), args);

    printStatics(node, generated, FScope::EScopeType::Class);

    mSourceFormatter.append(TEXT(R"(
QClass *Generated_Initializer_Class_{{name}}() {
    static QClass *instance = nullptr;
    if (!instance) {
        QReflection::CreateClass(instance, Generated_Class_{{name}}_Statics::ClassDesc);
    }
    return instance;
}
)"), args);
}

void FGenerator::printGenerateStructBody(FStructDeclareNode *node, FFunctionDeclareNode *generated) {
    FNamedFormatterArgs args;
    args.add(TEXT("name"), node->token.token)
        .add(TEXT("package"), mPackage)
        .add(TEXT("path"), mPath.toString())
        .add(TEXT("relativePath"), mRelativePath.toString())
        .add(TEXT("filename"), mPath.getFilename())
        .add(TEXT("lineNo"), generated->token.line + 1)
        .add(TEXT("currentFileId"), mCurrentFileId);

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
    printStatics(node, generated, FScope::EScopeType::Struct);

    mSourceFormatter.append(TEXT(R"(
QStruct *Generated_Initializer_Struct_{{name}}() {
    static QStruct *instance = nullptr;
    if (!instance) {
        QReflection::CreateStruct(instance, Generated_Struct_{{name}}_Statics::StructDesc);
    }
    return instance;
}
)"), args);
}

void FGenerator::printStatics(FDeclareNode *node, FFunctionDeclareNode *generated, FGenerator::FScope::EScopeType scope) {
    FNamedFormatterArgs args;
    args.add(TEXT("name"), node->token.token)
        .add(TEXT("package"), mPackage)
        .add(TEXT("path"), mPath.toString())
        .add(TEXT("relativePath"), mRelativePath.toString())
        .add(TEXT("filename"), mPath.getFilename())
        .add(TEXT("lineNo"), generated->token.line + 1)
        .add(TEXT("currentFileId"), mCurrentFileId)
        .add(TEXT("type"), scope == FScope::EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

    mSourceFormatter.appendLine(TEXT("struct Generated_{{type}}_{{name}}_Statics {"), args);
    mSourceFormatter.addIndent();

    for (auto field : node->fields) {
        if (field->type != ENodeType::Property) {
            continue;
        }

        if (!field->bHasPrefixMacro) {
            // no QPROPERTY macro. skip
            continue;
        }

        auto property = (FPropertyNode *) field;

        QReflection::EPropertyGenFlags type = QReflection::EPropertyGenFlags::None;
        if (field->bHasPrefixMacro) {
            type = getDataType(property->dataType);
        }

        FNamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), field->token.token);

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

        mSourceFormatter.appendLine(TEXT("static const TArray<QReflection::FMetaDataPairDesc> {{name}}_MetaData;"), fieldArgs, true);
    }

    mSourceFormatter.append(TEXT(R"(
    static const TArray<QReflection::FMetaDataPairDesc> {{type}}MetaData;
    static const TArray<QReflection::FPropertyDescBase const*> {{type}}Properties;
    static const QReflection::F{{type}}Desc {{type}}Desc;
};
)"), args);

    mSourceFormatter.removeIndent();

    pushScope(node->token.token, scope);

    for (auto field : node->fields) {
        if (field->type != ENodeType::Property) {
            continue;
        }

        visit(field);
    }

    popScope();

    mSourceFormatter.append(TEXT(R"(
const TArray<QReflection::FMetaDataPairDesc> Generated_{{type}}_{{name}}_Statics::{{type}}MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    uint64_t flags = 0;
    for (auto entry : node->meta) {
        FNamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.first).add(TEXT("value"), entry.second);

        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), TEXT("{{value}}")},)"), metaArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine(R"(};)");

    mSourceFormatter.appendLine(TEXT("const TArray<QReflection::FPropertyDescBase const*> Generated_{{type}}_{{name}}_Statics::{{type}}Properties = {"), args, true);
    mSourceFormatter.addIndent();

    for (auto field : node->fields) {
        if (!field->bHasPrefixMacro) {
            // no QPROPERTY macro. skip
            continue;
        }

        FNamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), field->token.token)
                 .add(TEXT("typeName"), node->token.token)
                 .add(TEXT("type"), scope == FScope::EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

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

    if (scope == FScope::EScopeType::Struct) {
        mSourceFormatter.append(TEXT(R"(    sizeof({{name}}),)"), args);
    }

    mSourceFormatter.append(TEXT(R"(
    Generated_{{type}}_{{name}}_Statics::{{type}}Properties,
    Generated_{{type}}_{{name}}_Statics::{{type}}MetaData,
};
)"), args);
}

QReflection::EPropertyGenFlags FGenerator::getDataType(FTypeNode *node) {
    const auto name = node->token.token;

    if (node->type == ENodeType::TemplateType) {
        auto templateNode = (FTemplateTypeNode *) node;
        if (name == TEXT("TArray")) {
            return QReflection::EPropertyGenFlags::Array;
        } else if (name == TEXT("TMap") || name == TEXT("TUnorderedMap")) {
            return QReflection::EPropertyGenFlags::Map;
        } else if (name == TEXT("TSet") || name == TEXT("TUnorderedSet")) {
            return QReflection::EPropertyGenFlags::Set;
        }
        // check is array type
    } else if (node->type == ENodeType::PointerType) {
        return QReflection::EPropertyGenFlags::Object;
    } else {
        if (name == TEXT("int8_t") || (name == TEXT("char") && !node->bUnSigned)) {
            return QReflection::EPropertyGenFlags::Int8; // maybe byte?
        } else if (name == TEXT("uint8_t") || (name == TEXT("char") && node->bUnSigned)) {
            return QReflection::EPropertyGenFlags::UInt8;
        } else if (name == TEXT("int32_t") || (name == TEXT("int") && !node->bUnSigned)) {
            return QReflection::EPropertyGenFlags::Int32;
        } else if (name == TEXT("uint32_t") || (name == TEXT("int") && node->bUnSigned)) {
            return QReflection::EPropertyGenFlags::UInt32;
        } else if (name == TEXT("int64_t")) {
            return QReflection::EPropertyGenFlags::Int64;
        } else if (name == TEXT("uint64_t")) {
            return QReflection::EPropertyGenFlags::UInt64;
        } else if (name == TEXT("float")) {
            return QReflection::EPropertyGenFlags::Float;
        } else if (name == TEXT("double")) {
            return QReflection::EPropertyGenFlags::Double;
        } else if (name == TEXT("FString")) {
            return QReflection::EPropertyGenFlags::String;
        }

        if (name.startWith(TEXT("H"))) {
            return QReflection::EPropertyGenFlags::Object;
        }

        if (name.startWith("F")) {
            return QReflection::EPropertyGenFlags::Struct;
        }

        // TODO:
        if (node->type == ENodeType::ClassDeclare) {
            return QReflection::EPropertyGenFlags::Class;
        }

    }

    LOG(LogQHT, Error, TEXT("Unknown data type: %ls"), *name);
    return QReflection::EPropertyGenFlags::None;
}

void FGenerator::renderPropertyType(FTypeNode *node) {
    FString name = node->token.token;

    FString property = TEXT("QIntProperty");
    FString staticClass = TEXT("nullptr");

    if (node->type == ENodeType::TemplateType) {
        property = TEXT("QArrayProperty");
    } else if (node->type == ENodeType::PointerType) {
        name = ((FPointerType *) node)->base->token.token;
        property = TEXT("QClassProperty");
        staticClass = name + TEXT("::StaticClass()");
    } else {
        if (name == TEXT("int8_t") || name == TEXT("int8_t") || (name == TEXT("char") && !node->bUnSigned) ) {
            property = TEXT("QInt8Property");
        } else if (name == TEXT("int32_t") || name == TEXT("uint32_t") || (name == TEXT("int") && !node->bUnSigned)) {
            property = TEXT("QIntProperty");
        } else if (name == TEXT("int64_t") || name == TEXT("uint64_t")) {
            property = TEXT("QInt64Property");
        } else if (name == TEXT("float")) {
            property = TEXT("QFloatProperty");
        } else if (name == TEXT("double")) {
            property = TEXT("QDoubleProperty");
        } else if (name == TEXT("FString")) {
            property = TEXT("QStringProperty");
        } else if (name.startWith(TEXT("H"))) {
            property = TEXT("QObjectProperty");
            staticClass = name + TEXT("::StaticClass()");
        } else if (name.startWith("F")) {
            property = TEXT("QStructProperty");
            staticClass = name + TEXT("::StaticStruct()");
        } else {
            LOG(LogQHT, Error, TEXT("Unknown data type: %ls"), *name);
        }
    }

    FNamedFormatterArgs args;
    args.add(TEXT("name"), name);
    args.add(TEXT("property"), property);
    args.add(TEXT("staticClass"), staticClass);
    args.add(TEXT("className"), mTopScope->currentName);

    mSourceFormatter.append(TEXT("    new {{property}}({{staticClass}}, TEXT(\"{{className}}_{{name}}_Template\"), 0),"), args);
}
