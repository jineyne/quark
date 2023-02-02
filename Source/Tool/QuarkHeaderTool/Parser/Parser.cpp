#include "Parser.h"
#include "Misc/StringBuilder.h"

FParser::FParser(FOptions options) : mOptions(options) {}

FCompoundNode *FParser::parse(const FString &input) {
    reset(input);

    mTopScope = &mScopeList[mScopeIndex];
    mTopScope->name = "Global";
    mTopScope->type = EScopeType::Global;
    mTopScope->currentAccessControlType = EAccessControlType::Public;

    FCompoundNode *compound = new FCompoundNode();

    auto node = parseStatement();
    compound->statements.add(node);

    while ((node = parseStatement()) != nullptr) {
        compound->statements.add(node);
    }

    if (bHasError) {
        // do stuff
        delete compound;
    }

    return compound;
}

FStatementNode *FParser::parseStatement() {
    FToken token;
    if (!getNextToken(token)) {
        return nullptr;
    }

    return parseDeclaration(token);
}

FStatementNode *FParser::parseDeclaration(FToken &token) {
    if (hasError()) {
        return nullptr;
    }

    TArray<FString> customMacroIt;
    if (token.token == "#") {
        return parseDirective();
    } else if (token.token == mOptions.enumNameMacro) {
        undo(token);
        return parseEnum(token);
    } else if (token.token == mOptions.structNameMacro) {
        undo(token);
        return parseStruct(token);
    } else if (token.token == mOptions.classNameMacro) {
        undo(token);
        return parseClass(token);
    } else if (token.token == mOptions.propertyNameMacro) {
        undo(token);
        return parseProperty(token);
    } else if (token.token == mOptions.functionNameMacro) {
        undo(token);
        return parseFunctionDeclare(token, true, false);
    } else if (token.token == "template") {
        undo(token);
        return parseFunctionDeclare(token, true, false);
    } else if (token.token == "namespace") {
        return parseNamespace();
    } else if (mOptions.customFunctionMacros.contains(token.token)) {
        undo(token);
        return parseFunctionDeclare(token, false, true);
    } else if (mOptions.customMacros.contains(token.token)) {
        undo(token);
        return parseCustomMacro(token);
    } else if (token.token == ";") {
        return nullptr;
    } else if (token.token == "class") {
        undo(token);

        auto type = parseType();
        delete type;

        FToken temp;
        if (!getIdentifier(temp)) {
            return nullptr;
        }

        if (!getNextToken(temp)) {
            return nullptr;
        }

        if (temp.type != ETokenType::Symbol) {
            return error("Invalid identifier");
        }

        undo(token);
        if (temp.token == TEXT("=") || temp.token == TEXT(";")) {
            return parseProperty(token);
        } else if (temp.token == TEXT(":") || temp.token == TEXT("{")) {
            return parseClass(token);
        } else if (temp.token == TEXT("(")) {
            return parseFunctionDeclare(token, true, false);
        } else {
            return error("Invalid syntax");
        }
    } else {
        undo(token);

        auto type = parseType();
        delete type;

        FToken temp;
        if (!getNextToken(temp)) {
            return nullptr;
        }

        if (temp.type == ETokenType::Symbol) {
            if (temp.token == "(") {
                // constructor!
                undo(token);
                return parseFunctionDeclare(token, false, false);
            } else {
                undo(token);
                return parseProperty(token);
            }

            return nullptr;
        } else if (temp.type == ETokenType::Identifier) {
            while (getNextToken(temp)) {
                // SKIP CUSTOM MACROS
                if ((std::find(mOptions.customMacros.begin(), mOptions.customMacros.end(), temp.token) != mOptions.customMacros.end())) {
                    continue;
                }

                if (temp.type != ETokenType::Identifier) {
                    if (temp.token == "(") {
                        undo(token);
                        return parseFunctionDeclare(token, true, false);
                    } else if (temp.token == "=" || temp.token == ";") {
                        undo(token);
                        return parseProperty(token);
                    }
                }

                return nullptr;
            }

            return nullptr;
        }
    }

    // undo(token);
    return error("Invalid identifier");
}

