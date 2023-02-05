#pragma once

#include "QHTPrerequisites.h"

#include "Token.h"

class Tokenizer {
protected:
    struct FComment {
        FString text;
        size_t startLine;
        size_t endLine;
    };

    FString mInput;

    size_t mInputLength;
    size_t mCursorPos;
    size_t mCursorLine;
    size_t mPrevCursorPos;
    size_t mPrevCursorLine;

    FComment mComment;
    FComment mLastComment;

    TCHAR mCurrentChar;
    std::char_traits<TCHAR>::int_type mCurrentCharC;

    bool bHasError = false;

public:
    Tokenizer();

    virtual ~Tokenizer() = default;

public:
    void reset(FString input, size_t startLine = 1);

    bool getNextToken(FToken &token, bool bAngleBracketsForStrings = false, bool bSeparateBraces = false);

    bool getNumber(FToken &token);

    bool getString(FToken &token);

    bool getIdentifier(FToken &token);

protected:
    void skipWhiteSpace();

    void skipComment();

    bool number(FToken &token);

    bool string(FToken &token, bool bAngleBracketsForStrings = false);

    bool identifier(FToken &token);

    TCHAR advance();

    void undo();

    void undo(const FToken &token);

    TCHAR peek(size_t level = 1);

    bool matchIdentifier(const TCHAR *identifier, bool undoIfMatch = false);

    bool matchSymbol(const TCHAR *symbol, bool undoIfMatch = false);

    bool requireIdentifier(const TCHAR *identifier);

    bool requireSymbol(const TCHAR *symbol);

    bool skipBlock();

    bool isEoF();

    void _error(const TCHAR *fmt, ...);

    bool hasError() const { return bHasError; }
};

#define error(...) (_error(__VA_ARGS__), nullptr)


