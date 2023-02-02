#include "Tokenzier.h"
#include "Misc/StringBuilder.h"

static const TCHAR EndOfFileChar = std::char_traits<TCHAR>::to_char_type(std::char_traits<TCHAR>::eof());

Tokenizer::Tokenizer() : mInputLength(0), mCursorPos(0), mCursorLine(0) {}

void Tokenizer::reset(FString input, size_t startLine) {
    mInput = input;
    mInputLength = input.length();
    mCursorPos = 0;
    mCursorLine = startLine;
    mPrevCursorPos = mCursorPos;
    mPrevCursorLine = mCursorLine;

    mCurrentChar = mInput[mCursorPos];
}

bool Tokenizer::getNextToken(FToken &token, bool bAngleBracketsForStrings, bool bSeparateBraces) {
    token.line = mCursorLine;
    token.pos = mCursorPos;
    token.token.clear();
    token.type = ETokenType::Unknown;

    while (mCurrentChar != EndOfFileChar) {
        if (mCurrentChar == '\n') {
            advance();
            continue;
        }

        if (isspace(mCurrentChar)) {
            skipWhiteSpace();
            continue;
        }

        if (mCurrentChar == '/') {
            skipComment();
            continue;
        }

        if (mCurrentChar == '#') {
            token.token.appendChar(mCurrentChar);
            advance();
            return true;
        }

        auto intp = std::char_traits<TCHAR>::to_char_type(peek());
        if (isdigit(mCurrentCharC) || ((mCurrentChar == '-' || mCurrentChar == '+') && isdigit(intp))) {
            return number(token);
        }

        if (mCurrentChar == '\"' || (bAngleBracketsForStrings && mCurrentChar == '<')) {
            return string(token, bAngleBracketsForStrings);
        }


        if (isalpha(mCurrentCharC) || mCurrentChar == '_') {
            return identifier(token);
        }

        token.token.appendChar(mCurrentChar);
        auto c = mCurrentChar;
        auto d = advance();
#define PAIR(cc, dd) (c == (cc) && d == (dd))
        if (PAIR('<', '<') || PAIR('-', '>') || PAIR('!', '=') || PAIR('<', '=') ||
            PAIR('>', '=') || PAIR('+', '+') || PAIR('-', '-') || PAIR('+', '=') ||
            PAIR('-', '=') || PAIR('*', '=') || PAIR('-', '=') || PAIR('^', '=') ||
            PAIR('&', '=') || PAIR('|', '=') || PAIR('~', '=') || PAIR('%', '=') ||
            PAIR('&', '&') || PAIR('|', '|') || PAIR('=', '=') || PAIR(':', ':') ||
            (!bSeparateBraces && PAIR('>', '>'))) {
#undef PAIR
            token.token.appendChar(d);

            return true;
        }

        token.type = ETokenType::Symbol;
        return true;
    }

    return false;
}

bool Tokenizer::getNumber(FToken &token) {
    if (!getNextToken(token)) {
        return false;
    }

    if (token.type == ETokenType::Literal && token.literal.type != ELiteralType::String) {
        return true;
    }

    undo(token);
    return false;
}

bool Tokenizer::getString(FToken &token) {
    if (!getNextToken(token)) {
        return false;
    }

    if (token.type == ETokenType::Literal && token.literal.type == ELiteralType::String) {
        return true;
    }

    undo(token);
    return false;
}

bool Tokenizer::getIdentifier(FToken &token) {
    if (!getNextToken(token)) {
        return false;
    }

    if (token.type == ETokenType::Identifier) {
        return true;
    }

    undo(token);
    return false;
}

void Tokenizer::skipWhiteSpace() {
    while (mCurrentChar != EndOfFileChar && mCurrentChar != '\n' &&
           (isspace(mCurrentChar) || iscntrl(mCurrentChar))) {
        advance();
    }
}

