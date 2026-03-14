#pragma once
#include <string>

struct IRInstr {
    std::string op;
    std::string a1;
    std::string a2;
    std::string res;
    std::string label;   // NEW — jump target for branch/goto
};

