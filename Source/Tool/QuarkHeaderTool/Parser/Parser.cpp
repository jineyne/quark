#include "Parser.h"
#include "Misc/StringBuilder.h"

Parser::Parser(Options options) : mOptions(options) {}

CompoundNode *Parser::parse(const String &input) {
    reset(input);

    mTopScope = &mScopeList[mScopeIndex];
    mTopScope->name = "Global";
    mTopScope->type = EScopeType::Global;
    mTopScope->currentAccessControlType = EAccessControlType::Public;

    CompoundNode *compound = new CompoundNode();

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

StatementNode *Parser::parseStatement() {
    Token token;
    if (!getNextToken(token)) {
        return nullptr;
    }

    return parseDeclaration(token);
}

StatementNode *Parser::parseDeclaration(Token &token) {
    if (hasError()) {
        return nullptr;
    }

    TArray<String> customMacroIt;
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
    } else if (token.token == TEXT("template")) {
        undo(token);
        return parseFunctionDeclare(token, true, false);
    } else if (token.token == TEXT("namespace")) {
        return parseNamespace();
    } else if (mOptions.customFunctionMacros.contains(token.token)) {
        undo(token);
        return parseFunctionCall(token, true);
    } else if (mOptions.customMacros.contains(token.token)) {
        undo(token);
        return parseCustomMacro(token);
    } else if (token.token == ";") {
        return nullptr;
    } else if (token.token == TEXT("class") || token.token == TEXT("struct")) {
        undo(token);

        auto type = parseType();
        bool bIsApi = type->token.token == mOptions.apiMacro;
        delete type;

        Token temp;

        // skip type identifier
        if (bIsApi) {
            if (!getIdentifier(temp)) {
                return nullptr;
            }
        }

        // maybe {?
        if (!getNextToken(temp)) {
            return nullptr;
        }

        if (temp.type != ETokenType::Symbol) {
            return error(TEXT("Invalid identifier"));
        }

        undo(token);
        if (temp.token == TEXT("=") || temp.token == TEXT(";")) {
            return parseProperty(token);
        } else if (temp.token == TEXT(":") || temp.token == TEXT("{")) {
            if (token.token == TEXT("class")) {
                return parseClass(token);
            } else {
                return parseStruct(token);
            }
        } else if (temp.token == TEXT("(")) {
            return parseFunctionDeclare(token, true, false);
        } else {
            return error(TEXT("Invalid syntax"));
        }
    } else {
        undo(token);

        auto type = parseType();
        delete type;

        Token temp;
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
    return error(TEXT("Invalid identifier"));
}

NamespaceNode *Parser::parseNamespace() {
    Token token;
    if (!getIdentifier(token)) {
        error(TEXT("missing namespace name"));
        return nullptr;
    }

    auto node = new NamespaceNode();
    node->token = token;

    if (!requireSymbol(TEXT("{"))) {
        delete node;
        return nullptr;
    }

    pushScope(token.token, EScopeType::Namespace, EAccessControlType::Public);

    while (!matchSymbol(TEXT("}"))) {
        StatementNode *statement;
        while ((statement = parseStatement()) != nullptr) {
            node->statements.add(statement);
        }

        /*q_delete(node);
        return ERROR(TEXT("Invalid end of namespace");*/
    }

    popScope();
    return node;
}

DirectiveNode *Parser::parseDirective() {
    Token token;
    if (!getNextToken(token)) {
        return error(TEXT("Missing compiler directive after #"));
    }

    if (token.type != ETokenType::Identifier) {
        return error(TEXT("Invalid directive identifier"));
    }

    auto directive = new DirectiveNode();
    directive->token = token;

    bool multiLineEnabled = false;
    if (token.token == "define") {
        multiLineEnabled = true;
    } else if (token.token == "include") {
        Token includeToken;
        if (!getNextToken(includeToken, true)) {
            return error(TEXT("Missing compiler include statement after #"));
        }

        if (includeToken.type != ETokenType::Literal || includeToken.literal.type != ELiteralType::String) {
            return error(TEXT("Missing compiler include statement after #"));
        }

        directive->data = includeToken;
    }

    do {
        while (!isEoF() && (advance() != '\n'));
    } while (multiLineEnabled && mCurrentChar == '\\');

    return directive;
}

CustomMacroNode *Parser::parseCustomMacro(Token &token) {
    auto node = new CustomMacroNode();

    if (!parseMacroMeta(token, token.token, TO_PSTATEMENT(node))) {
        delete node;
        return nullptr;
    }

    return node;
}

FunctionCallNode *Parser::parseFunctionCall(Token &token, bool bIsMacro) {
#define ERROR(...) delete declare; return error(__VA_ARGS__);

    auto declare = new FunctionCallNode();
    if (!getIdentifier(declare->token)) {
        ERROR(TEXT("Missing function name"));
    }

    size_t level = 1;
    requireSymbol(TEXT("("));

    // skip inner arguments
    while (level > 0) {
        Token inner;
        getNextToken(inner);

        if (inner.token == TEXT("(")) {
            level++;
        } else if (inner.token == TEXT(")")) {
            level--;
        }
    };

    matchSymbol(TEXT(";"));

    return declare;

#undef ERROR
}

FunctionDeclareNode *Parser::parseFunctionDeclare(Token &token, bool bNeedReturnType, bool bIsMacro) {
#define ERROR(...) delete declare; return error(__VA_ARGS__);

    auto declare = new FunctionDeclareNode();
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
            Token paramType;
            if (!getIdentifier(paramType)) {
                ERROR(TEXT("Invalid parameter type"));
            }

            Token paramValue;
            if (!getIdentifier(paramValue)) {
                ERROR(TEXT("Invalid parameter name"));
            }
        } while (matchSymbol(TEXT(",")));

        requireSymbol(TEXT(">"));
    }

    if (bNeedReturnType) {
        declare->returns = parseType();
        if (!declare->returns) {
            ERROR(TEXT("Invalid return type"));
        }
    }

    if (!getIdentifier(declare->token)) {
        ERROR(TEXT("Missing function name"));
    }

    requireSymbol(TEXT("("));

    if (!matchSymbol(TEXT(")"))) {
        do {
            Argument argument;
            argument.type = parseType();
            if (!argument.type) {
                ERROR(TEXT("Missing argument type"));
            }

            Token nameToken;
            if (!getIdentifier(nameToken)) {
                ERROR(TEXT("Missing argument name"));
            }
            argument.name = nameToken.token;

            // Parse default mValue
            if (matchSymbol(TEXT("="))) {
                String defaultValue;
                Token innerToken;
                getNextToken(innerToken);
                if (innerToken.type == ETokenType::Literal) {
                    auto initializer = new LiteralNode();
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

                    auto initializer = new LiteralNode();
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

    matchIdentifier(TEXT("override"));
    matchIdentifier(TEXT("const"));

    bool bIsPureVirtualMethod = false;

    // pure virtual method
    if (matchSymbol(TEXT("="))) {
        requireSymbol(TEXT("0"));
        requireSymbol(TEXT(";"));

        bIsPureVirtualMethod = true;
    }

    int blockLevel = 1;
    if (matchSymbol(TEXT("{"))) {
        if (bIsPureVirtualMethod) {
            ERROR(TEXT("%ls is pure virtual method"), *declare->token.token);
        }

        Token temp;
        do {
            getNextToken(temp);
            if (temp.token == TEXT("{")) {
                blockLevel += 1;
            } else if (temp.token == TEXT("}")) {
                blockLevel -= 1;
            }
        } while (blockLevel > 0);
    } else {
        requireSymbol(TEXT(";"));
    }
    return declare;

#undef ERROR
}

StructDeclareNode *Parser::parseStruct(Token &token) {
#define ERROR(...) delete declare; return error(__VA_ARGS__);

    auto declare = new StructDeclareNode();

    if (!parseMacroMeta(token, mOptions.structNameMacro, TO_PSTATEMENT(declare))) {
        delete declare;
        return nullptr;
    }

    if (!requireIdentifier(TEXT("struct"))) {
        ERROR(TEXT("Missing identifier struct"));
    }

    matchIdentifier(mOptions.apiMacro.getData());
    if (!getIdentifier(declare->token)) {
        ERROR(TEXT("Missing identifier"));
    }

    if (matchSymbol(TEXT(":"))) {
        Token accessOrName;
        if (!getIdentifier(accessOrName)) {
            ERROR(TEXT("Missing class or access token"));
        }

        declare->baseAccess = EAccessControlType::Private;
        if (!parseAccessControl(accessOrName, declare->baseAccess)) {
            // identifier
            undo(accessOrName);
        }

        auto base = parseType();
        if (base == nullptr) {
            ERROR(TEXT("Invalid base type"));
        }

        declare->base = base;
    }

    if (!requireSymbol(TEXT("{"))) {
        ERROR(TEXT("Unexpected end of class"));
    }

    pushScope(declare->token.token, EScopeType::Struct, EAccessControlType::Private);

    EAccessControlType fieldAccessType = EAccessControlType::Private;
    while (true) {
        // PARSE MANUALLY?
        if (!getNextToken(token)) {
            ERROR(TEXT("Invalid token"));
        }

        if (token.type == ETokenType::Identifier) {
            if (token.token == "private") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR(TEXT("Invalid end of line"));
                }
                continue;
            } else if (token.token == "protected") {
                fieldAccessType = EAccessControlType::Protected;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR(TEXT("Invalid end of line"));
                }
                continue;
            } else if (token.token == "public") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR(TEXT("Invalid end of line"));
                }
                continue;
            } else if (token.token == declare->token.token) {
                // constructor!
                undo(token);
                declare->constructor = parseFunctionDeclare(token, false, false);
                if (declare->constructor == nullptr) {
                    ERROR(TEXT("Invalid constructor"));
                }
            } else {
                undo(token);
                auto statement = parseStatement();
                if (statement->isField()) {
                    declare->fields.add(static_cast<FieldNode *>(statement));
                } else if (statement->isStatement()) {
                    if (statement->token.token == mOptions.generatedMacro) {
                        declare->generated = reinterpret_cast<FunctionDeclareNode *>(statement);
                    }
                } else if (statement->isDeclare()) {
                    ERROR(TEXT("Function not allow to struct"));
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
                    ERROR(TEXT("Invalid destructor"));
                }
                continue;
            }

            // symbol for statement?
            ERROR(TEXT("Invalid symbol"));
        }

        /*auto statement = parseStatement();
        if (!statement) {
            q_delete(declare);
            return nullptr;
        }*/
    }

    requireSymbol(TEXT(";"));
    return declare;