FNamespaceNode *FParser::parseNamespace() {
    FToken token;
    if (!getIdentifier(token)) {
        error("missing namespace name");
        return nullptr;
    }

    auto node = new FNamespaceNode();
    node->token = token;

    if (!requireSymbol(TEXT("{"))) {
        delete node;
        return nullptr;
    }

    pushScope(token.token, EScopeType::Namespace, EAccessControlType::Public);

    while (!matchSymbol(TEXT("}"))) {
        FStatementNode *statement;
        while ((statement = parseStatement()) != nullptr) {
            node->statements.add(statement);
        }

        /*_delete(node);
        return error("Invalid end of namespace");*/
    }

    popScope();
    return node;
}

FDirectiveNode *FParser::parseDirective() {
    FToken token;
    if (!getNextToken(token)) {
        return error("Missing compiler directive after #");
    }

    if (token.type != ETokenType::Identifier) {
        return error("Invalid directive identifier");
    }

    auto directive = new FDirectiveNode();
    directive->token = token;

    bool multiLineEnabled = false;
    if (token.token == "define") {
        multiLineEnabled = true;
    } else if (token.token == "include") {
        FToken includeToken;
        if (!getNextToken(includeToken, true)) {
            return error("Missing compiler include statement after #");
        }

        if (includeToken.type != ETokenType::Literal || includeToken.literal.type != ELiteralType::String) {
            return error("Missing compiler include statement after #");
        }

        directive->data = includeToken;
    }

    do {
        while (!isEoF() && (advance() != '\n'));
    } while (multiLineEnabled && mCurrentChar == '\\');

    return directive;
}

FCustomMacroNode *FParser::parseCustomMacro(FToken &token) {
    auto node = new FCustomMacroNode();

    if (!parseMacroMeta(token, token.token, TO_PSTATEMENT(node))) {
        delete node;
        return nullptr;
    }

    return node;
}

