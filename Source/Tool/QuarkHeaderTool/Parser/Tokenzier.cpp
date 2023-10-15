#include "Tokenzier.h"

static const char EndOfFileChar = std::char_traits<char>::to_char_type(std::char_traits<char>::eof());

Tokenizer::Tokenizer() : mInputLength(0), mCursorPos(0), mCursorLine(0) {}

void Tokenizer::reset(std::string input, size_t startLine) {
    mInput = input;
    mInputLength = input.length();
    mCursorPos = 0;
    mCursorLine = startLine;
    mPrevCursorPos = mCursorPos;
    mPrevCursorLine = mCursorLine;

    mCurrentChar = mInput[mCursorPos];
}

bool Tokenizer::getNextToken(Token &token, bool bAngleBracketsForStrings, bool bSeparateBraces) {
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

        if (mCurrentChar == '/' && (peek() == '/' || peek() == '*')) {
            skipComment();
            continue;
        }

        if (mCurrentChar == '#') {
            token.token += mCurrentChar;
            advance();
            return true;
        }

        auto intp = std::char_traits<char>::to_char_type(peek());
        if (isdigit(mCurrentCharC) || ((mCurrentChar == '-' || mCurrentChar == '+') && isdigit(intp))) {
            return number(token);
        }

        if (mCurrentChar == '\"' || (bAngleBracketsForStrings && mCurrentChar == '<')) {
            return string(token, bAngleBracketsForStrings);
        }


        if (isalpha(mCurrentCharC) || mCurrentChar == '_') {
            return identifier(token);
        }

        token.token += mCurrentChar;
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
            token.token += d;

            return true;
        }

        token.type = ETokenType::Symbol;
        return true;
    }

    return false;
}

bool Tokenizer::getNumber(Token &token) {
    if (!getNextToken(token)) {
        return false;
    }

    if (token.type == ETokenType::Literal && token.literal.type != ELiteralType::String) {
        return true;
    }

    undo(token);
    return false;
}

bool Tokenizer::getString(Token &token) {
    if (!getNextToken(token)) {
        return false;
    }

    if (token.type == ETokenType::Literal && token.literal.type == ELiteralType::String) {
        return true;
    }

    undo(token);
    return false;
}

bool Tokenizer::getIdentifier(Token &token) {
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

    char nextChar = peek();

    std::vector<std::string> lines;
    if (mCurrentChar == '/' && nextChar == '/') {
        // Single Line Comment

        size_t indentationLastLine = 0;
        while (!isEoF() && mCurrentChar == '/' && nextChar == '/') {
            std::string line = ("");

            for (advance(); mCurrentChar != EndOfFileChar && mCurrentChar != '\n'; advance()) {
                line += mCurrentChar;
            }

            int32_t lastSlashIndex = line.find('/');
            if (lastSlashIndex == std::string::npos) {
                line = "";
            } else {
                line = line.substr(0, lastSlashIndex);
            }

            int32_t firstCharIndex = line.find(" \t");
            if (firstCharIndex == std::string::npos) {
                line = "";
            } else {
                line = line.substr(0, firstCharIndex);
            }

            if (firstCharIndex > indentationLastLine && !lines.empty()) {
                lines.back() += (" ") + line;
            } else {
                lines.push_back(std::move(line));
                indentationLastLine = firstCharIndex;
            }

            skipWhiteSpace();

            if (!isEoF()) {
                nextChar = peek();
            }
        }
    } else if (mCurrentChar == '/' && nextChar == '*') {
        // MultiLine Block
        std::string line;
        while (mCurrentChar != EndOfFileChar && (mCurrentChar != '*' || nextChar != '/')) {
            advance();
            nextChar = peek();

            if (line.empty()) {
                skipWhiteSpace();
            }

            if (mCurrentChar == '\n') {
                if (!lines.empty() || !line.empty()) {
                    lines.push_back(line);
                }

                line.clear();
            } else {
                if (!line.empty() || !(isspace(mCurrentChar) || mCurrentChar == '*')) {
                    line += (mCurrentChar);
                }
            }

            while (!lines.empty() && lines.back().empty()) {
                lines.pop_back();
            }
        }

        // Remove */
        advance();
        advance();
    }

    size_t length = 0;
    std::for_each(lines.begin(), lines.end(), [&](const std::string &x) { length += x.length(); });
    /*FStringBuilder ss(length * 2);
    for (size_t i = 0; i < lines.length(); ++i) {
        if (i > 0) {
            ss << "\n";
        }

        ss << lines[i];
    }*/

    // mComment. = ss.toString();
    mComment.endLine = mCursorLine;
}