void Tokenizer::skipComment() {
    if (!mComment.text.empty()) {
        mLastComment = mComment;
    }

    mComment.text = "";
    mComment.startLine = mCursorLine;
    mComment.endLine = mCursorLine;

    TCHAR nextChar = peek();

    TArray<FString> lines;
    if (mCurrentChar == '/' && nextChar == '/') {
        // Single Line Comment

        size_t indentationLastLine = 0;
        while (!isEoF() && mCurrentChar == '/' && nextChar == '/') {
            FString line = TEXT("");

            for (advance(); mCurrentChar != EndOfFileChar && mCurrentChar != '\n'; advance()) {
                line.appendChar(mCurrentChar);
            }

            int32_t lastSlashIndex = line.findChar('/');
            if (lastSlashIndex == INDEX_NONE) {
                line = "";
            } else {
                line = line.left(lastSlashIndex);
            }

            int32_t firstCharIndex = line.find(" \t");
            if (firstCharIndex == INDEX_NONE) {
                line = "";
            } else {
                line = line.left(firstCharIndex);
            }

            if (firstCharIndex > indentationLastLine && !lines.empty()) {
                lines.top() += TEXT(" ") + line;
            } else {
                lines.add(std::move(line));
                indentationLastLine = firstCharIndex;
            }

            skipWhiteSpace();

            if (!isEoF()) {
                nextChar = peek();
            }
        }
    } else if (mCurrentChar == '/' && nextChar == '*') {
        // MultiLine Block
        FString line;
        while (mCurrentChar != EndOfFileChar && (mCurrentChar != '*' || nextChar != '/')) {
            advance();
            nextChar = peek();

            if (line.empty()) {
                skipWhiteSpace();
            }

            if (mCurrentChar == '\n') {
                if (!lines.empty() || !line.empty()) {
                    lines.add(line);
                }

                line.clear();
            } else {
                if (!line.empty() || !(isspace(mCurrentChar) || mCurrentChar == '*')) {
                    line.appendChar(mCurrentChar);
                }
            }

            while (!lines.empty() && lines.top().empty()) {
                lines.pop();
            }
        }

        // Remove */
        advance();
        advance();
    }

    FStringBuilder ss(lines.length() * 2);
    for (size_t i = 0; i < lines.length(); ++i) {
        if (i > 0) {
            ss << "\n";
        }

        ss << lines[i];
    }

    mComment.text = ss.toString();
    mComment.endLine = mCursorLine;
}

bool Tokenizer::number(FToken &token) {
    auto intp = std::char_traits<TCHAR>::to_char_type(peek());
    if (isdigit(mCurrentCharC) || ((mCurrentChar == '-' || mCurrentChar == '+') && isdigit(intp))) {
        bool isFloat = false;
        bool isDouble = true;
        bool isHex = false;
        bool isNegated = mCurrentChar == '-';

        if (mCurrentChar == '-' || mCurrentChar == '+') {
            advance();
        }

        while (isdigit(mCurrentChar) || (!isFloat && mCurrentChar == '.') ||
               (!isHex && (mCurrentChar == 'X' || mCurrentChar == 'x')) ||
               (isHex && isxdigit(mCurrentCharC))) {
            if (mCurrentChar == '.') {
                isFloat = true;
            }

            if (mCurrentChar == 'x' || mCurrentChar == 'X') {
                isHex = true;
            }

            token.token.appendChar(mCurrentChar);
            advance();
        }

        if (isFloat && (mCurrentChar == 'f' || mCurrentChar == 'F')) {
            isDouble = false;
        }

        token.type = ETokenType::Literal;
        if (!isFloat) {
            try {
                if (token.token.length() == 1) {
                    if (isNegated) {
                        token.literal.i8 = token.token[0] - '0';
                        token.literal.type = ELiteralType::Int8;
                    } else {
                        token.literal.u8 = token.token[0] - '0';
                        token.literal.type = ELiteralType::UInt8;
                    }
                } else if (token.token.length() > 1){
                    if (isNegated) {
                        token.literal.i32 = std::stoi(*token.token, 0, 0);
                        token.literal.type = ELiteralType::Int32;
                    } else {
                        token.literal.u32 = std::stoul(*token.token, 0, 0);
                        token.literal.type = ELiteralType::UInt32;
                    }
                } else {
                    _error(TEXT("Invalid number"));
                    return false;
                }
            } catch (std::out_of_range) {
                if (isNegated) {
                    token.literal.i64 = std::stoll(*token.token, 0, 0);
                    token.literal.type = ELiteralType::Int64;
                } else {
                    token.literal.u64 = std::stoull(*token.token, 0, 0);
                    token.literal.type = ELiteralType::UInt64;
                }
            }
        } else {
            if (isDouble) {
                token.literal.d = std::stod(*token.token);
                token.literal.type = ELiteralType::Double;
            } else {
                token.literal.f = std::stof(*token.token);
                token.literal.type = ELiteralType::Float;
            }
        }

        return true;
    }

    return false;
}

