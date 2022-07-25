#pragma once

#include <string>
#include <vector>
#include "Lexema.hpp"

class Parser{
    public:
        Parser(const std::string &str);
        ~Parser() = default;
        const std::string           &getInput() { return input_; }
        const std::vector<Lexema>   &getLexems() { return lexems_; }
    private:
        std::vector<Lexema>     lexems_;
        std::string             input_;
};