FFunctionDeclareNode *FParser::parseFunctionDeclare(FToken &token, bool bNeedReturnType, bool bIsMacro) {
#define ERROR(MSG) delete declare; return error(MSG);

    auto declare = new FFunctionDeclareNode();
    declare->isMacro = bIsMacro;

    if (!parseMacroMeta(token, mOptions.functionNameMacro, TO_PSTATEMENT(declare))) {
        delete declare;
        return nullptr;
    }

    bool isVirtual = false, isInline = false, isConstExpr = false, isStatic = false;
    for (bool matched = true; matched;) {
        matched = (!isVirtual && (isVirtual = matchIdentifier(TEXT("virtual")))) ||
                  (!isInline && (isInline = matchIdentifier(TEXT("inline")))) ||
                  (!isConstExpr && (isConstExpr = matchIdentifier(TEXT("constexpr")))) ||
                  (!isStatic && (isStatic = matchIdentifier(TEXT("static"))));
    }

    bool isTemplate = false;
    if (!isTemplate && (isTemplate = matchIdentifier(TEXT("template")))) {
        requireSymbol(TEXT("<"));

        do {
            FToken paramType;
            if (!getIdentifier(paramType)) {
                ERROR("Invalid parameter type");
            }

            FToken paramValue;
            if (!getIdentifier(paramValue)) {
                ERROR("Invalid parameter name");
            }
        } while (matchSymbol(TEXT(",")));

        requireSymbol(TEXT(">"));
    }

    if (bNeedReturnType) {
        declare->returns = parseType();
        if (!declare->returns) {
            ERROR("Invalid return type");
        }
    }

    if (!getIdentifier(declare->token)) {
        ERROR("Missing function name");
    }

    requireSymbol(TEXT("("));

    if (!matchSymbol(TEXT(")"))) {
        do {
            FArgument argument;
            argument.type = parseType();
            if (!argument.type) {
                ERROR("Missing argument type");
            }

            FToken nameToken;
            if (!getIdentifier(nameToken)) {
                ERROR("Missing argument name");
            }
            argument.name = nameToken.token;

            // Parse default value
            if (matchSymbol(TEXT("="))) {
                FString defaultValue;
                FToken innerToken;
                getNextToken(innerToken);
                if (innerToken.type == ETokenType::Literal) {
                    auto initializer = new FLiteralNode();
                    initializer->token = innerToken;
                    initializer->literal = innerToken.literal;
                    argument.initializer = initializer;
                } else {
                    // TODO: parse expression
                    do {
                        if (innerToken.token == "," || innerToken.token == ")") {
                            undo(innerToken);
                            break;
                        }

                        defaultValue += innerToken.token;
                    } while (getNextToken(innerToken));

                    auto initializer = new FLiteralNode();
                    initializer->literal.type = ELiteralType::String;
                    initializer->literal.s = defaultValue;
                    argument.initializer = initializer;
                }
            } else {
                argument.initializer = nullptr;
            }
        } while (matchSymbol(TEXT(",")));

        requireSymbol(TEXT(")"));
    }

    int blockLevel = 1;
    if (matchSymbol(TEXT("{"))) {
        FToken temp;
        do {
            getNextToken(temp);
            if (temp.token == TEXT("{")) {
                blockLevel += 1;
            } else if (temp.token == TEXT("}")) {
                blockLevel -= 1;
            }
        } while (blockLevel > 0);
    } else {
        matchIdentifier(TEXT("override"));

        requireSymbol(TEXT(";"));
    }
    return declare;

#undef ERROR
}

FStructDeclareNode *FParser::parseStruct(FToken &token) {
#define ERROR(MSG) delete declare; return error(MSG);

    auto declare = new FStructDeclareNode();

    if (!parseMacroMeta(token, mOptions.structNameMacro, TO_PSTATEMENT(declare))) {
        delete declare;
        return nullptr;
    }

    if (!requireIdentifier(TEXT("struct"))) {
        ERROR("Missing identifier struct");
    }

    matchIdentifier(mOptions.apiMacro.getData());
    if (!getIdentifier(declare->token)) {
        ERROR("Missing identifier");
    }

    if (matchSymbol(TEXT(":"))) {
        FToken accessOrName;
        if (!getIdentifier(accessOrName)) {
            ERROR("Missing class or access token");
        }

        declare->baseAccess = EAccessControlType::Private;
        if (!parseAccessControl(accessOrName, declare->baseAccess)) {
            // identifier
            undo(accessOrName);
        }

        auto base = parseType();
        if (base == nullptr) {
            ERROR("Invalid base type");
        }

        declare->base = base;
    }

    if (!requireSymbol(TEXT("{"))) {
        ERROR("Unexpected end of class");
    }

    pushScope(declare->token.token, EScopeType::Struct, EAccessControlType::Private);

    EAccessControlType fieldAccessType = EAccessControlType::Private;
    while (true) {
        // PARSE MANUALLY?
        if (!getNextToken(token)) {
            ERROR("Invalid token");
        }

        if (token.type == ETokenType::Identifier) {
            if (token.token == "private") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR("Invalid end of line");
                }
                continue;
            } else if (token.token == "protected") {
                fieldAccessType = EAccessControlType::Protected;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR("Invalid end of line");
                }
                continue;
            } else if (token.token == "public") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR("Invalid end of line");
                }
                continue;
            } else if (token.token == declare->token.token) {
                // constructor!
                undo(token);
                declare->constructor = parseFunctionDeclare(token, false, false);
                if (declare->constructor == nullptr) {
                    ERROR("Invalid constructor");
                }
            } else {
                undo(token);
                auto statement = parseStatement();
                if (statement->isField()) {
                    declare->fields.add(static_cast<FFieldNode *>(statement));
                } else if (statement->isDeclare()) {
                    if (statement->token.token == mOptions.generatedMacro) {
                        declare->generated = reinterpret_cast<FFunctionDeclareNode *>(statement);
                    } else {
                        ERROR("Function not allow to struct");
                    }
                }
                continue;
            }
        } else {
            if (token.token == "}") {
                break;
            } else if (token.token == "~") {
                // Destroyer
                declare->destructor = parseFunctionDeclare(token, false, false);
                if (declare->destructor == nullptr) {
                    ERROR("Invalid destructor");
                }
                continue;
            }

            // symbol for statement?
            ERROR("Invalid symbol");
        }

        /*auto statement = parseStatement();
        if (!statement) {
            _delete(declare);
            return nullptr;
        }*/
    }

    return declare;

