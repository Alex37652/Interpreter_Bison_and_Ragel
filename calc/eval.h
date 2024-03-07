#pragma once

#include "ast.h"

#include <unordered_map>
#include <variant>

namespace calc {

class Eval
{
public:
    Eval() = default;

    Eval(std::unordered_map<std::string, std::shared_ptr<Ast>> var_to_val) : var_to_val(var_to_val)
    { }

    int eval(std::shared_ptr<Ast> stmt);

private:
    std::unordered_map<std::string, std::shared_ptr<Ast>> var_to_val;
};

}
