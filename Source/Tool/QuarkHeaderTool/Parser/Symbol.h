#pragma once

#include "QHTPrerequisites.h"

const static std::string GENERATED = "GENERATED";

struct Symbol {
public:
    std::string name = "";
    std::vector<Symbol *> children = {};

    std::map<std::string, std::string> metas = {};
    std::map<std::string, std::string> extras = {};

    bool marked = false;

public:
    ~Symbol();
};