#undef ERROR
}

FClassDeclareNode *FParser::parseClass(FToken &token) {
#define ERROR(MSG) delete declare; return error(MSG);

    auto declare = new FClassDeclareNode();

    if (!parseMacroMeta(token, mOptions.classNameMacro, TO_PSTATEMENT(declare))) {
        delete declare;
        return nullptr;
    }

    if (matchIdentifier(TEXT("template"))) {
        ERROR("template class is not support yet");
    }

    if (!requireIdentifier(TEXT("class"))) {
        ERROR("Missing identifier class");
    }

    matchIdentifier(*mOptions.apiMacro);

    if (!getIdentifier(declare->token)) {
        ERROR("Missing identifier");
    }

    if (matchSymbol(TEXT(":"))) {
        FToken accessOrName;
        if (!getIdentifier(accessOrName)) {
            ERROR("Missing class or access token");
        }

        declare->baseAccess = EAccessControlType::Private;
        if (!parseAccessControl(accessOrName, declare->baseAccess)) {
            // identifier
            undo(accessOrName);
        }

        auto base = parseType();
        if (base == nullptr) {
            ERROR("Invalid base type");
        }

        declare->base = base;
    }

    if (!requireSymbol(TEXT("{"))) {
        ERROR("Unexpected end of class");
    }

    pushScope(declare->token.token, EScopeType::Class, EAccessControlType::Private);

    EAccessControlType fieldAccessType = EAccessControlType::Private;
    while (true) {
        // PARSE MANUALLY?
        if (!getNextToken(token)) {
            ERROR("Invalid token");
        }

        if (token.type == ETokenType::Identifier) {
            if (token.token == "private") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR("Invalid end of line");
                }
                continue;
            } else if (token.token == "protected") {
                fieldAccessType = EAccessControlType::Protected;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR("Invalid end of line");
                }
                continue;
            } else if (token.token == "public") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR("Invalid end of line");
                }
                continue;
            } else if (token.token == declare->token.token) {
                FToken temp;
                if (!getNextToken(temp)) {
                    ERROR("Invalid syntax");
                }

                undo(token);
                if (temp.token == "(") {
                    // constructor!
                    declare->constructor = parseFunctionDeclare(token, false, false);
                    if (declare->constructor == nullptr) {
                        ERROR("Invalid constructor");
                    }
                } else {
                    auto statement = parseStatement();
                    if (statement->isField()) {
                        auto field = static_cast<FFieldNode *>(statement);
                        field->access = fieldAccessType;
                        declare->fields.add(field);
                    } else if (statement->isDeclare()) {
                        if (statement->token.token == mOptions.generatedMacro) {
                            declare->generated = reinterpret_cast<FFunctionDeclareNode *>(statement);
                            declare->generated->access = fieldAccessType;
                        } else {
                            auto function = static_cast<FDeclareNode *>(statement);
                            function->access = fieldAccessType;
                            declare->functions.add(function);
                        }
                    }
                    continue;
                }
            } else {
                undo(token);
                auto statement = parseStatement();
                if (statement->isField()) {
                    auto field = static_cast<FFieldNode *>(statement);
                    field->access = fieldAccessType;
                    declare->fields.add(field);
                } else if (statement->isDeclare()) {
                    if (statement->token.token == mOptions.generatedMacro) {
                        declare->generated = reinterpret_cast<FFunctionDeclareNode *>(statement);
                        declare->generated->access = fieldAccessType;
                    } else {
                        auto function = static_cast<FDeclareNode *>(statement);
                        function->access = fieldAccessType;
                        declare->functions.add(function);
                    }
                }
                continue;
            }
        } else {
            if (token.token == "}") {
                break;
            } else if (token.token == "~") {
                // Destroyer
                declare->destructor = parseFunctionDeclare(token, false, false);
                if (declare->destructor == nullptr) {
                    ERROR("Invalid destructor");
                }
                continue;
            }

            // symbol for statement?
            ERROR("Invalid symbol");
        }

        /*auto statement = parseStatement();
        if (!statement) {
            _delete(declare);
            return nullptr;
        }*/
    }

    return declare;