#undef ERROR
}

ClassDeclareNode *Parser::parseClass(Token &token) {
#define ERROR(...) delete declare; return error(__VA_ARGS__);

    auto declare = new ClassDeclareNode();

    if (!parseMacroMeta(token, mOptions.classNameMacro, TO_PSTATEMENT(declare))) {
        delete declare;
        return nullptr;
    }

    if (matchIdentifier(TEXT("template"))) {
        ERROR(TEXT("template class is not support yet"));
    }

    if (!requireIdentifier(TEXT("class"))) {
        ERROR(TEXT("Missing identifier class"));
    }

    matchIdentifier(*mOptions.apiMacro);

    if (!getIdentifier(declare->token)) {
        ERROR(TEXT("Missing identifier"));
    }

    if (matchSymbol(TEXT(":"))) {
        Token accessOrName;
        if (!getIdentifier(accessOrName)) {
            ERROR(TEXT("Missing class or access token"));
        }

        declare->baseAccess = EAccessControlType::Private;
        if (!parseAccessControl(accessOrName, declare->baseAccess)) {
            // identifier
            undo(accessOrName);
        }

        auto base = parseType();
        if (base == nullptr) {
            ERROR(TEXT("Invalid base type"));
        }

        declare->base = base;
    }

    if (!requireSymbol(TEXT("{"))) {
        ERROR(TEXT("Unexpected end of class"));
    }

    pushScope(declare->token.token, EScopeType::Class, EAccessControlType::Private);

    EAccessControlType fieldAccessType = EAccessControlType::Private;
    while (true) {
        // PARSE MANUALLY?
        if (!getNextToken(token)) {
            ERROR(TEXT("Invalid token"));
        }

        if (token.type == ETokenType::Identifier) {
            if (token.token == "private") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR(TEXT("Invalid end of line"));
                }
                continue;
            } else if (token.token == "protected") {
                fieldAccessType = EAccessControlType::Protected;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR(TEXT("Invalid end of line"));
                }
                continue;
            } else if (token.token == "public") {
                fieldAccessType = EAccessControlType::Private;
                if (!requireSymbol(TEXT(":"))) {
                    ERROR(TEXT("Invalid end of line"));
                }
                continue;
            } else if (token.token == declare->token.token) {
                Token temp;
                if (!getNextToken(temp)) {
                    ERROR(TEXT("Invalid syntax"));
                }

                undo(token);
                if (temp.token == "(") {
                    // constructor!
                    declare->constructor = parseFunctionDeclare(token, false, false);
                    if (declare->constructor == nullptr) {
                        ERROR(TEXT("Invalid constructor"));
                    }
                } else {
                    auto statement = parseStatement();
                    if (statement->isField()) {
                        auto field = static_cast<FieldNode *>(statement);
                        field->access = fieldAccessType;
                        declare->fields.add(field);
                    } else if (statement->isDeclare()) {
                        if (statement->token.token == mOptions.generatedMacro) {
                            declare->generated = reinterpret_cast<FunctionDeclareNode *>(statement);
                            declare->generated->access = fieldAccessType;
                        } else {
                            auto function = static_cast<DeclareNode *>(statement);
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
                    auto field = static_cast<FieldNode *>(statement);
                    field->access = fieldAccessType;
                    declare->fields.add(field);
                } else if (statement->isStatement()) {
                    if (statement->token.token == mOptions.generatedMacro) {
                        declare->generated = reinterpret_cast<FunctionDeclareNode *>(statement);
                        declare->generated->access = fieldAccessType;
                    }
                } else if (statement->isDeclare()) {
                    auto function = static_cast<DeclareNode *>(statement);
                    function->access = fieldAccessType;
                    declare->functions.add(function);
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
                    ERROR(TEXT("Invalid destructor"));
                }
                continue;
            } else if (token.token == TEXT("#")) {
                delete parseDirective();
            }

            // symbol for statement?
            ERROR(TEXT("Invalid symbol"));
        }

        /*auto statement = parseStatement();
        if (!statement) {
            q_delete(declare);
            return nullptr;
        }*/
    }

    return declare;

