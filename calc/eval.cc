#include "eval.h"

#include <iostream>
#include <cassert>

namespace calc {

int Eval::eval(std::shared_ptr<Ast> ast)
{
    if (!ast) {
        return 0;
    }

    switch (ast->type) {
        case Type::number:
            return std::static_pointer_cast<Number>(ast)->value;
        case Type::binary: {
            auto op = std::static_pointer_cast<Binary>(ast);
            int lhs = eval(op->lhs);
            int rhs = eval(op->rhs);
            auto get_op = [&] () -> int {
                switch (op->op) {
                    case Opcode::plus: return lhs + rhs;
                    case Opcode::minus: return lhs - rhs;
                    case Opcode::mul: return lhs * rhs;
                    case Opcode::div: return lhs / rhs;
                    default: assert(0);
                }
            };
            return get_op();
        }
        case Type::unary: {
            auto op = std::static_pointer_cast<Unary>(ast);
            auto get_op = [&] () -> int {
                switch (op->op) {
                    case Opcode::uminus: return -eval(op->operand);
                    default: assert(0);
                }
            };
            return get_op();
        }
        case Type::assignment: {
            auto op = std::static_pointer_cast<Assignment>(ast);
            std::string name = op->name;
            var_to_val[name] = new_number(eval(op->expression));
            return eval(var_to_val[name]);
        }
        case Type::variable: {
            auto op = std::static_pointer_cast<Variable>(ast);
            std::string name = op->name;
            return eval(var_to_val[name]);
        }
        case Type::definition: {
            auto op = std::static_pointer_cast<Definition>(ast);
            var_to_val[op->name] = ast;
            return 0;
        }
        case Type::call: {
            auto op = std::static_pointer_cast<Call>(ast);
            auto func = std::static_pointer_cast<Definition>(var_to_val[op->name]);
            auto params = func->params;
            auto args = op->args;
            std::unordered_map<std::string, std::shared_ptr<Ast>> mp;
            for (auto [key, value]: var_to_val) {
                mp[key] = value;
            }
            for (int i = 0; i < params.size(); ++i) {
                mp[params[i]] = args[i];
            }
            Eval ev(mp);
            return ev.eval(func->expression);
        }
        case Type::block: {
            auto op = std::static_pointer_cast<Block>(ast);
            auto lines = op->lines;
            for (int i = 0; i < lines.size() - 1; ++i) {
                eval(lines[i]);
            }
            return eval(lines[lines.size() - 1]);
        }
        case Type::binary_cond: {
            auto op = std::static_pointer_cast<BinaryCond>(ast);
            int lhs = eval(op->lhs);
            int rhs = eval(op->rhs);
            auto get_op = [&] () -> int {
                switch (op->op) {
                    case Opcode::or_: return lhs || rhs;
                    case Opcode::and_: return lhs && rhs;
                    case Opcode::equals: return lhs == rhs;
                    case Opcode::noteq: return lhs != rhs;
                    case Opcode::less: return lhs < rhs;
                    case Opcode::lessoreq: return lhs <= rhs;
                    case Opcode::more: return lhs > rhs;
                    case Opcode::moreoreq: return lhs >= rhs;
                    default: assert(0);
                }
            };
            return get_op();
        }
        case Type::unary_cond: {
            auto op = std::static_pointer_cast<UnaryCond>(ast);
            auto get_op = [&] () -> int {
                switch (op->op) {
                    case Opcode::not_: return !eval(op->operand);
                    default: assert(0);
                }
            };
            return get_op();
        }
        case Type::if_: {
            auto op = std::static_pointer_cast<If>(ast);
            auto cond = op->condition;
            auto body = op->body;
            if (eval(cond)) {
                return eval(body);
            }
            return 0;
        }
        case Type::if_else: {
            auto op = std::static_pointer_cast<IfElse>(ast);
            auto cond = op->condition;
            auto body = op->body;
            auto else_body = op->else_body;
            if (eval(cond)) {
                return eval(body);
            } 
            return eval(else_body);
        }
        case Type::while_: {
            auto op = std::static_pointer_cast<If>(ast);
            auto cond = op->condition;
            auto body = op->body;
            int answer = 0;
            while (eval(cond)) {
                answer = eval(body);
            }
            return answer;
        }
        case Type::print: {
            auto op = std::static_pointer_cast<Print>(ast);
            std::cout<<eval(op->expression)<<std::endl;
            return 0;
        }
        default: {
            assert(0);
        }
    }
}

}