#undef ERROR
}

FEnumDeclareNode *FParser::parseEnum(FToken &token) {
    FEnumDeclareNode *declare = new FEnumDeclareNode();

    if (!parseMacroMeta(token, mOptions.enumNameMacro, TO_PSTATEMENT(declare))) {
        delete declare;
        return nullptr;
    }

    if (!requireIdentifier(TEXT("enum"))) {
        delete declare;
        return nullptr;
    }

    bool isEnumClass = matchIdentifier(TEXT("class"));

    // parse name
    FToken enumToken;
    if (!getIdentifier(declare->token)) {
        delete declare;
        return error("Invalid enum syntax");
    }

    if (isEnumClass) {
        declare->cxxclass = true;
    } else {
        declare->cxxclass = false;
    }

    if (matchSymbol(TEXT(":"))) {
        FToken baseToken;
        if (!getIdentifier(baseToken)) {
            delete declare;
            return error("Missing enum type specifier after :");
        }

        auto base = new FLiteralTypeNode();
        base->token = baseToken;

        declare->base = base;
    }

    requireSymbol(TEXT("{"));
    if (!matchSymbol(TEXT("}"))) {
        FToken token;
        do {
            auto field = parseEnumField(token);
            if (field == nullptr) {
                /*_delete(declare);
                return error("Invalid field entry");*/
                break;
            }

            declare->fields.add(TO_FIELD(field));
        } while (matchSymbol(TEXT(",")) && !matchSymbol(TEXT("}"), true));
    }

    if (!requireSymbol(TEXT("}"))) {
        delete declare;
        return error("Invalid end of enum compound");
    }

    matchSymbol(TEXT(";"));
    return declare;
}

FPropertyNode *FParser::parseProperty(FToken &token) {
#define ERROR(MSG) delete property; return error(MSG);

    auto property = new FPropertyNode();

    if (!parseMacroMeta(token, mOptions.propertyNameMacro, TO_PSTATEMENT(property))) {
        delete property;
        return nullptr;
    }

    bool isMutable = false, isStatic = false;
    for (bool matched = true; matched;) {
        matched = (!isMutable && (isMutable = matchIdentifier(TEXT("mutable")))) ||
                  (!isStatic && (isStatic = matchIdentifier(TEXT("static"))));
    }

    property->dataType = parseType();
    if (!property->dataType) {
        ERROR("Invalid data type");
    }

    if (!getIdentifier(property->token)) {
        ERROR("Missing variable name");
    }

    if (matchSymbol(TEXT("["))) {
        ERROR("Array type is not support yet!");
    }

    // ignore initializer
    FToken t;
    while (getNextToken(t)) {
        if (t.token == TEXT(";")) {
            break;
        }
    }

    return property;

#undef ERROR
}

