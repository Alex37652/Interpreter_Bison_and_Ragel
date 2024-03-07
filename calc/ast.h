#pragma once

#include <string>
#include <memory>
#include <vector>

namespace calc {

enum class Type {
    number,
    binary,
    unary,
    assignment,
    variable,
    definition,
    call,
    block,
    binary_cond,
    unary_cond,
    if_,
    if_else,
    while_,
    print,


};

enum class Opcode {
    plus,
    minus,
    mul,
    div,
    uminus,
    or_,
    and_,
    not_,
    equals,
    noteq,
    less,
    lessoreq,
    more,
    moreoreq,

};

struct Ast
{
    Type type;

    Ast(Type type);
    virtual ~Ast() = default;
};

struct Number
    : public Ast
{
    int value;

    explicit Number(int value);
};

struct Binary
    : public Ast
{
    Opcode op;
    std::shared_ptr<Ast> lhs;
    std::shared_ptr<Ast> rhs;

    Binary(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast> rhs);
};

struct Unary
    : public Ast
{
    Opcode op;
    std::shared_ptr<Ast> operand;

    Unary(Opcode op, std::shared_ptr<Ast> operand);
};

struct Assignment
    : public Ast
{
    std::string name;
    std::shared_ptr<Ast> expression;

    Assignment(std::string name, std::shared_ptr<Ast> expression);
};

struct Variable
    : public Ast
{
    std::string name;

    explicit Variable(std::string name);
};

struct Definition
    : public Ast
{
    std::string name;
    std::vector<std::string> params;
    std::shared_ptr<Ast> expression;

    Definition(std::string name, std::vector<std::string> params, std::shared_ptr<Ast> expression);
};

struct Call
    : public Ast
{
    std::string name;
    std::vector<std::shared_ptr<Ast>> args;

    Call(std::string name, std::vector<std::shared_ptr<Ast>> args);
};

struct Block
    : public Ast
{
    std::vector<std::shared_ptr<Ast>> lines;

    Block(std::vector<std::shared_ptr<Ast>> lines);
};

struct BinaryCond
    : public Ast
{
    Opcode op;
    std::shared_ptr<Ast> lhs;
    std::shared_ptr<Ast> rhs;

    BinaryCond(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast> rhs);
};

struct UnaryCond
    : public Ast
{
    Opcode op;
    std::shared_ptr<Ast> operand;

    UnaryCond(Opcode op, std::shared_ptr<Ast> operand);
};

struct If
    : public Ast
{
    std::shared_ptr<Ast> condition;
    std::shared_ptr<Ast> body;

    If(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body);
};

struct IfElse
    : public Ast
{
    std::shared_ptr<Ast> condition;
    std::shared_ptr<Ast> body;
    std::shared_ptr<Ast> else_body;

    IfElse(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body, std::shared_ptr<Ast> else_body);
};

struct While
    : public Ast
{
    std::shared_ptr<Ast> condition;
    std::shared_ptr<Ast> body;

    While(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body);
};

struct Print
    : public Ast
{
    std::shared_ptr<Ast> expression;

    Print(std::shared_ptr<Ast> expression);
};

std::shared_ptr<Ast> new_number(int value);
std::shared_ptr<Ast> new_binary(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast>rhs);
std::shared_ptr<Ast> new_unary(Opcode op, std::shared_ptr<Ast> operand);
std::shared_ptr<Ast> new_assignment(std::string name, std::shared_ptr<Ast> expression);
std::shared_ptr<Ast> new_variable(std::string name);
std::shared_ptr<Ast> new_definition(std::string name, std::vector<std::string> params, std::shared_ptr<Ast> expression);
std::shared_ptr<Ast> new_call(std::string name, std::vector<std::shared_ptr<Ast>> args);
std::shared_ptr<Ast> new_block(std::vector<std::shared_ptr<Ast>> lines);
std::shared_ptr<Ast> new_binary_cond(Opcode op, std::shared_ptr<Ast> lhs, std::shared_ptr<Ast>rhs);
std::shared_ptr<Ast> new_unary_cond(Opcode op, std::shared_ptr<Ast> operand);
std::shared_ptr<Ast> new_if(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body);
std::shared_ptr<Ast> new_if_else(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body, std::shared_ptr<Ast> else_body);
std::shared_ptr<Ast> new_while(std::shared_ptr<Ast> condition, std::shared_ptr<Ast> body);
std::shared_ptr<Ast> new_print(std::shared_ptr<Ast> expression);

std::string to_string(std::shared_ptr<Ast> ast);

}
