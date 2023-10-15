#include "SymbolParser.h"

template<typename ... Args>
std::string format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

SymbolParser::SymbolParser(SymbolParser::Options options) : mOptions(options) { }

std::vector<Symbol *> *SymbolParser::run(const std::string &input) {
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
            _error(("inner structure not support!"));
            return false;
        }

        undo(token);
        return struct_(token);
    } else if (token.token == mOptions.classNameMacro) {
        if (mTopScope->type != EScopeType::Global) {
            _error(("inner class not support!"));
            return false;
        }

        undo(token);
        return class_(token);
    }

    return !isEoF();
}

bool SymbolParser::enum_(Token &token) {
    // add empty symbol
    mSymbols.push_back(new Symbol());
    auto symbol = mSymbols.back();

    // parse meta
    if (!meta(token, mOptions.enumNameMacro, symbol)) {
        mSymbols.pop_back();
        return false;
    }

    // skip keyword
    if (!requireIdentifier(("enum"))) {
        _error(("Missing identifier struct"));
        return false;
    }

    // is enum class?
    bool isEnumClass = matchIdentifier(("class"));
    symbol->extras.insert(std::make_pair("cxxclass", isEnumClass ? ("true") : ("false")));

    // parse name
    if (!getIdentifier(token)) {
        _error(("Expected enum name"));
        return false;
    }

    symbol->name = token.token;

    // ADD GENERATED TO MANUALLY
    symbol->extras.insert(std::make_pair(GENERATED, std::to_string(token.line + 1)));

    // skip until block
    while (true) {
        if (!getNextToken(token)) {
            _error(("Expected enum content"));
        }

        // declare forward with qenum
        if (token.token == (";")) {
            _error(("Forward declaration with QENUM macro"));
            return false;
        }

        if (token.token == ("{")) {
            break;
        }
    }

    if (!matchSymbol(("}"))) {
        do {
            if (!enumEntry(token)) {
                return false;
            }
        } while (matchSymbol((",")) && !matchSymbol(("}"), true));
    }

    // invalid end of block
    if (!requireSymbol(("}"))) {
        _error(("Invalid end of enum compound"));
        return false;
    }

    requireSymbol((";"));
    return true;
}

bool SymbolParser::enumEntry(Token &token) {
    if (!getIdentifier(token)) {
        return false;
    }

    // add symbol symbol
    auto &children = mSymbols.back()->children;
    children.push_back(new Symbol());
    auto symbol = children.back();
    symbol->name = token.token;

    // symbol initializer
    if (matchSymbol(("="))) {
        while (getNextToken(token) && token.token != mOptions.enumEntryNameMacro &&
               (token.type != ETokenType::Symbol || (token.token != (",") && token.token != ("}")))) {
        }

        undo(token);
    }

    meta(token, mOptions.enumEntryNameMacro, symbol);
    return true;
}

bool SymbolParser::struct_(Token &token) {
    // add empty symbol
    mSymbols.push_back(new Symbol());
    auto symbol = mSymbols.back();

    // parse meta
    if (!meta(token, mOptions.structNameMacro, symbol)) {
        delete mSymbols.back();
        mSymbols.pop_back();

        return false;
    }

    // skip keyword
    if (!requireIdentifier(("struct"))) {
        _error(("Missing identifier struct"));
        return false;
    }

    // skip api
    matchIdentifier(mOptions.apiMacro.data());

    // parse name
    if (!getIdentifier(token)) {
        _error(("Missing identifier"));
        return false;
    }
    symbol->name = token.token;

    // skip until block
    while (true) {
        if (!getNextToken(token)) {
            _error(("Expected struct content"));
        }

        // declare forward with qstruct
        if (token.token == (";")) {
            _error(("Forward declaration with Struct macro"));
            return false;
        }

        if (token.token == ("{")) {
            break;
        }
    }

    while (true) {
        if (!getNextToken(token)) {
            _error(("Invalid token"));
            return false;
        }

        if (token.token == mOptions.propertyNameMacro) {
            undo(token);
            property(token);
        } else if (token.token == mOptions.generatedMacro) {
            symbol->extras.insert(std::make_pair(GENERATED, std::to_string(token.line + 1)));
        } else if (token.token == ("}")) {
            undo(token);
            break;
        }
    }

    // invalid end of block
    if (!requireSymbol(("}"))) {
        _error(("Invalid end of enum compound"));
        return false;
    }

    requireSymbol((";"));
    return true;
}