bool Tokenizer::string(FToken &token, bool bAngleBracketsForStrings) {
    if (mCurrentChar == '\"' || (bAngleBracketsForStrings && mCurrentChar == '<')) {
        const char closingElement = mCurrentChar == '"' ? '"' : '>';

        advance();
        while (mCurrentChar != closingElement && std::char_traits<TCHAR>::not_eof(mCurrentCharC)) {
            if (mCurrentChar == '\\') {
                advance();

                if (!std::char_traits<TCHAR>::not_eof(mCurrentCharC)) {
                    break;
                } else if (mCurrentChar == 'n') {
                    mCurrentChar = '\n';
                } else if (mCurrentChar == 't') {
                    mCurrentChar = '\t';
                } else if (mCurrentChar == 'r') {
                    mCurrentChar = '\r';
                } else if (mCurrentChar == '"') {
                    mCurrentChar = '"';
                }
            }

            token.token.appendChar(mCurrentChar);
            advance();
        }

        if (mCurrentChar != closingElement) {
            undo();
        } else {
            advance();
        }

        token.type = ETokenType::Literal;
        token.literal.type = ELiteralType::String;
        token.literal.s = token.token;

        return true;
    }

    return false;
}

bool Tokenizer::identifier(FToken &token) {
    while (mCurrentChar != EndOfFileChar && (isalnum(mCurrentCharC) || mCurrentChar == '_')) {
        token.token.appendChar(mCurrentChar);
        advance();
    }

    // specific literal string
    if (token.token == "true") {
        token.type = ETokenType::Literal;
        token.literal.type = ELiteralType::Boolean;
        token.literal.b = true;
    } else if (token.token == "false") {
        token.type = ETokenType::Literal;
        token.literal.type = ELiteralType::Boolean;
        token.literal.b = false;
    } else {
        token.type = ETokenType::Identifier;
    }

    return true;
}

TCHAR Tokenizer::advance() {
    mPrevCursorPos = mCursorPos++;
    mPrevCursorLine = mCursorLine;
    if (mCurrentChar == '\n') {
        mCursorLine++;
    }

    if (mCursorPos >= mInputLength) {
        mCurrentChar = EndOfFileChar;
    } else {
        mCurrentChar = mInput[mCursorPos];
        mCurrentCharC = std::char_traits<TCHAR>::to_int_type(mCurrentChar);
    }

    return mCurrentChar;
}

void Tokenizer::undo() {
    mCursorLine = mPrevCursorLine;
    mCursorPos = mPrevCursorPos;

    mCurrentChar = mInput[mCursorPos];
    mCurrentCharC = std::char_traits<TCHAR>::to_int_type(mCurrentChar);
}

void Tokenizer::undo(const FToken &token) {
    mCursorLine = token.line;
    mCursorPos = token.pos;

    mCurrentChar = mInput[mCursorPos];
    mCurrentCharC = std::char_traits<TCHAR>::to_int_type(mCurrentChar);
}

TCHAR Tokenizer::peek(size_t level) {
    return !isEoF() ? mInput[mCursorPos + (level)] : EndOfFileChar;
}

bool Tokenizer::matchIdentifier(const TCHAR *identifier, bool undoIfMatch) {
    FToken token;
    if (getNextToken(token)) {
        if (token.type == ETokenType::Identifier && token.token == identifier) {
            if (undoIfMatch) {
                undo(token);
            }

            return true;
        }

        undo(token);
    }

    return false;
}

bool Tokenizer::matchSymbol(const TCHAR *symbol, bool undoIfMatch) {
    FToken token;
    if (getNextToken(token, false, std::char_traits<TCHAR>::length(symbol) == 1 && symbol[0] == TEXT('>'))) {
        if (token.type == ETokenType::Symbol && token.token == symbol) {
            if (undoIfMatch) {
                undo(token);
            }

            return true;
        }

        undo(token);
    }

    return false;
}

bool Tokenizer::requireIdentifier(const TCHAR *identifier) {
    if (!matchIdentifier(identifier)) {
        _error(TEXT("Missing identifier %s"), identifier);
        return false;
    }

    return true;
}

bool Tokenizer::requireSymbol(const TCHAR *symbol) {
    if (!matchSymbol(symbol)) {
        _error(TEXT("Missing symbol %s"), symbol);
        return false;
    }

    return true;
}

bool Tokenizer::skipBlock() {
    // block must start with {
    if (!matchSymbol(TEXT("{"))) {
        return false;
    }

    size_t remainingBracket = 1;
    FToken token;
    while (getNextToken(token)) {
        if (token.type == ETokenType::Symbol) {
            if (token.token == TEXT("{")) {
                ++remainingBracket;
            } else if (token.token == TEXT("}")) {
                --remainingBracket;
            }
        }

        if (remainingBracket <= 0) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::isEoF() {
    return mCursorPos >= mInputLength;
}

void Tokenizer::_error(const TCHAR *fmt, ...) {
    TCHAR buffer[512];

    va_list args;
    va_start(args, fmt);
    FCString::Vsprintf(buffer, 512, fmt, args);
    va_end(args);

    LOG(LogQHT, Error, TEXT("%ld:%ld: %ls"), mCursorLine, 0, buffer)
    bHasError = true;
}