FEnumFieldNode *FParser::parseEnumField(FToken &token) {
    if (!getIdentifier(token)) {
        return nullptr;
    }

    auto field = new FEnumFieldNode();
    field->token = token;

    if (matchSymbol(TEXT("="))) {
        FString value;

        while (getNextToken(token) && token.token != mOptions.enumEntryNameMacro &&
               (token.type != ETokenType::Symbol || (token.token != "," && token.token != "}"))) {
            value += token.token;
        }

        auto initializer = new FLiteralNode();
        // initializer->token = token;
        initializer->literal = token.literal;
        field->initializer = initializer;

        undo(token);
    } else {
        field->initializer = nullptr;
    }

    parseMacroMeta(token, mOptions.enumEntryNameMacro, TO_PSTATEMENT(field));

    return field;
}

FTypeNode *FParser::parseType() {
    bool isConst = false;
    bool isVolatile = false;
    bool isMutable = false;
    for (bool matched = true; matched;) {
        matched = (!isConst && (isConst = matchIdentifier(TEXT("const"))) ||
                   !isVolatile && (isVolatile = matchIdentifier(TEXT("volatile"))) ||
                   !isMutable && (isMutable = matchIdentifier(TEXT("mutable"))));
    }

    auto declarator = parseTypeDeclarator();
    isConst |= matchIdentifier(TEXT("const"));

    bool isUnSigned = false;
    bool isSigned = false;
    for (bool matched = true; matched;) {
        matched = (!isUnSigned && (isUnSigned = matchIdentifier(TEXT("unsigned"))));
        matched = (!isSigned && (isSigned = matchIdentifier(TEXT("signed"))));
    }

    if (isSigned && isUnSigned) {
        return error("Invalid combination of type specifiers");
    }

    FTypeNode *type = nullptr;

    if (matchSymbol(TEXT("<"))) {
        auto templateType = new FTemplateTypeNode();
        templateType->token.type = ETokenType::Identifier;
        templateType->token.token = declarator;

        do {
            auto argument = parseType();
            templateType->arguments.add(argument);
        } while (matchSymbol(TEXT(",")));

        if (!requireSymbol(TEXT(">"))) {
            return error("'>' required");
        }

        type = templateType;
    } else {
        type = new FLiteralTypeNode();
        type->token.type = ETokenType::Identifier;
        type->token.token = declarator;
    }

    FToken token;

    while (getNextToken(token)) {
        if (token.token == "&") {
            auto temp = new FReferenceType();
            temp->base = type;
            type = temp;
        } else if (token.token == "&&") {
            auto temp = new FLReferenceType();
            temp->base = type;
            type = temp;
        } else if (token.token == "*") {
            auto temp = new FPointerType();
            temp->base = type;
            type = temp;
        } else {
            undo(token);
            break;
        }

        if (matchSymbol(TEXT("const"))) {
            type->isConst = true;
        }
    }

    // function pointer?
    if (matchSymbol(TEXT("("))) {
        // parse void(*)(args, ...)
        //            ^
        //            |

        if (matchSymbol(TEXT("*"))) {
            FToken token;
            getNextToken(token);
            if (token.token != ")" || (token.type != ETokenType::Identifier && !matchSymbol(TEXT(")")))) {
                return error("Unexpected syntax");
            }

            auto funcNode = new FFunctionPointerType();
            funcNode->returns = type;

            if (!matchSymbol(TEXT(")"))) {
                do {
                    auto argument = new FArgument();
                    argument->type = parseType();
                    if (argument->type == nullptr) {
                        return nullptr;
                    }

                    if (!getNextToken(token)) {
                        return error("Unexpected end of file");
                    }

                    if (token.type == ETokenType::Identifier) {
                        argument->name = token.token;
                    } else {
                        undo(token);
                    }

                    funcNode->arguments.add(argument);
                } while (matchSymbol(TEXT(",")));

                if (!matchSymbol(TEXT(")"))) {
                    return error("Unexpected syntax");
                }
            }

            type = funcNode;
        }
    }

    type->isVolatile = isVolatile;
    type->isMutable = isMutable;
    type->bUnSigned = isUnSigned;
    return type;
}

