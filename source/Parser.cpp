#include "Parser.hpp"

Parser::Parser(const std::string &str){
    input_.assign(str);
    std::size_t index = 0;
    while (str[index]){
        if (std::isspace(static_cast<unsigned char>(str[index])))
            ++index;
        else
            lexems_.push_back(Lexema(str, index));
    }
}