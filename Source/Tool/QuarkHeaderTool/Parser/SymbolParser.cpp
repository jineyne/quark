#include "SymbolParser.h"

SymbolParser::SymbolParser(SymbolParser::Options options) : mOptions(options) { }

TArray<Symbol *> *SymbolParser::run(const String &input) {
    reset(input);

    // set global scope
    mTopScope = &mScopeList[mScopeIndex];
    mTopScope->name = "Global";
    mTopScope->type = EScopeType::Global;
    mTopScope->currentAccessControlType = EAccessControlType::Public;

    while (statement()) {}

    // error caused
    if (!isEoF()) {
        for (auto symbol : mSymbols) {
            delete symbol;
        }

        return nullptr;
    }

    return &mSymbols;
}

bool SymbolParser::statement() {
    Token token;
    if (!getNextToken(token)) {
        return false;
    }

    return declaration(token);
}

bool SymbolParser::declaration(Token &token) {
    if (token.token == mOptions.enumNameMacro) {
        undo(token);
        return enum_(token);
    } else if (token.token == mOptions.structNameMacro) {
        if (mTopScope->type != EScopeType::Global) {
            _error(TEXT("inner structure not support!"));
            return false;
        }

        undo(token);
        return struct_(token);
    } else if (token.token == mOptions.classNameMacro) {
        if (mTopScope->type != EScopeType::Global) {
            _error(TEXT("inner class not support!"));
            return false;
        }

        undo(token);
        return class_(token);
    }

    return !isEoF();
}

bool SymbolParser::enum_(Token &token) {
    // add empty symbol
    mSymbols.add(new Symbol());
    auto symbol = mSymbols.top();

    // parse meta
    if (!meta(token, mOptions.enumNameMacro, symbol)) {
        mSymbols.pop();
        return false;
    }

    // is enum class?
    bool isEnumClass = matchIdentifier(TEXT("class"));
    symbol->extras.add(TEXT("cxxclass"), isEnumClass ? TEXT("true") : TEXT("false"));

    // parse name
    if (!getIdentifier(token)) {
        _error(TEXT("Expected enum name"));
        return false;
    }

    symbol->name = token.token;

    // skip until block
    while (true) {
        if (!getNextToken(token)) {
            _error(TEXT("Expected enum content"));
        }

        // declare forward with qenum
        if (token.token == TEXT(";")) {
            _error(TEXT("Forward declaration with QENUM macro"));
            return false;
        }

        if (token.token == TEXT("{")) {
            break;
        }
    }

    if (!matchSymbol(TEXT("}"))) {
        do {
            if (!enumEntry(token)) {
                return false;
            }
        } while (matchSymbol(TEXT(",")) && !matchSymbol(TEXT("}"), true));
    }

    // invalid end of block
    if (!requireSymbol(TEXT("}"))) {
        _error(TEXT("Invalid end of enum compound"));
        return false;
    }

    requireSymbol(TEXT(";"));
    return true;
}

bool SymbolParser::enumEntry(Token &token) {
    if (!getIdentifier(token)) {
        return false;
    }

    // add symbol symbol
    auto &children = mSymbols.top()->children;
    children.add(new Symbol());
    auto symbol = children.top();
    symbol->name = token.token;

    // symbol initializer
    if (matchSymbol(TEXT("="))) {
        while (getNextToken(token) && token.token != mOptions.enumEntryNameMacro &&
               (token.type != ETokenType::Symbol || (token.token != TEXT(",") && token.token != TEXT("}")))) {
        }

        undo(token);
    }

    meta(token, mOptions.enumEntryNameMacro, symbol);
    return true;
}

