#include "QHTPrerequisites.h"
#include <Exception/Exception.h>
#include <FileSystem/Path.h>
#include <FileSystem/FileSystem.h>
#include <ThirdParty/argparser.h>

#include "Generator/ClangGenerator.h"
#include "Generator/GeneratorTool.h"
#include "Parser/SymbolParser.h"

DEFINE_LOG_CATEGORY(LogQHT)

static TArray<std::string> includes = {};
static TMap<String, ClangGenerator::Configuration> configurations = {};

bool configurationFile(const Path &file, const String &package, Path &root, Path &output, const String &api) {
    auto sourcePath = Path::Combine(output, file.getFilename() + ".g.cpp");
    auto headerPath = Path::Combine(output, file.getFilename() + ".g.h");

    ClangGenerator::Configuration configuration;
    configuration.annotationRequired = TEXT("name");
    configuration.path = file;
    configuration.package = package;
    configuration.relativePath = file;
    configuration.relativePath.makeRelative(root);
    configuration.source = FileSystem::CreateAndOpenFile(sourcePath);
    configuration.header = FileSystem::CreateAndOpenFile(headerPath);

    // CURRENT_FILE_ID
    String fileId = "";
    if (!package.empty()) {
        fileId += package + TEXT("_");
    }
    fileId += TEXT("Source_") + file.getFilename() + TEXT("_h");

    configurations.add(fileId, configuration);
    return true;
}

bool configurationDirectory(const Path &path, const String &package, Path &root, Path &output, const String &api) {
    if (!FileSystem::IsDirectory(path)) {
        return false;
    }

    TArray<Path> directories;
    TArray<Path> files;

    FileSystem::GetChildren(path, files, directories);
    for (const auto &file : files) {
        if (!file.toString().endWith(TEXT(".h"))) {
            continue;
        }

        configurationFile(file, package, root, output, api);
    }

    for (const auto &directory : directories) {
        if (!configurationDirectory(directory, package, root, output, api)) {
            return false;
        }
    }

    return true;
}

bool generateFile(const Path &file, const String &package, Path &root, Path &output, const String &api) {
    auto inputStream = FileSystem::OpenFile(file);
    TArray<ANSICHAR> inputData(inputStream->size() + 1);
    inputStream->read(inputData.getData(), inputStream->size());
    inputData[inputStream->size()] = '\0';

    auto options = SymbolParser::Options();
    options.apiMacro = api + TEXT("_EXPORT");
    SymbolParser sp = SymbolParser(options);
    auto symbols = sp.run(inputData.getData());
    if (symbols == nullptr) {
        return false;
    }

    TArray<std::string> flags;
    for (auto include : includes) {
        flags.add("-I" + include);
    }

    // add generated folder
    flags.add(std::string("-I") + TCHAR_TO_ANSI(*output.toString()));

    // CURRENT_FILE_ID
    std::string fileId = "";

    // set GENERATE_BODY macro
    for (auto symbol : *symbols) {
        fileId = "";
        auto found = symbol->extras.find(GENERATED);
        if (found == nullptr) {
            continue;
        }

        String generated = *found;
        std::string generatedBody = "";
        if (!package.empty()) {
            fileId += std::string(TCHAR_TO_ANSI(*package)) + "_";
        }
        fileId += std::string("Source_") + TCHAR_TO_ANSI(*file.getFilename()) + "_h";
        generatedBody = fileId + std::string("_") + TCHAR_TO_ANSI(*generated) + "_GENERATED_BODY";

        /*flags.add("-DCURRENT_FILE_ID=" + fileId);
        flags.add("-D" + generatedBody + "=");*/
    }

    auto tool = new GeneratorTool(file.toString(), flags);
    auto array = tool->buildAsts();
    if (array.empty()) {
        return false;
    }

    auto pctx = &(array[0]->getASTContext());
    auto tu = pctx->getTranslationUnitDecl();

    String _fileId = ANSI_TO_TCHAR(fileId.c_str());

    auto configuration = configurations.find(_fileId);
    if (configuration == nullptr) {
        LOG(LogQHT, Fatal, TEXT("Unable to find configuration for '%s'"), *_fileId);
        return false;
    }

    auto generator = new ClangGenerator(*configuration, *symbols);
    generator->generate(tu);

    return true;
}

bool generateDirectory(const Path &path, const String &package, Path &root, Path &output, const String &api) {
    if (!FileSystem::IsDirectory(path)) {
        return false;
    }

    TArray<Path> directories;
    TArray<Path> files;

    FileSystem::GetChildren(path, files, directories);
    for (const auto &file : files) {
        if (!file.toString().endWith(TEXT(".h"))) {
            continue;
        }

        generateFile(file, package, root, output, api);
    }

    for (const auto &directory : directories) {
        if (!generateDirectory(directory, package, root, output, api)) {
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    argparse::ArgumentParser argumentParser("QuarkHeaderTool");
    argumentParser.add_argument("path");
    argumentParser.add_argument("source");
    argumentParser.add_argument("--absolute").default_value(false).implicit_value(true);
    argumentParser.add_argument("--package").default_value(std::string(""));
    argumentParser.add_argument("--api").default_value(std::string("DLL"));
    argumentParser.add_argument("-I", "--include").default_value<std::vector<std::string>>({}).append();

    argumentParser.parse_args(argc, argv);

    auto rawPath = argumentParser.get<std::string>("path");
    auto rawInput = argumentParser.get<std::string>("source");
    auto isAbsolute = argumentParser.get<bool>("absolute");
    auto rawPackage = argumentParser.get<std::string>("package");
    auto rawAPI = argumentParser.get<std::string>("api");
    auto rawIncludes = argumentParser.get<std::vector<std::string>>("include");

    auto path =  Path(ANSI_TO_TCHAR(rawPath.c_str()));
    auto input = Path(ANSI_TO_TCHAR(rawInput.c_str()));
    auto output = Path::Combine(ANSI_TO_TCHAR(GENERATED_APP_ROOT), path.getDirectory(path.getDirectoryCount() - 1));
    auto package = rawPackage.empty() ? path.getFilename() : ANSI_TO_TCHAR(rawPackage.c_str());
    auto api = ANSI_TO_TCHAR(rawAPI.c_str());

    for (auto include : rawIncludes) {
        includes.add(include);
    }

    if (!isAbsolute) {
        input = Path::Combine(path, input);
        output = Path::Combine(path, output);
    }

    if (!FileSystem::Exists(input)) {
        EXCEPT(LogQHT, InvalidParametersException, TEXT("given input file is not exists"));
    }

    // configuration inputs
    if (FileSystem::IsFile(input)) {
        configurationFile(input, package, path, output, api);
    } else if (FileSystem::IsDirectory(input)) {
        if (!configurationDirectory(input, package, path, output, api)) {
            return EXIT_FAILURE;
        }
    }

    // generate inputs
    if (FileSystem::IsFile(input)) {
        generateFile(input, package, path, output, api);
    } else if (FileSystem::IsDirectory(input)) {
        if (!generateDirectory(input, package, path, output, api)) {
            return EXIT_FAILURE;
        }
    }

    return 0;
}