bool SymbolParser::class_(Token &token) {
    // add empty symbol
    mSymbols.push_back(new Symbol());
    auto symbol = mSymbols.back();

    // parse meta
    if (!meta(token, mOptions.classNameMacro, symbol)) {
        delete mSymbols.back();
        mSymbols.pop_back();
        return false;
    }

    // skip keyword
    if (!requireIdentifier(("class"))) {
        _error(("Missing identifier struct"));
        return false;
    }

    // skip api
    matchIdentifier(mOptions.apiMacro.data());

    // parse name
    if (!getIdentifier(token)) {
        _error(("Missing identifier"));
        return false;
    }
    symbol->name = token.token;

    // skip until block
    while (true) {
        if (!getNextToken(token)) {
            _error(("Expected struct content"));
        }

        // declare forward with qstruct
        if (token.token == (";")) {
            _error(("Forward declaration with Struct macro"));
            return false;
        }

        if (token.token == ("{")) {
            break;
        }
    }

    size_t count = 1;
    while (true) {
        if (!getNextToken(token)) {
            _error(("Invalid token"));
            return false;
        }

        if (token.token == mOptions.propertyNameMacro) {
            undo(token);
            property(token);
        } else if (token.token == mOptions.generatedMacro) {
            symbol->extras.insert(std::make_pair(GENERATED, std::to_string(token.line + 1)));
        } else if (token.token == ("{")) {
            count++;
        } else if (token.token == ("}")) {
            count--;
            if (count <= 0) {
                undo(token);
                break;
            }
        }
    }

    // invalid end of block
    if (!requireSymbol(("}"))) {
        _error(("Invalid end of enum compound"));
        return false;
    }

    requireSymbol((";"));
    return true;
}

bool SymbolParser::property(Token &token) {
    // add symbol symbol
    auto &children = mSymbols.back()->children;
    children.push_back(new Symbol());
    auto symbol = children.back();
    symbol->name = token.token;

    // parse meta
    if (!meta(token, mOptions.propertyNameMacro, symbol)) {
        return false;
    }

    // skip keyword
    matchIdentifier(("mutable"));
    matchIdentifier(("static"));

    // TODO: add more?

    // skip type
    if (!type()) {
        _error(("Invalid data type"));
        return false;
    }

    // parse name
    if (!getIdentifier(token)) {
        _error(("Missing variable name"));
        return false;
    }
    symbol->name = token.token;

    // skip array
    if (matchSymbol(("["))) {
        _error(("Array type is not support yet!"));
        return false;

        // TODO: array
        /*do {
            if (!type(token)) {
                return false;
            }
        } while (!matchSymbol(("]")));

        if (!requireSymbol(("]"))) {
            _error(("']' required"));
            return false;
        }*/
    }

    // skip initializer
    while (getNextToken(token)) {
        if (token.token == (";")) {
            break;
        }
    }

    return true;
}

bool SymbolParser::method(Token &token) {
    return false;
}

bool SymbolParser::meta(Token &token, std::string macro, Symbol *target) {
    if (!matchIdentifier(macro.data())) {
        return false;
    }

    if (!metaSequence(token, (""), target)) {
        return false;
    }

    return true;
}

bool SymbolParser::metaSequence(Token &token, std::string scope, Symbol *target) {
    if (!requireSymbol(("("))) {
        return false;
    }

    if (!matchSymbol((")"))) {
        do {
            Token keyToken;
            if (!getIdentifier(keyToken)) {
                _error(("Expected identifier in meta sequence"));
                return false;
            }

            const auto key = scope.empty() ? keyToken.token : format(("%ls.%ls"), scope.data(), keyToken.token.data());
            Token valueToken;

            if (matchSymbol(("="))) {
                if (matchSymbol(("("))) {
                    metaSequence(token, key, target);
                    continue;
                }

                if (!getNextToken(valueToken)) {
                    _error(("Invalid meta sequence syntax: %ls"), scope.data());
                    return false;
                }


                if (valueToken.type == ETokenType::Symbol) {
                    _error(("Invalid meta sequence syntax"));
                    return false;
                } else {
                    target->metas.insert(std::make_pair(key, valueToken.token));
                }
            } else {
                target->metas.insert(std::make_pair(key, ("true")));
            }
        } while (matchSymbol((",")));

        requireSymbol((")"));
    }

    return true;
}

bool SymbolParser::type() {
    Token token;

    // skip keyword
    matchIdentifier(("const"));
    matchIdentifier(("volatile"));
    matchIdentifier(("mutable"));

    // skip forward declaration
    matchIdentifier(("class"));
    matchIdentifier(("struct"));
    matchIdentifier(("typename"));

    bool first = true;
    while (true) {
        if (!matchSymbol(("::")) && !first) {
            break;
        }

        first = false;

        if (!getIdentifier(token)) {
            _error(("Expected identifier: %ls"), token.token.data());
            return false;
        }
    }

    if (matchIdentifier(("unsigned")) || matchIdentifier(("signed"))) {
        _error(("Invalid combination of type specifiers"));
        return false;
    }

    // skip template
    if (matchSymbol(("<"))) {
        do {
            if (!type()) {
                return false;
            }
        } while (matchSymbol((",")));

        if (!requireSymbol((">"))) {
            _error(("'>' required"));
            return false;
        }
    }

    // skip memory keyword
    matchSymbol(("&"));
    matchSymbol(("&&"));
    matchSymbol(("*"));

    // skip const
    matchSymbol(("const"));


    // skip function pointer
    if (matchSymbol(("("))) {
        if (!matchSymbol((")"))) {
            do {
                if (!type()) {
                    return false;
                }

                if (!getNextToken(token)) {
                    _error(("Unexpected end of file"));
                    return false;
                }

                if (token.type != ETokenType::Identifier) {
                    undo(token);
                }
            } while (matchSymbol((",")));

            if (!matchSymbol((")"))) {
                _error(("Invalid end of function pointer"));
                return false;
            }
        }
    }

    return true;
}
