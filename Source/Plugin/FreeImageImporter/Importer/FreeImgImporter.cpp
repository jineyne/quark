//
// Created by jiney on 2023-03-28.
//

#include "FreeImgImporter.h"
#include "Importer/Importer.h"
#include "FileSystem/FileSystem.h"
#include "Image/PixelUtil.h"
#include "Image/Texture.h"
#include "Resource/Resources.h"
#include "Misc/StringBuilder.h"

void FreeImageLoadErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
    // Callback method as required by FreeImage to report problems
    const char* typeName = FreeImage_GetFormatFromFIF(fif);
    if (typeName) {
        LOG(FLogFreeImgImporter, Error, TEXT("FreeImage error: '%ls' when loading format %ls"),
            ANSI_TO_TCHAR(message), ANSI_TO_TCHAR(typeName));
    } else {
        LOG(FLogFreeImgImporter, Error, TEXT("FreeImage error: '%ls'"), ANSI_TO_TCHAR(message));
    }
}

FFreeImgImporter::FFreeImgImporter() {
    // Initialize FreeImage library
    FreeImage_Initialise();

    // Register codecs
    FStringBuilder strExt(512);
    strExt.setDynamic();

    strExt << TEXT("Supported formats: ");
    bool first = true;
    for (int i = 0; i < FreeImage_GetFIFCount(); ++i)
    {
        // Skip DDS codec since FreeImage does not have the option
        // to keep DXT data compressed, we'll use our own codec
        if ((FREE_IMAGE_FORMAT)i == FIF_DDS)
            continue;

        FString exts = FString(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i));
        if (!first)
            strExt << TEXT(",");

        first = false;
        strExt << exts;

        // Pull off individual formats (separated by comma by FI)
        TArray<FString> extsVector = exts.split(TEXT(","));
        for (auto v = extsVector.begin(); v != extsVector.end(); ++v)
        {
            auto findIter = std::find(mExtensions.begin(), mExtensions.end(), *v);

            if(findIter == mExtensions.end())
            {
                FString ext = *v;
                // mExtensionToFID.add(ext.lower(), i);
                mExtensions.add(ext);
            }
        }
    }

    // Set error handler
    FreeImage_SetOutputMessage(FreeImageLoadErrorHandler);
}

FFreeImgImporter::~FFreeImgImporter() {
    // Deinitialize FreeImage library
    FreeImage_DeInitialise();
}

bool FFreeImgImporter::isExtensionSupported(const FString &ext) const {
    // Check if FreeImage supports this file extension
    return mExtensions.contains(ext.lower());
}

bool FFreeImgImporter::isMagicNumberSupported(const uint8_t *magicNumPtr, uint32_t numBytes) const {
    // Check if FreeImage supports this magic number
    return true;
}

FResource *FFreeImgImporter::import(const FPath &path, const TSharedPtr<FImporterOptions> &options) {
    // Load the image using FreeImage
    if (!FFileSystem::Exists(path)) {
        LOG(FLogImporter, Error, TEXT("Unable to find texture: %ls"), *path.toString());
        return nullptr;
    }

    auto file = FFileSystem::OpenFile(path);
    auto size = file->size();
    TArray<uint8_t> fileData(size + 1);
    file->read(*fileData, size);
    fileData[file->size()] = TEXT('\0');

    FIMEMORY *fiMemory = FreeImage_OpenMemory(*fileData, size);
    if (fiMemory == nullptr) {
        return nullptr;
    }

    FREE_IMAGE_FORMAT fiFormat = FreeImage_GetFileTypeFromMemory(fiMemory);

    FIBITMAP *bitmap = FreeImage_LoadFromMemory(fiFormat, fiMemory);
    if (bitmap == nullptr) {
        LOG(FLogFreeImgImporter, Fatal, TEXT("Failed to load image"));
        return nullptr;
    }

    int width = FreeImage_GetWidth(bitmap);
    int height = FreeImage_GetHeight(bitmap);
    int bpp = FreeImage_GetBPP(bitmap);
    int pitch = FreeImage_GetPitch(bitmap);
    uint8_t *bits = (uint8_t *) FreeImage_GetBits(bitmap);

    int channels = bpp / 8;

    EPixelFormat format;
    switch (channels) {
        break;
        case 3: // 24 RGB format, 8 bits per channel
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
            format = EPixelFormat::RGB8;
#else
            format = EPixelFormat::BGR8;
#endif
            break;

        case 4: // 32 RGBA format, 8 bits per channel
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
            format = EPixelFormat::RGBA8;
#else
            format = EPixelFormat::BGRA8;
#endif
            break;

        default:
            format = EPixelFormat::Unknown;
            LOG(FLogImporter, Error, TEXT("Texture channels not support!"));
            return nullptr;
    }

    FTextureDesc desc{};
    desc.format = format;
    desc.type = ETextureType::e2D;
    desc.usage = ETextureUsage::Static;
    desc.width = width;
    desc.height = height;
    desc.depth = channels;

    auto texture = FTexture::NewPtr(desc);
    auto pixelData = FPixelData::New(width, height, channels, format);

    auto dstElemSize = FPixelUtil::GetNumElemBytes(format);
    auto dstPitch = width * FPixelUtil::GetNumElemBytes(format);
    auto output = pixelData->getData();

    uint8_t *pSrc;
    uint8_t *pDst = output;

    if (channels == dstElemSize) {
        for (uint32_t y = 0; y < height; ++y) {
            pSrc = bits + (height - y - 1) * pitch;
            memcpy(pDst, pSrc, dstPitch);
            pDst += dstPitch;
        }
    } else {
        for (uint32_t y = 0; y < height; ++y)
        {
            pSrc = bits + (height - y - 1) * pitch;

            for(uint32_t x = 0; x < width; ++x)
                memcpy(pDst + x * dstElemSize, pSrc + x * channels, channels);

            pDst += dstPitch;
        }
    }

    /*uint32_t x = 0, y = 0;
    for (uint32_t i = 0; i < size; i += channels) {
        auto color = FColor::Black;
        switch (channels) {
            break;
            case 3: // 24 RGB format, 8 bits per channel
                format = EPixelFormat::RGB8;
                color = FColor::FromRGBA(bits[i + 0], bits[i + 1], bits[i + 2], 255);
                break;

            case 4: // 32 RGBA format, 8 bits per channel
                format = EPixelFormat::RGBA8;
                color = FColor::FromRGBA(bits[i + 0], bits[i + 1], bits[i + 2], bits[i + 3]);
                break;

            default:
                format = EPixelFormat::Unknown;
                break;
        }

        pixelData->setColorAt(color, x, y);
        x++;
        if (x == width) {
            x = 0;
            y++;
        }
    }*/
    texture->writeData(pixelData);

    FreeImage_Unload(bitmap);
    FreeImage_CloseMemory(fiMemory);

    const FString fileName = path.getFilename();
    texture->setName(fileName);

    return texture;
}
