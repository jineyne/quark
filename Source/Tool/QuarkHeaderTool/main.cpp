#include "QHTPrerequisites.h"
#include "FileSystem/Path.h"
#include "FileSystem/FileSystem.h"
#include "Generator/Generator.h"
#include "Parser/Parser.h"

DEFINE_LOG_CATEGORY(LogQHT)

bool generateFile(const FPath &file, const FString &package, FPath &root, FPath &output) {
    auto sourcePath = FPath::Combine(output, file.getFilename() + ".g.cpp");
    auto headerPath = FPath::Combine(output, file.getFilename() + ".g.h");

    FGeneratorDesc desc{};
    desc.package = package;

    desc.source = FFileSystem::CreateAndOpenFile(sourcePath);
    desc.header = FFileSystem::CreateAndOpenFile(headerPath);

    desc.path = file;
    desc.relativePath = desc.path;
    desc.relativePath.makeRelative(root);

    auto inputStream = FFileSystem::OpenFile(desc.path);
    TArray<ANSICHAR> inputData(inputStream->size() + 1);
    inputStream->read(inputData.getData(), inputStream->size());
    inputData[inputStream->size()] = '\0';

    FString source = inputData.getData();

    FOptions options{};
    FParser parser = FParser(options);
    auto compound = parser.parse(source);

    FGenerator generator = FGenerator(desc);
    generator.generate(compound);

    return true;
}

bool generateFolder(const FPath &path, const FString &package, FPath &root, FPath &output) {
    if (!FFileSystem::IsDirectory(path)) {
        return false;
    }

    TArray<FPath> directories;
    TArray<FPath> files;

    FFileSystem::GetChildren(path, files, directories);
    for (const auto &file : files) {
        if (!file.toString().endWith(TEXT(".h"))) {
            continue;
        }

        generateFile(file, package, root, output);
    }

    for (const auto &directory : directories) {
        if (!generateFolder(path + directory, package, root, output)) {
            return false;
        }
    }

    return true;
}

int main() {
    TArray<FPath> sources = {
        TEXT("D:/Projects/Quark/Test/Utility/Serialization")
    };

    generateFile(sources[0] + TEXT("ArchiveTest.h"), FString::Empty, sources[0], sources[0]);
    generateFile(sources[0] + TEXT("BaseClass.h"), FString::Empty, sources[0], sources[0]);
    generateFile(sources[0] + TEXT("SaveData.h"), FString::Empty, sources[0], sources[0]);
    generateFile(sources[0] + TEXT("OtherActor.h"), FString::Empty, sources[0], sources[0]);

    return 0;
}