#undef ERROR
}

EnumDeclareNode *Parser::parseEnum(Token &token) {
    EnumDeclareNode *declare = new EnumDeclareNode();

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
    Token enumToken;
    if (!getIdentifier(declare->token)) {
        delete declare;
        return error(TEXT("Invalid enum syntax"));
    }

    if (isEnumClass) {
        declare->cxxclass = true;
    } else {
        declare->cxxclass = false;
    }

    if (matchSymbol(TEXT(":"))) {
        Token baseToken;
        if (!getIdentifier(baseToken)) {
            delete declare;
            return error(TEXT("Missing enum type specifier after :"));
        }

        auto base = new LiteralTypeNode();
        base->token = baseToken;

        declare->base = base;
    }

    requireSymbol(TEXT("{"));
    if (!matchSymbol(TEXT("}"))) {
        Token token;
        do {
            auto field = parseEnumField(token);
            if (field == nullptr) {
                /*q_delete(declare);
                return ERROR(TEXT("Invalid field entry");*/
                break;
            }

            declare->fields.add(TO_FIELD(field));
        } while (matchSymbol(TEXT(",")) && !matchSymbol(TEXT("}"), true));
    }

    if (!requireSymbol(TEXT("}"))) {
        delete declare;
        return error(TEXT("Invalid end of enum compound"));
    }

    matchSymbol(TEXT(";"));
    return declare;
}

