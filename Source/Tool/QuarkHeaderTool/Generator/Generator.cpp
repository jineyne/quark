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

Generator::Generator(const Path &path) : mPath(path) {
    auto sourcePath = path;
    auto headerPath = path;

    auto fileName = path.getFilename();

    sourcePath.setFilename(fileName + TEXT(".g.cpp"));
    headerPath.setFilename(fileName + TEXT(".g.h"));

    mSourceFormatter = FileSystem::CreateAndOpenFile(sourcePath);
    mHeaderFormatter = FileSystem::CreateAndOpenFile(headerPath);
}

Generator::Generator(const TSharedPtr<Stream> &source, const TSharedPtr<Stream> &header, const Path &path)
    : mHeaderFormatter(header), mSourceFormatter(source), mPath(path) {}

Generator::Generator(const GeneratorDesc &desc)
    : mHeaderFormatter(desc.header), mSourceFormatter(desc.source), mPath(desc.path)
      , mRelativePath(desc.relativePath), mPackage(desc.package) {
    if (desc.source == nullptr || desc.header == nullptr) {
        auto sourcePath = mPath;
        auto headerPath = mPath;

        auto fileName = mPath.getFilename();

        sourcePath.setFilename(fileName + ".g.cpp");
        headerPath.setFilename(fileName + ".g.h");

        mSourceFormatter = FileSystem::CreateAndOpenFile(sourcePath);
        mHeaderFormatter = FileSystem::CreateAndOpenFile(headerPath);
    }
}