bool FParser::parseMacroMeta(FToken &token, FString macroName, FStatementNode **declare) {
    if (!matchIdentifier(*macroName)) {
        (*declare)->bHasPrefixMacro = false;
        return true;
    }

    if (!requireSymbol(TEXT("("))) {
        return false;
    }

    if (!parseMetaSequence(token, "", declare)) {
        return false;
    }

    // Is allow semicolon on end of metadata ?
    (*declare)->bHasPrefixMacro = true;
    return true;
}

bool FParser::parseMetaSequence(FToken &token, FString scope, FStatementNode **declare) {
    if (!matchSymbol(TEXT(")"))) {
        do {
            FToken keyToken;
            if (!getIdentifier(keyToken)) {
                _error(TEXT("Expected identifier in meta sequence"));
                return false;
            }
            const FString key = scope.empty() ? keyToken.token : FString::Printf(TEXT("%ls.%ls"), *scope, *keyToken.token);

            FToken valueToken;
            if (matchSymbol(TEXT("="))) {
                if (matchSymbol(TEXT("("))) {
                    // compound value?
                    parseMetaSequence(token, key, declare);
                    continue;
                }

                if (!getNextToken(valueToken)) {
                    _error(TEXT("Invalid meta sequence syntax"));
                    throw;
                }

                if (valueToken.type == ETokenType::Symbol) {
                    _error(TEXT("Invalid meta sequence syntex"));
                    throw;
                } else {
                    (*declare)->meta.add(key, valueToken.token);
                }
            } else {
                (*declare)->meta.add(key, TEXT(""));
            }
        } while (matchSymbol(TEXT(",")));

        matchSymbol(TEXT(")"));
    }

    return true;
}

bool FParser::parseAccessControl(const FToken &token, EAccessControlType &type) {
    if (token.token == "public") {
        type = EAccessControlType::Public;
        return true;
    } else if (token.token == "protected") {
        type = EAccessControlType::Protected;
        return true;
    } else if (token.token == "private") {
        type = EAccessControlType::Private;
        return true;
    }

    return false;
}

FString FParser::parseTypeDeclarator() {
    matchIdentifier(TEXT("class"));
    matchIdentifier(TEXT("struct"));
    matchIdentifier(TEXT("typename"));

    FStringBuilder ss(512);
    FToken token;
    bool first = true;

    while (true) {
        if (matchSymbol(TEXT("::"))) {
            ss << "::";
        } else if (!first) {
            break;
        }

        first = false;

        if (!getIdentifier(token)) {
            _error(TEXT("Expected identifier: %s"), *token.token);
            return "";
        }

        ss << token.token;
    }

    return ss.toString();
}

void FParser::pushScope(const FString &name, EScopeType scopeType, EAccessControlType accessControlType) {
    if (mScopeIndex == mScopeList.size() - 1) {
        throw;
    }

    mTopScope = &mScopeList[++mScopeIndex];
    mTopScope->type = scopeType;
    mTopScope->name = name;
    mTopScope->currentAccessControlType = accessControlType;
}

void FParser::popScope() {
    if (mScopeIndex == 0) {
        throw;
    }

    mTopScope = &mScopeList[--mScopeIndex];
}

void FParser::skipDeclaration(FToken &token) {
    int32_t scopeDepth = 0;
    while (getNextToken(token)) {
        if (token.token == ";" && scopeDepth == 0) {
            break;
        }

        if (token.token == "{") {
            scopeDepth++;
        }

        if (token.token == "}") {
            --scopeDepth;
            if (scopeDepth == 0) {
                break;
            }
        }
    }
}