bool SymbolParser::struct_(Token &token) {
    // add empty symbol
    mSymbols.add(new Symbol());
    auto symbol = mSymbols.top();

    // parse meta
    if (!meta(token, mOptions.structNameMacro, symbol)) {
        delete mSymbols.pop();
        return false;
    }

    // skip keyword
    if (!requireIdentifier(TEXT("struct"))) {
        _error(TEXT("Missing identifier struct"));
        return false;
    }

    // skip api
    matchIdentifier(mOptions.apiMacro.getData());

    // parse name
    if (!getIdentifier(token)) {
        _error(TEXT("Missing identifier"));
        return false;
    }
    symbol->name = token.token;

    // skip until block
    while (true) {
        if (!getNextToken(token)) {
            _error(TEXT("Expected struct content"));
        }

        // declare forward with qstruct
        if (token.token == TEXT(";")) {
            _error(TEXT("Forward declaration with Struct macro"));
            return false;
        }

        if (token.token == TEXT("{")) {
            break;
        }
    }

    while (true) {
        if (!getNextToken(token)) {
            _error(TEXT("Invalid token"));
            return false;
        }

        if (token.token == mOptions.propertyNameMacro) {
            undo(token);
            property(token);
        } else if (token.token == mOptions.generatedMacro) {
            symbol->extras.add(GENERATED, String::Printf(TEXT("%d"), token.line + 1));
        } else if (token.token == TEXT("}")) {
            undo(token);
            break;
        }
    }

    // invalid end of block
    if (!requireSymbol(TEXT("}"))) {
        _error(TEXT("Invalid end of enum compound"));
        return false;
    }

    requireSymbol(TEXT(";"));
    return true;
}

bool SymbolParser::class_(Token &token) {
    // add empty symbol
    mSymbols.add(new Symbol());
    auto symbol = mSymbols.top();

    // parse meta
    if (!meta(token, mOptions.classNameMacro, symbol)) {
        delete mSymbols.pop();
        return false;
    }

    // skip keyword
    if (!requireIdentifier(TEXT("class"))) {
        _error(TEXT("Missing identifier struct"));
        return false;
    }

    // skip api
    matchIdentifier(mOptions.apiMacro.getData());

    // parse name
    if (!getIdentifier(token)) {
        _error(TEXT("Missing identifier"));
        return false;
    }
    symbol->name = token.token;

    // skip until block
    while (true) {
        if (!getNextToken(token)) {
            _error(TEXT("Expected struct content"));
        }

        // declare forward with qstruct
        if (token.token == TEXT(";")) {
            _error(TEXT("Forward declaration with Struct macro"));
            return false;
        }

        if (token.token == TEXT("{")) {
            break;
        }
    }

    size_t count = 1;
    while (true) {
        if (!getNextToken(token)) {
            _error(TEXT("Invalid token"));
            return false;
        }

        if (token.token == mOptions.propertyNameMacro) {
            undo(token);
            property(token);
        } else if (token.token == mOptions.generatedMacro) {
            symbol->extras.add(GENERATED, String::Printf(TEXT("%d"), token.line + 1));
        } else if (token.token == TEXT("{")) {
            count++;
        } else if (token.token == TEXT("}")) {
            count--;
            if (count <= 0) {
                undo(token);
                break;
            }
        }
    }

    // invalid end of block
    if (!requireSymbol(TEXT("}"))) {
        _error(TEXT("Invalid end of enum compound"));
        return false;
    }

    requireSymbol(TEXT(";"));
    return true;
}

bool SymbolParser::property(Token &token) {
    // add symbol symbol
    auto &children = mSymbols.top()->children;
    children.add(new Symbol());
    auto symbol = children.top();
    symbol->name = token.token;

    // parse meta
    if (!meta(token, mOptions.propertyNameMacro, symbol)) {
        return false;
    }

    // skip keyword
    matchIdentifier(TEXT("mutable"));
    matchIdentifier(TEXT("static"));

    // TODO: add more?

    // skip type
    if (!type()) {
        _error(TEXT("Invalid data type"));
        return false;
    }

    // parse name
    if (!getIdentifier(token)) {
        _error(TEXT("Missing variable name"));
        return false;
    }
    symbol->name = token.token;

    // skip array
    if (matchSymbol(TEXT("["))) {
        _error(TEXT("Array type is not support yet!"));
        return false;

        // TODO: array
        /*do {
            if (!type(token)) {
                return false;
            }
        } while (!matchSymbol(TEXT("]")));

        if (!requireSymbol(TEXT("]"))) {
            _error(TEXT("']' required"));
            return false;
        }*/
    }

    // skip initializer
    while (getNextToken(token)) {
        if (token.token == TEXT(";")) {
            break;
        }
    }

    return true;
}

