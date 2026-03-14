#include "codegen.h"
#include <fstream>
#include <unordered_set>
#include <iostream>
using namespace std;

void CodeGen::emitC(const std::vector<IRInstr>& ir, const std::string& outFile) {
    std::ofstream out(outFile);
    out << "#include \"profiler_runtime.h\"\n#include <stdio.h>\n\n";
    out << "int main() {\n";

    std::unordered_set<std::string> declared;

    for (auto &i : ir) {

        if (i.op == "label") {
            out << "prof_hit(\"" << i.label << "\");\n";
            out << i.label << ":\n";
        }

        else if (i.op == "goto") {
            out << "prof_hit(\"" << i.res << "\");\n";
            out << "goto " << i.res << ";\n";
        }
        else if (i.op == "ifgoto") {
            out << "if (" << i.a1 << " == 0) goto " << i.res << ";\n";
        }

        else if (i.op == "assign") {
            if (!declared.count(i.res)) {
                out << "int " << i.res << " = " << i.a1 << ";\n";
                declared.insert(i.res);
            } else {
                out << i.res << " = " << i.a1 << ";\n";
            }
        }

        else if (i.op == "print") {
            out << "printf(\"%d\\n\", " << i.a1 << ");\n";
        }
        else { // Looks like a standard binary operation (math or comparison)!
            if (!declared.count(i.res)) {
                out << "int " << i.res << " = " << i.a1 << " " << i.op << " " << i.a2 << ";\n";
                declared.insert(i.res);
            } else {
                out << i.res << " = " << i.a1 << " " << i.op << " " << i.a2 << ";\n";
            }
        }
    }

    out << "prof_dump();\nreturn 0;\n}\n";
    out.close();
    std::cout << "Generated C code in " << outFile << std::endl;
}

