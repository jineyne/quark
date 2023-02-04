//
// Created by jiney on 2023-02-05.
//

#include "Exception.h"

#include "Misc/StringBuilder.h"

FException::FException(const FString &type, const FString &description, const FString &source)
        : mType(type), mDescription(description), mSource(source) {}

FException::FException(const FString &type, const FString &description, const FString &source,
                       const FString &file, uint32_t line)
        : mType(type), mDescription(description), mSource(source), mFile(file), mLine(line) {}

FException::FException(const FException &rhs)
        : mType(rhs.mType), mDescription(rhs.mDescription), mSource(rhs.mSource), mFile(rhs.mFile)
        , mLine(rhs.mLine) {}

void FException::operator=(const FException &rhs) {
    mType = rhs.mType;
    mDescription = rhs.mDescription;
    mSource = rhs.mSource;
    mFile = rhs.mFile;
    mLine = rhs.mLine;
}

const FString &FException::getFullDescription() const {
    if (mFullDescription.empty()) {
        FStringBuilder ss(512);
        ss << TEXT("Unhandled framework exception (") << mType << TEXT(") occurred: ")
           << mDescription << TEXT("\r\n")
           << TEXT("\tin :") << mSource << TEXT("\r\n");

        if (mLine > 0) {
            ss << TEXT("\tat: ") << mFile << TEXT(":") << mLine << TEXT("\r\n");
        }

        mFullDescription = ss.toString();
    }

    return mFullDescription;
}


NotImplementedException::NotImplementedException(const FString &description, const FString &source,
                                                 const FString &file, uint32_t line)
        : FException(TEXT("NotImplementedException"), description, source, file, line) {}

InvalidStateException::InvalidStateException(const FString &description, const FString &source, const FString &file,
                                             uint32_t line)
        : FException(TEXT("InvalidStateException"), description, source, file, line) {}

InvalidParametersException::InvalidParametersException(const FString &description, const FString &source,
                                                       const FString &file, uint32_t line)
        : FException(TEXT("InvalidParametersException"), description, source, file, line) {}

InvalidOperationException::InvalidOperationException(const FString &description, const FString &source,
                                                     const FString &file, uint32_t line)
        : FException(TEXT("InvalidOperationException"), description, source, file, line) {}

InternalErrorException::InternalErrorException(const FString &description, const FString &source, const FString &file,
                                               uint32_t line)
        : FException(TEXT("InternalErrorException"), description, source, file, line) {}

RenderAPIException::RenderAPIException(const FString &description, const FString &source, const FString &file,
                                       uint32_t line)
        : FException(TEXT("RenderAPIException"), description, source, file, line) {}