bool Generator::generate(Node *node) {
    FStringBuilder sb(1024);
    sb.setDynamic();
    if (!mPackage.empty()) {
        sb << mPackage << TEXT("_");
    }

    sb << TEXT("Source_");
    sb << mPath.getFilename() << TEXT("_h");

    mCurrentFileId = sb.toString();

    NamedFormatterArgs args;
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

Literal Generator::visitNamespace(NamespaceNode *node) {
    // TODO: MAKE SCOPE
    // mFormatter.addIndent();
    mPackage = node->token.token;
    pushScope(node->token.token, FScope::EScopeType::Namespace);

    NamedFormatterArgs args;
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

Literal Generator::visitDirective(DirectiveNode *node) {
    return INodeVisiter::visitDirective(node);
}

Literal Generator::visitVariableDeclare(VariableDeclareNode *node) {
    NamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    mSourceFormatter.appendLine(TEXT("// VARIABLE {name} DECLARE"), args);
    return INodeVisiter::visitVariableDeclare(node);
}

Literal Generator::visitFunctionDeclare(FunctionDeclareNode *node) {
    NamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    if (node->isMacro) {
        mSourceFormatter.appendLine(TEXT("// FUNCTION MACRO {{name}} DECLARE"), args, true);
    } else {
        mSourceFormatter.appendLine(TEXT("// FUNCTION {{name}} DECLARE"), args, true);
    }
    return INodeVisiter::visitFunctionDeclare(node);
}

Literal Generator::visitStructDeclare(StructDeclareNode *node) {
    FunctionDeclareNode *generated = node->generated;

    if (generated != nullptr) {
        printGenerateStructBody(node, generated);
    }

    return INodeVisiter::visitStructDeclare(node);
}

Literal Generator::visitClassDeclare(ClassDeclareNode *node) {
    NamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    FunctionDeclareNode *generated = node->generated;
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

Literal Generator::visitEnumDeclare(EnumDeclareNode *node) {
    NamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    mHeaderFormatter.append(TEXT(R"(
static Enum *{{name}}_StaticEnum();
static Enum* Generated_Initializer_Enum_{{name}}();
)"), args);

    mSourceFormatter.append(TEXT(R"(
Enum *Generated_Initializer_Enum_{{name}}();

static Enum *{{name}}_StaticEnum() {
    static Enum *instance = nullptr;
    if (!instance) {
        instance = getStaticEnum(Generated_Initializer_Enum_{{name}}, "{{name}}");
    }
    return instance;
}

static FInitEnumOnStart Generated_InitEnumOnStart_Enum_{{name}}({{name}}_StaticEnum, "{{name}}", "{{path}}");

static Enum* Generated_Initializer_Enum_{{name}}() {
    static Enum* instance = nullptr;
    if (!instance) {
        static const TVector<Reflection::EnumEntry> entries = {
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

        static const TVector<Reflection::MetaDataPairDesc> metas = {
)"), args);
    mSourceFormatter.addIndent(3);

    for (auto entry : node->meta) {
        NamedFormatterArgs metaArgs;
        metaArgs.add(TEXT("key"), entry.key).add(TEXT("mValue"), entry.value);
        mSourceFormatter.appendLine(TEXT(R"({TEXT("{{key}}"), "{{mValue}}"},)"), metaArgs, true);
    }

    for (auto field : node->fields) {
        for (auto entry : field->meta) {
            NamedFormatterArgs metaArgs;
            metaArgs.add(TEXT("name"), field->token.token).add(TEXT("key"), entry.key).add(TEXT("mValue"), entry.value);
            mSourceFormatter.appendLine(TEXT(R"({TEXT("{{name}}.{{key}}"), "{{mValue}}"},)"), metaArgs, true);
        }
    }

    mSourceFormatter.removeIndent(3);
    mSourceFormatter.append(TEXT(R"(
        };

        static const Reflection::EnumDesc desc = {
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

        Reflection::CreateEnum(instance, desc);
    }
    return instance;
}

)"), args);


    return INodeVisiter::visitEnumDeclare(node);
}

Literal Generator::visitProperty(PropertyNode *node) {
    if (!node->bHasPrefixMacro) {
        // no QPROPERTY macro. skip
        return INodeVisiter::visitProperty(node);
    }

    NamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);
    args.add(TEXT("scopeName"), mTopScope->currentName);
    args.add(TEXT("className"), mTopScope->currentName);
    args.add(TEXT("type"), node->dataType->token.token);

    // fmt::arg("scopeName", mTopScope->currentName), fmt::arg("prefix", prefix))

    bool bIsClass = mTopScope->type == FScope::EScopeType::Class;
    String typeName = bIsClass ? TEXT("Class") : TEXT("Struct");
    args.add(TEXT("typeName"), typeName);

    auto type = Reflection::EPropertyGenFlags::None;
    if (node->bHasPrefixMacro) {
        type = getDataType(node->dataType);
    }

    args.add(TEXT("genFlags"), static_cast<int>(type));

    uint64_t flags = 0;
    TMap<String, String> metas;

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

    // ignore property
    if (!node->bHasPrefixMacro) {
        flags |= PropertyFlags_NonSerialized;
    }

    args.add(TEXT("flags"), flags);

    switch (type) {
    case Reflection::EPropertyGenFlags::Object:
        mSourceFormatter.append(TEXT(R"(
const Reflection::ObjectPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
        break;

    case Reflection::EPropertyGenFlags::Struct:
        mSourceFormatter.append(TEXT(R"(
const Reflection::StructPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    {{name}}::StaticStruct,
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
        break;

    case Reflection::EPropertyGenFlags::Class:
        mSourceFormatter.append(TEXT(R"(
const Reflection::StructPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    {{className}}::StaticClass,
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
        break;

    case Reflection::EPropertyGenFlags::Array:
        mSourceFormatter.append(TEXT(R"(
const Reflection::ArrayPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
)"), args);
        renderPropertyType(static_cast<TemplateTypeNode*>(node->dataType)->arguments[0]);
        mSourceFormatter.append(TEXT(R"(
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
        break;

    case Reflection::EPropertyGenFlags::Map:
        mSourceFormatter.append(TEXT(R"(
const Reflection::ArrayPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
};
)"), args);
        break;

    case Reflection::EPropertyGenFlags::Set:
        mSourceFormatter.append(TEXT(R"(
const Reflection::ArrayPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
};
)"), args);
        break;

    default:
        mSourceFormatter.append(TEXT(R"(
const Reflection::GenericPropertyDesc Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_PropertyDesc = {
    TEXT("{{name}}"),
    (EPropertyFlags) {{flags}},
    (Reflection::EPropertyGenFlags) {{genFlags}},
    sizeof({{className}}::{{name}}),
    1,
    offsetof({{className}}, {{name}}),
    Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData,
};
)"), args);
        break;
    }

    mSourceFormatter.append(TEXT(R"(
const TArray<Reflection::MetaDataPairDesc> Generated_{{typeName}}_{{scopeName}}_Statics::{{name}}_MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    for (auto entry : node->meta) {
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

    return INodeVisiter::visitProperty(node);
}

Literal Generator::visitEnumField(EnumFieldNode *node) {
    NamedFormatterArgs args;
    DEFINE_DEFAULT_ARGS(args);

    args.add(TEXT("type"), mTopScope->currentName);
    mSourceFormatter.appendLine(TEXT(R"({TEXT("{{type}}::{{name}}"), (int64_t) {{type}}::{{name}}},)"), args, true);

    for (auto entry : node->meta) {
        FStringBuilder ss(512);
        ss << node->token.token << TEXT("/") << entry.key;
        mMetaData[ss.toString()] = entry.value;
    }

    return INodeVisiter::visitEnumField(node);
}

Literal Generator::visitLiteral(LiteralNode *node) {
    return INodeVisiter::visitLiteral(node);
}

void Generator::pushScope(const String &name, FScope::EScopeType type) {
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

void Generator::popScope() {
    auto temp = mTopScope;
    mTopScope = temp->parent;

    LOG(LogQHT, Debug, TEXT("Scope popped: %ls"), *temp->fullName);
    delete temp;
}

void Generator::printGenerateBody(ClassDeclareNode *node, FunctionDeclareNode *generated) {
    NamedFormatterArgs args;
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
Class *Generated_Initializer_Class_{{name}}();
static InitClassOnStart Generated_InitClassOnStart_Class_{{name}}(&Generated_Initializer_Class_{{name}}, &{{name}}::StaticClass, TEXT("{{name}}"), TEXT("{{relativePath}}"));

)"), args);

    printStatics(node, generated, FScope::EScopeType::Class);

    mSourceFormatter.append(TEXT(R"(
Class *Generated_Initializer_Class_{{name}}() {
    static Class *instance = nullptr;
    if (!instance) {
        Reflection::CreateClass(instance, Generated_Class_{{name}}_Statics::ClassDesc);
    }
    return instance;
}
)"), args);
}

void Generator::printGenerateStructBody(StructDeclareNode *node, FunctionDeclareNode *generated) {
    NamedFormatterArgs args;
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
    printStatics(node, generated, FScope::EScopeType::Struct);

    mSourceFormatter.append(TEXT(R"(
Struct *Generated_Initializer_Struct_{{name}}() {
    static Struct *instance = nullptr;
    if (!instance) {
        Reflection::CreateStruct(instance, Generated_Struct_{{name}}_Statics::StructDesc);
    }
    return instance;
}
)"), args);
}

void Generator::printStatics(DeclareNode *node, FunctionDeclareNode *generated, FScope::EScopeType scope) {
    NamedFormatterArgs args;
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

        auto property = static_cast<PropertyNode*>(field);

        auto type = Reflection::EPropertyGenFlags::None;
        if (field->bHasPrefixMacro) {
            type = getDataType(property->dataType);
        }

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), field->token.token);

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
                TEXT("static const Reflection::ArrayPropertyDesc {{name}}_PropertyDesc;"), fieldArgs, true);
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

        mSourceFormatter.appendLine(
            TEXT("static const TArray<Reflection::MetaDataPairDesc> {{name}}_MetaData;"), fieldArgs, true);
    }

    mSourceFormatter.append(TEXT(R"(
    static const TArray<Reflection::MetaDataPairDesc> {{type}}MetaData;
    static const TArray<Reflection::PropertyDescBase const*> {{type}}Properties;
    static const Reflection::F{{type}}Desc {{type}}Desc;
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
const TArray<Reflection::MetaDataPairDesc> Generated_{{type}}_{{name}}_Statics::{{type}}MetaData = {
)"), args);
    mSourceFormatter.addIndent();

    uint64_t flags = 0;
    for (auto entry : node->meta) {
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

    for (auto field : node->fields) {
        if (!field->bHasPrefixMacro) {
            // no QPROPERTY macro. skip
            continue;
        }

        NamedFormatterArgs fieldArgs;
        fieldArgs.add(TEXT("name"), field->token.token)
                 .add(TEXT("typeName"), node->token.token)
                 .add(TEXT("type"), scope == FScope::EScopeType::Class ? TEXT("Class") : TEXT("Struct"));

        mSourceFormatter.appendLine(
            TEXT(
                "(const Reflection::PropertyDescBase *) &Generated_{{type}}_{{typeName}}_Statics::{{name}}_PropertyDesc,"),
            fieldArgs, true);
    }

    mSourceFormatter.removeIndent();
    mSourceFormatter.appendLine("};");

    args.add(TEXT("flags"), 0);

    mSourceFormatter.append(TEXT(R"(
const Reflection::F{{type}}Desc Generated_{{type}}_{{name}}_Statics::{{type}}Desc = {
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

Reflection::EPropertyGenFlags Generator::getDataType(TypeNode *node) {
    const auto name = node->token.token;

    if (node->type == ENodeType::TemplateType) {
        auto templateNode = static_cast<TemplateTypeNode*>(node);
        if (name == TEXT("TArray")) {
            return Reflection::EPropertyGenFlags::Array;
        }
        if (name == TEXT("TMap") || name == TEXT("TUnorderedMap")) {
            return Reflection::EPropertyGenFlags::Map;
        }
        if (name == TEXT("TSet") || name == TEXT("TUnorderedSet")) {
            return Reflection::EPropertyGenFlags::Set;
        }
        // check is array type
    } else if (node->type == ENodeType::PointerType) {
        return Reflection::EPropertyGenFlags::Object;
    } else {
        if (name == TEXT("int8_t") || (name == TEXT("char") && !node->bUnSigned)) {
            return Reflection::EPropertyGenFlags::Int8; // maybe byte?
        }
        if (name == TEXT("uint8_t") || (name == TEXT("char") && node->bUnSigned)) {
            return Reflection::EPropertyGenFlags::UInt8;
        }
        if (name == TEXT("int32_t") || (name == TEXT("int") && !node->bUnSigned)) {
            return Reflection::EPropertyGenFlags::Int32;
        }
        if (name == TEXT("uint32_t") || (name == TEXT("int") && node->bUnSigned)) {
            return Reflection::EPropertyGenFlags::UInt32;
        }
        if (name == TEXT("int64_t")) {
            return Reflection::EPropertyGenFlags::Int64;
        }
        if (name == TEXT("uint64_t")) {
            return Reflection::EPropertyGenFlags::UInt64;
        }
        if (name == TEXT("float")) {
            return Reflection::EPropertyGenFlags::Float;
        }
        if (name == TEXT("double")) {
            return Reflection::EPropertyGenFlags::Double;
        }
        if (name == TEXT("String")) {
            return Reflection::EPropertyGenFlags::String;
        }

        if (name.startWith(TEXT("H"))) {
            return Reflection::EPropertyGenFlags::Object;
        }

        if (name.startWith("F")) {
            return Reflection::EPropertyGenFlags::Struct;
        }

        // TODO:
        if (node->type == ENodeType::ClassDeclare) {
            return Reflection::EPropertyGenFlags::Class;
        }
    }

    LOG(LogQHT, Error, TEXT("Unknown data type: %ls"), *name);
    return Reflection::EPropertyGenFlags::None;
}

void Generator::renderPropertyType(TypeNode *node) {
    String name = node->token.token;

    String property = TEXT("IntProperty");
    String staticClass = TEXT("nullptr");

    if (node->type == ENodeType::TemplateType) {
        property = TEXT("ArrayProperty");
    } else if (node->type == ENodeType::PointerType) {
        name = static_cast<PointerType*>(node)->base->token.token;
        property = TEXT("ClassProperty");
        staticClass = name + TEXT("::StaticClass()");
    } else {
        if (name == TEXT("int8_t") || name == TEXT("int8_t") || (name == TEXT("char") && !node->bUnSigned)) {
            property = TEXT("Int8Property");
        } else if (name == TEXT("int32_t") || name == TEXT("uint32_t") || (name == TEXT("int") && !node->bUnSigned)) {
            property = TEXT("IntProperty");
        } else if (name == TEXT("int64_t") || name == TEXT("uint64_t")) {
            property = TEXT("Int64Property");
        } else if (name == TEXT("float")) {
            property = TEXT("FloatProperty");
        } else if (name == TEXT("double")) {
            property = TEXT("DoubleProperty");
        } else if (name == TEXT("String")) {
            property = TEXT("StringProperty");
        } else if (name.startWith(TEXT("H"))) {
            property = TEXT("ObjectProperty");
            staticClass = name + TEXT("::StaticClass()");
        } else if (name.startWith("F")) {
            property = TEXT("StructProperty");
            staticClass = name + TEXT("::StaticStruct()");
        } else {
            LOG(LogQHT, Error, TEXT("Unknown data type: %ls"), *name);
        }
    }

    NamedFormatterArgs args;
    args.add(TEXT("name"), name);
    args.add(TEXT("property"), property);
    args.add(TEXT("staticClass"), staticClass);
    args.add(TEXT("className"), mTopScope->currentName);

    mSourceFormatter.append(
        TEXT("    q_new<{{property}}>({{staticClass}}, TEXT(\"{{className}}_{{name}}_Template\"), 0),"), args);
}