bool Tokenizer::number(Token &token) {
    auto intp = std::char_traits<char>::to_char_type(peek());
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

            token.token += mCurrentChar;
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
                        token.literal.i32 = std::stoi(token.token.data(), 0, 0);
                        token.literal.type = ELiteralType::Int32;
                    } else {
                        token.literal.u32 = std::stoul(token.token.data(), 0, 0);
                        token.literal.type = ELiteralType::UInt32;
                    }
                } else {
                    _error(("Invalid number"));
                    return false;
                }
            } catch (std::out_of_range) {
                if (isNegated) {
                    token.literal.i64 = std::stoll(token.token.data(), 0, 0);
                    token.literal.type = ELiteralType::Int64;
                } else {
                    token.literal.u64 = std::stoull(token.token.data(), 0, 0);
                    token.literal.type = ELiteralType::UInt64;
                }
            }
        } else {
            if (isDouble) {
                token.literal.d = std::stod(token.token.data());
                token.literal.type = ELiteralType::Double;
            } else {
                token.literal.f = std::stof(token.token.data());
                token.literal.type = ELiteralType::Float;
            }
        }

        return true;
    }

    return false;
}

bool Tokenizer::string(Token &token, bool bAngleBracketsForStrings) {
    if (mCurrentChar == '\"' || (bAngleBracketsForStrings && mCurrentChar == '<')) {
        const char closingElement = mCurrentChar == '"' ? '"' : '>';

        advance();
        while (mCurrentChar != closingElement && std::char_traits<char>::not_eof(mCurrentCharC)) {
            if (mCurrentChar == '\\') {
                advance();

                if (!std::char_traits<char>::not_eof(mCurrentCharC)) {
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

            token.token += mCurrentChar;
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

bool Tokenizer::identifier(Token &token) {
    while (mCurrentChar != EndOfFileChar && (isalnum(mCurrentCharC) || mCurrentChar == '_')) {
        token.token += mCurrentChar;
        advance();
    }

    // specific literal std::string
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

char Tokenizer::advance() {
    mPrevCursorPos = mCursorPos++;
    mPrevCursorLine = mCursorLine;
    if (mCurrentChar == '\n') {
        mCursorLine++;
    }

    if (mCursorPos >= mInputLength) {
        mCurrentChar = EndOfFileChar;
    } else {
        mCurrentChar = mInput[mCursorPos];
        mCurrentCharC = std::char_traits<char>::to_int_type(mCurrentChar);
    }

    return mCurrentChar;
}

void Tokenizer::undo() {
    mCursorLine = mPrevCursorLine;
    mCursorPos = mPrevCursorPos;

    mCurrentChar = mInput[mCursorPos];
    mCurrentCharC = std::char_traits<char>::to_int_type(mCurrentChar);
}

void Tokenizer::undo(const Token &token) {
    mCursorLine = token.line;
    mCursorPos = token.pos;

    mCurrentChar = mInput[mCursorPos];
    mCurrentCharC = std::char_traits<char>::to_int_type(mCurrentChar);
}

char Tokenizer::peek(size_t level) {
    return !isEoF() ? mInput[mCursorPos + (level)] : EndOfFileChar;
}

bool Tokenizer::matchIdentifier(const char *identifier, bool undoIfMatch) {
    Token token;
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

bool Tokenizer::matchSymbol(const char *symbol, bool undoIfMatch) {
    Token token;
    if (getNextToken(token, false, std::char_traits<char>::length(symbol) == 1 && symbol[0] == ('>'))) {
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

bool Tokenizer::requireIdentifier(const char *identifier) {
    if (!matchIdentifier(identifier)) {
        _error(("Missing identifier %s"), identifier);
        return false;
    }

    return true;
}

bool Tokenizer::requireSymbol(const char *symbol) {
    if (!matchSymbol(symbol)) {
        _error(("Missing symbol %s"), symbol);
        return false;
    }

    return true;
}

bool Tokenizer::skipBlock() {
    // block must start with {
    if (!matchSymbol(("{"))) {
        return false;
    }

    size_t remainingBracket = 1;
    Token token;
    while (getNextToken(token)) {
        if (token.type == ETokenType::Symbol) {
            if (token.token == ("{")) {
                ++remainingBracket;
            } else if (token.token == ("}")) {
                --remainingBracket;
            }
        }

        if (remainingBracket <= 0) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::isEoF() const {
    return mCursorPos >= mInputLength;
}

void Tokenizer::_error(const char *fmt, ...) {
    char buffer[512];

    va_list args;
    va_start(args, fmt);
    vsprintf_s(buffer, 512, fmt, args);
    va_end(args);

    std::cout << buffer << std::endl;
    bHasError = true;
}