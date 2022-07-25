#include "Parser.hpp"
#include "Expression.hpp"
#include <iostream>

#define COMPUTER_USAGE "compute [-d] \"exprassion\" ..."

int main(int argc, char **argv){
    if (argc > 1) {
        std::string input(argv[1]);
        bool showDebug = false;
        if (std::string(argv[1]) == std::string("-d") && argc >= 3){
            input.assign(argv[2]);
            showDebug = true;
        }

        try {
            Parser pars(input.c_str());

           if (showDebug){
                bool isFirst = true;
                for (auto lexema : pars.getLexems()){
                    if (isFirst)
                        isFirst = false;
                    else
                        std::cout << "->";
                    std::cout << '[' << lexema << ']';
                }
                std::cout << std::endl;
           }

            Expression expr(pars.getLexems());
            expr.compute();
            std::vector<Equation> equations = expr.getAllResults();


           if (showDebug){
                bool isFirst = true;
                for (const auto& eq : equations)
                {
                    if (isFirst) {
                        isFirst = false;
                    } else {
                        std::cout << " = ";
                    }
                    std::cout << eq;
                }
                std::cout << std::endl;
           }


            for (std::size_t index = equations.size() - 1; index > 0; --index){
                for (std::size_t jndex = 0; jndex < equations.size() - 1; ++jndex){
                    equations[jndex] = equations[jndex] - equations[index];
                }
                equations.pop_back();
            }

            if (showDebug){
                for (const auto& eq : equations)
                    std::cout << eq << std::endl;
            }


            if (equations.size() != 1){
                std::cerr << "Incorrectly reduced inequalities!" << std::endl;
            } else {
                auto ans = equations[0].Solve(showDebug);
                if (ans.first == ans.second) {
                    std::cout << "Unique root: " << ans.first << std::endl;
                } else {
                    std::cout << "First root: " << ans.first << std::endl;
                    std::cout << "Second root: " << ans.second << std::endl;
                }
            }
        } catch (const std::logic_error& e){
            std::cerr << e.what() << std::endl;
        }
    } else {
        std::cerr << COMPUTER_USAGE << std::endl;
    }
    return 0;
}