bool SymbolParser::method(Token &token) {
    return false;
}

bool SymbolParser::meta(Token &token, String macro, Symbol *target) {
    if (!matchIdentifier(*macro)) {
        return false;
    }

    if (!metaSequence(token, TEXT(""), target)) {
        return false;
    }

    return true;
}

bool SymbolParser::metaSequence(Token &token, String scope, Symbol *target) {
    if (!requireSymbol(TEXT("("))) {
        return false;
    }

    if (!matchSymbol(TEXT(")"))) {
        do {
            Token keyToken;
            if (!getIdentifier(keyToken)) {
                _error(TEXT("Expected identifier in meta sequence"));
                return false;
            }

            const auto key = scope.empty() ? keyToken.token : String::Printf(TEXT("%ls.%ls"), *scope, *keyToken.token);
            Token valueToken;

            if (matchSymbol(TEXT("="))) {
                if (matchSymbol(TEXT("("))) {
                    metaSequence(token, key, target);
                    continue;
                }

                if (!getNextToken(valueToken)) {
                    _error(TEXT("Invalid meta sequence syntax: %ls"), *scope);
                    return false;
                }


                if (valueToken.type == ETokenType::Symbol) {
                    _error(TEXT("Invalid meta sequence syntax"));
                    return false;
                } else {
                    target->metas.add(key, valueToken.token);
                }
            } else {
                target->metas.add(key, TEXT("true"));
            }
        } while (matchSymbol(TEXT(",")));

        requireSymbol(TEXT(")"));
    }

    return true;
}

bool SymbolParser::type() {
    Token token;

    // skip keyword
    matchIdentifier(TEXT("const"));
    matchIdentifier(TEXT("volatile"));
    matchIdentifier(TEXT("mutable"));

    // skip forward declaration
    matchIdentifier(TEXT("class"));
    matchIdentifier(TEXT("struct"));
    matchIdentifier(TEXT("typename"));

    bool first = true;
    while (true) {
        if (!matchSymbol(TEXT("::")) && !first) {
            break;
        }

        first = false;

        if (!getIdentifier(token)) {
            _error(TEXT("Expected identifier: %ls"), *token.token);
            return false;
        }
    }

    if (matchIdentifier(TEXT("unsigned")) || matchIdentifier(TEXT("signed"))) {
        _error(TEXT("Invalid combination of type specifiers"));
        return false;
    }

    // skip template
    if (matchSymbol(TEXT("<"))) {
        do {
            if (!type()) {
                return false;
            }
        } while (matchSymbol(TEXT(",")));

        if (!requireSymbol(TEXT(">"))) {
            _error(TEXT("'>' required"));
            return false;
        }
    }

    // skip memory keyword
    matchSymbol(TEXT("&"));
    matchSymbol(TEXT("&&"));
    matchSymbol(TEXT("*"));

    // skip const
    matchSymbol(TEXT("const"));


    // skip function pointer
    if (matchSymbol(TEXT("("))) {
        if (!matchSymbol(TEXT(")"))) {
            do {
                if (!type()) {
                    return false;
                }

                if (!getNextToken(token)) {
                    _error(TEXT("Unexpected end of file"));
                    return false;
                }

                if (token.type != ETokenType::Identifier) {
                    undo(token);
                }
            } while (matchSymbol(TEXT(",")));

            if (!matchSymbol(TEXT(")"))) {
                _error(TEXT("Invalid end of function pointer"));
                return false;
            }
        }
    }

    return true;
}
