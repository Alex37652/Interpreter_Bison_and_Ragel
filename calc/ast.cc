#include "ast.h"

#include <cassert>

namespace calc {

Ast::Ast(Type type)
    : type(type)
{ }


Number::Number(int value)
    : Ast(Type::number), value(value)
{ }

Binary::Binary(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast> rhs)
    : Ast(Type::binary), op(op), lhs(lhs), rhs(rhs)
{ }

Unary::Unary(Opcode op, std::shared_ptr<Ast> operand)
    : Ast(Type::unary), op(op), operand(operand)
{ }

Assignment::Assignment(std::string name, std::shared_ptr<Ast> expression) 
    : Ast(Type::assignment), name(name), expression(expression)
{ }

Variable::Variable(std::string name)
    : Ast(Type::variable), name(name)
{ }

Definition::Definition(std::string name, std::vector<std::string> params, std::shared_ptr<Ast> expression)
    : Ast(Type::definition), name(name), params(params), expression(expression)
{ }   

Call::Call(std::string name, std::vector<std::shared_ptr<Ast>> args)
    : Ast(Type::call), name(name), args(args)
{ }    

Block::Block(std::vector<std::shared_ptr<Ast>> lines)
    : Ast(Type::block), lines(lines)
{ }

BinaryCond::BinaryCond(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast> rhs)
    : Ast(Type::binary_cond), op(op), lhs(lhs), rhs(rhs)
{ }

UnaryCond::UnaryCond(Opcode op, std::shared_ptr<Ast> operand)
    : Ast(Type::unary_cond), op(op), operand(operand)
{ }

If::If(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body)
    : Ast(Type::if_), condition(condition), body(body)
{ }

IfElse::IfElse(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body, std::shared_ptr<Ast> else_body)
    : Ast(Type::if_else), condition(condition), body(body), else_body(else_body)
{ }

While::While(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body)
    : Ast(Type::while_), condition(condition), body(body)
{ }

Print::Print(std::shared_ptr<Ast> expression)
    : Ast(Type::print), expression(expression)
{ }

std::shared_ptr<Ast> new_number(int value)
{
    return std::make_shared<Number>(value);
}

std::shared_ptr<Ast> new_binary(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast> rhs)
{
    return std::make_shared<Binary>(op, lhs, rhs);
}

std::shared_ptr<Ast> new_unary(Opcode op, std::shared_ptr<Ast> operand)
{
    return std::make_shared<Unary>(op, operand);
}

std::shared_ptr<Ast> new_assignment(std::string name, std::shared_ptr<Ast> expression)
{
    return std::make_shared<Assignment>(name, expression);
}

std::shared_ptr<Ast> new_variable(std::string name)
{
    return std::make_shared<Variable>(name);
}

std::shared_ptr<Ast> new_definition(std::string name, std::vector<std::string> params, std::shared_ptr<Ast> expression) {
    return std::make_shared<Definition>(name, params, expression);
}

std::shared_ptr<Ast> new_call(std::string name, std::vector<std::shared_ptr<Ast>> args) {
    return std::make_shared<Call>(name, args);
}

std::shared_ptr<Ast> new_block(std::vector<std::shared_ptr<Ast>> lines) {
    return std::make_shared<Block>(lines);
}

std::shared_ptr<Ast> new_binary_cond(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast>rhs) {
    return std::make_shared<BinaryCond>(op, lhs, rhs);
}

std::shared_ptr<Ast> new_unary_cond(Opcode op, std::shared_ptr<Ast> operand) {
    return std::make_shared<UnaryCond>(op, operand);
}

std::shared_ptr<Ast> new_if(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body) {
    return std::make_shared<If>(condition, body);
}

std::shared_ptr<Ast> new_if_else(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body, std::shared_ptr<Ast> else_body) {
    return std::make_shared<IfElse>(condition, body, else_body);
}

std::shared_ptr<Ast> new_while(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body) {
    return std::make_shared<While>(condition, body);
}

std::shared_ptr<Ast> new_print(std::shared_ptr<Ast> expression) {
    return std::make_shared<Print>(expression);
}



std::string to_string(std::shared_ptr<Ast> ast)
{
    if (!ast) {
        return {};
    }

    switch (ast->type) {
        case Type::number:
            return std::to_string(std::static_pointer_cast<Number>(ast)->value);
        case Type::binary: {
            auto op = std::static_pointer_cast<Binary>(ast);
            auto get_op = [&] () -> std::string {
                switch (op->op) {
                    case Opcode::plus: return "+";
                    case Opcode::minus: return "-";
                    case Opcode::mul: return "*";
                    case Opcode::div: return "/";
                    default: assert(0);
                }
            };
            return "(" + to_string(op->lhs) + ") " + get_op() + " (" + to_string(op->rhs) + ")";
        }
        case Type::unary: {
            auto op = std::static_pointer_cast<Unary>(ast);
            auto get_op = [&] () -> std::string {
                switch (op->op) {
                    case Opcode::uminus: return "-";
                    default: assert(0);
                }
            };
            return get_op() + "(" + to_string(op->operand) + ")";
        }
        case Type::assignment: {
            auto op = std::static_pointer_cast<Assignment>(ast);
            return op->name;
        }
        default: {
            assert(0);
        }
    }
}

}
