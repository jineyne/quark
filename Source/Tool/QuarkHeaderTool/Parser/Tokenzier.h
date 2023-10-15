#pragma once

#include "QHTPrerequisites.h"

#include "Token.h"

class Tokenizer {
protected:
    struct Comment {
        std::string text;
        size_t startLine;
        size_t endLine;
    };

    std::string mInput;

    size_t mInputLength;
    size_t mCursorPos;
    size_t mCursorLine;
    size_t mPrevCursorPos;
    size_t mPrevCursorLine;

    Comment mComment;
    Comment mLastComment;

    char mCurrentChar;
    std::char_traits<char>::int_type mCurrentCharC;

    bool bHasError = false;

public:
    Tokenizer();

    virtual ~Tokenizer() = default;

public:
    void reset(std::string input, size_t startLine = 1);

    bool getNextToken(Token &token, bool bAngleBracketsForStrings = false, bool bSeparateBraces = false);

    bool getNumber(Token &token);

    bool getString(Token &token);

    bool getIdentifier(Token &token);

protected:
    void skipWhiteSpace();

    void skipComment();

    bool number(Token &token);

    bool string(Token &token, bool bAngleBracketsForStrings = false);

    bool identifier(Token &token);

    char advance();

    void undo();

    void undo(const Token &token);

    char peek(size_t level = 1);

    bool matchIdentifier(const char *identifier, bool undoIfMatch = false);

    bool matchSymbol(const char *symbol, bool undoIfMatch = false);

    bool requireIdentifier(const char *identifier);

    bool requireSymbol(const char *symbol);

    bool skipBlock();

    bool isEoF() const;

    void _error(const char *fmt, ...);

    bool hasError() const { return bHasError; }
};

#define error(...) (_error(__VA_ARGS__), nullptr)


