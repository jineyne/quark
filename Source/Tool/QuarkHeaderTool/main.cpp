#include "QHTPrerequisites.h"
#include <Exception/Exception.h>
#include <FileSystem/Path.h>
#include <FileSystem/FileSystem.h>
#include <ThirdParty/argparser.h>

#include "Generator/Generator.h"
#include "Parser/Parser.h"

DEFINE_LOG_CATEGORY(LogQHT)

bool generateFile(const FPath &file, const FString &package, FPath &root, FPath &output, const FString &api) {
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
    options.apiMacro = FString::Printf(TEXT("%ls_EXPORT"), *api);
    FParser parser = FParser(options);
    auto compound = parser.parse(source);

    FGenerator generator = FGenerator(desc);
    generator.generate(compound);

    return true;
}

bool generateFolder(const FPath &path, const FString &package, FPath &root, FPath &output, const FString &api) {
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

        generateFile(file, package, root, output, api);
    }

    for (const auto &directory : directories) {
        if (!generateFolder(directory, package, root, output, api)) {
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    /*TArray<FPath> sources = {
        TEXT("D:/Projects/Quark/Test/Utility/Serialization")
    };*/

    argparse::ArgumentParser argumentParser("QuarkHeaderTool");
    argumentParser.add_argument("path");
    argumentParser.add_argument("source");
    argumentParser.add_argument("output");
    argumentParser.add_argument("--absolute").default_value(false).implicit_value(true);
    argumentParser.add_argument("--package").default_value(std::string(""));
    argumentParser.add_argument("--api").default_value(std::string("DLL"));

    argumentParser.parse_args(argc, argv);

    auto rawPath = argumentParser.get<std::string>("path");
    auto rawInput = argumentParser.get<std::string>("source");
    auto rawOutput = argumentParser.get<std::string>("output");
    auto isAbsolute = argumentParser.get<bool>("absolute");
    auto rawPackage = argumentParser.get<std::string>("package");
    auto rawAPI = argumentParser.get<std::string>("api");

    auto path =  FPath(ANSI_TO_TCHAR(rawPath.c_str()));
    auto input = FPath(ANSI_TO_TCHAR(rawInput.c_str()));
    auto output = FPath(ANSI_TO_TCHAR(rawOutput.c_str()));
    auto package = rawPackage.empty() ? path.getFilename() : ANSI_TO_TCHAR(rawPackage.c_str());
    auto api = ANSI_TO_TCHAR(rawAPI.c_str());

    if (!isAbsolute) {
        input = FPath::Combine(path, input);
        output = FPath::Combine(path, output);
    }

    if (!FFileSystem::Exists(input)) {
        EXCEPT(LogQHT, InvalidParametersException, TEXT("given input file is not exists"));
    }

    if (FFileSystem::IsFile(input)) {
        generateFile(input, package, path, output, api);
    } else if (FFileSystem::IsDirectory(input)) {
        if (!generateFolder(input, package, input, output, api)) {
            return EXIT_FAILURE;
        }
    }

    return 0;
}