#include "QHTPrerequisites.h"

/*#include "Generator/ClangGenerator.h"
#include "Generator/GeneratorTool.h"
#include "Parser/SymbolParser.h"*/
#include "argparser.h"

struct Configuration {
    std::string annotationRequired;

    std::filesystem::path path;
    std::filesystem::path relativePath;
    std::string package;

    std::shared_ptr<std::ofstream> source;
    std::shared_ptr<std::ofstream> header;
};

static std::vector<std::string> includes = {};
static std::map<std::string, Configuration> configurations = {};

bool configurationFile(const std::filesystem::path &file, const std::string &package, std::filesystem::path &root, std::filesystem::path &output, const std::string &api) {
    std::stringstream sourcePath;
    std::stringstream headerPath;
    sourcePath << output.string() << "\\" << file.filename().replace_extension().string() << ".g.cpp";
    headerPath << output.string() << "\\" << file.filename().replace_extension().string() << ".g.h";
    // headerPath << output << file.filename() << ".g.h";

    Configuration configuration;
    configuration.annotationRequired = ("name");
    configuration.path = file;
    configuration.package = package;
    configuration.relativePath = std::filesystem::relative(file, root);
    configuration.source = std::make_shared<std::ofstream>();
    configuration.header = std::make_shared<std::ofstream>();
    configuration.source->open(sourcePath.str());
    configuration.header->open(headerPath.str());

    // CURRENT_FILE_ID
    std::string fileId = "";
    if (!package.empty()) {
        fileId += package + ("_");
    }
    fileId += ("Source_") + file.string() + ("_h");

    configurations.insert(std::make_pair(fileId, configuration));
    return true;
}

bool has_suffix(const std::string &str, const std::string &suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool configurationDirectory(const std::filesystem::path &path, const std::string &package, std::filesystem::path &root, std::filesystem::path &output, const std::string &api) {
    if (!std::filesystem::is_directory(path)) {
        return false;
    }

    for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!entry.is_directory()) {
            std::string filepath = entry.path().string();
            if (!has_suffix(filepath, ".h")) {
                continue;
            }

            configurationFile(filepath, package, root, output, api);
        }
    }

    return true;
}

/*bool generateFile(const std::filesystem::path &file, const std::string &package, std::filesystem::path &root, std::filesystem::path &output, const std::string &api) {
    std::ifstream stream;
    stream.open(file);
    stream.seekg(0, std::ios::end);
    int length = stream.tellg();

    std::vector<char> data(length + 1);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), length);
    data[length] = '\0';

    auto options = SymbolParser::Options();
    options.apiMacro = api + ("_EXPORT");
    SymbolParser sp = SymbolParser(options);
    auto symbols = sp.run(data.data());
    if (symbols == nullptr) {
        return false;
    }

    std::vector<std::string> flags;
    for (auto include : includes) {
        flags.push_back("-I" + include);
    }

    // add generated folder
    flags.push_back(std::string("-I") + output.string());

    // CURRENT_FILE_ID
    std::string fileId = "";

    // set GENERATE_BODY macro
    for (auto symbol : *symbols) {
        fileId = "";
        auto found = symbol->extras.find(GENERATED);
        if (found == symbol->extras.end()) {
            continue;
        }

        std::string generated = found->second;
        std::string generatedBody = "";
        if (!package.empty()) {
            fileId += package + "_";
        }
        fileId += std::string("Source_") + file.filename().string() + "_h";
        generatedBody = fileId + "_" + generated + "_GENERATED_BODY";

        *//*flags.add("-DCURRENT_FILE_ID=" + fileId);
        flags.add("-D" + generatedBody + "=");*//*
    }

    auto tool = new GeneratorTool(file.string(), flags);
    auto array = tool->buildAsts();
    if (array.empty()) {
        return false;
    }

    auto pctx = &(array[0]->getASTContext());
    auto tu = pctx->getTranslationUnitDecl();

    auto configuration = configurations.find(fileId);
    if (configuration == configurations.end()) {
        std::cout << "Unable to find configuration for " << fileId;
        return false;
    }

    auto generator = new ClangGenerator(configuration->second, *symbols);
    generator->generate(tu);

    return true;
}

bool generateDirectory(const std::filesystem::path &path, const std::string &package, std::filesystem::path &root, std::filesystem::path &output, const std::string &api) {
    if (!is_directory(path)) {
        return false;
    }

    for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
        if (!entry.is_directory()) {
            std::string filepath = entry.path().string();
            if (!has_suffix(filepath, ".h")) {
                continue;
            }

            generateFile(filepath, package, root, output, api);
        }
    }

    return true;
}*/

int main(int argc, char **argv) {
    argparse::ArgumentParser argumentParser("QuarkHeaderTool");
    argumentParser.add_argument("path");
    argumentParser.add_argument("source");
    argumentParser.add_argument("output");
    argumentParser.add_argument("--absolute").default_value(false).implicit_value(true);
    argumentParser.add_argument("--package").default_value(std::string(""));
    argumentParser.add_argument("--api").default_value(std::string("DLL"));
    argumentParser.add_argument("-I", "--include").default_value<std::vector<std::string>>({}).append();

    try {
        argumentParser.parse_args(argc, argv);
    } catch (std::exception e) {
        std::cerr << e.what();
        return EXIT_FAILURE;
    }

    auto rawPath = argumentParser.get<std::string>("path");
    auto rawInput = argumentParser.get<std::string>("source");
    auto rawOutput = argumentParser.get<std::string>("output");
    auto isAbsolute = argumentParser.get<bool>("absolute");
    auto rawPackage = argumentParser.get<std::string>("package");
    auto rawAPI = argumentParser.get<std::string>("api");
    auto rawIncludes = argumentParser.get<std::vector<std::string>>("include");

    auto path =  std::filesystem::path(rawPath);
    auto input = std::filesystem::path(rawInput);
    auto output = std::filesystem::path(rawOutput);
    auto package = rawPackage.empty() ? path.filename().string() : rawPackage;
    auto api = rawAPI.c_str();

    for (auto include : rawIncludes) {
        includes.push_back(include);
    }

    if (!isAbsolute) {
        input = path.append(rawInput);
        output = path.append(rawOutput);
    }

    if (!std::filesystem::exists(input)) {
        // EXCEPT(LogQHT, InvalidParametersException, ("given input file is not exists"));
        std::cerr << "given input file is not exists: " << input << std::endl;
        return EXIT_FAILURE;
    }

    // configuration inputs
    if (std::filesystem::is_directory(input)) {
        if (!configurationDirectory(input, package, path, output, api)) {
            return EXIT_FAILURE;
        }
    } else {
        configurationFile(input, package, path, output, api);
    }

    // generate inputs
    /*if (std::filesystem::is_directory(input)) {
        if (!generateDirectory(input, package, path, output, api)) {
            return EXIT_FAILURE;
        }
    } else {
        generateFile(input, package, path, output, api);
    }*/

    return 0;
}