PropertyNode *Parser::parseProperty(Token &token) {
#define ERROR(...) delete property; return error(__VA_ARGS__);

    auto property = new PropertyNode();

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
        ERROR(TEXT("Invalid data type"));
    }

    if (!getIdentifier(property->token)) {
        ERROR(TEXT("Missing variable name"));
    }

    if (matchSymbol(TEXT("["))) {
        ERROR(TEXT("Array type is not support yet!"));
    }

    // ignore initializer
    Token t;
    while (getNextToken(t)) {
        if (t.token == TEXT(";")) {
            break;
        }
    }

    return property;

#undef ERROR
}

EnumFieldNode *Parser::parseEnumField(Token &token) {
    if (!getIdentifier(token)) {
        return nullptr;
    }

    auto field = new EnumFieldNode();
    field->token = token;

    if (matchSymbol(TEXT("="))) {
        String value;

        while (getNextToken(token) && token.token != mOptions.enumEntryNameMacro &&
               (token.type != ETokenType::Symbol || (token.token != "," && token.token != "}"))) {
            value += token.token;
        }

        auto initializer = new LiteralNode();
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

TypeNode *Parser::parseType() {
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
        return error(TEXT("Invalid combination of type specifiers"));
    }

    TypeNode *type = nullptr;

    if (matchSymbol(TEXT("<"))) {
        auto templateType = new TemplateTypeNode();
        templateType->token.type = ETokenType::Identifier;
        templateType->token.token = declarator;

        do {
            auto argument = parseType();
            templateType->arguments.add(argument);
        } while (matchSymbol(TEXT(",")));

        if (!requireSymbol(TEXT(">"))) {
            return error(TEXT("'>' required"));
        }

        type = templateType;
    } else {
        type = new LiteralTypeNode();
        type->token.type = ETokenType::Identifier;
        type->token.token = declarator;
    }

    Token token;

    while (getNextToken(token)) {
        if (token.token == "&") {
            auto temp = new ReferenceType();
            temp->base = type;
            type = temp;
        } else if (token.token == "&&") {
            auto temp = new LReferenceType();
            temp->base = type;
            type = temp;
        } else if (token.token == "*") {
            auto temp = new PointerType();
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
            Token token;
            getNextToken(token);
            if (token.token != ")" || (token.type != ETokenType::Identifier && !matchSymbol(TEXT(")")))) {
                return error(TEXT("Unexpected syntax"));
            }

            auto funcNode = new FunctionPointerType();
            funcNode->returns = type;

            if (!matchSymbol(TEXT(")"))) {
                do {
                    auto argument = new Argument();
                    argument->type = parseType();
                    if (argument->type == nullptr) {
                        return nullptr;
                    }

                    if (!getNextToken(token)) {
                        return error(TEXT("Unexpected end of file"));
                    }

                    if (token.type == ETokenType::Identifier) {
                        argument->name = token.token;
                    } else {
                        undo(token);
                    }

                    funcNode->arguments.add(argument);
                } while (matchSymbol(TEXT(",")));

                if (!matchSymbol(TEXT(")"))) {
                    return error(TEXT("Unexpected syntax"));
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

bool Parser::parseMacroMeta(Token &token, String macroName, StatementNode **declare) {
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

bool Parser::parseMetaSequence(Token &token, String scope, StatementNode **declare) {
    if (!matchSymbol(TEXT(")"))) {
        do {
            Token keyToken;
            if (!getIdentifier(keyToken)) {
                _error(TEXT("Expected identifier in meta sequence"));
                return false;
            }
            const String key = scope.empty() ? keyToken.token : String::Printf(TEXT("%ls.%ls"), *scope, *keyToken.token);

            Token valueToken;
            if (matchSymbol(TEXT("="))) {
                if (matchSymbol(TEXT("("))) {
                    // compound mValue?
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

bool Parser::parseAccessControl(const Token &token, EAccessControlType &type) {
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

String Parser::parseTypeDeclarator() {
    matchIdentifier(TEXT("class"));
    matchIdentifier(TEXT("struct"));
    matchIdentifier(TEXT("typename"));

    FStringBuilder ss(512);
    Token token;
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

void Parser::pushScope(const String &name, EScopeType scopeType, EAccessControlType accessControlType) {
    if (mScopeIndex == mScopeList.size() - 1) {
        throw;
    }

    mTopScope = &mScopeList[++mScopeIndex];
    mTopScope->type = scopeType;
    mTopScope->name = name;
    mTopScope->currentAccessControlType = accessControlType;
}

void Parser::popScope() {
    if (mScopeIndex == 0) {
        throw;
    }

    mTopScope = &mScopeList[--mScopeIndex];
}

void Parser::skipDeclaration(Token &token) {
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