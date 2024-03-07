%skeleton "lalr1.cc"
%require "2.5"
%defines
%define api.namespace {calc}
%define api.value.type variant
%define parser_class_name {Parser}

%code requires {
    #include <iostream>
    #include <memory>
    #include "ast.h"
    namespace calc {class Lexer;}
}

%parse-param {calc::Lexer& lexer} {std::shared_ptr<Ast>& result} {std::string& message}

%code {
    #include "lexer.h"
    #define yylex lexer.lex
}

%token END 0 "end of file"
%token ERROR
%token EOL "\n"

%token <int> NUM
%token <std::string> NAME

%token PLUS "+"
%token MINUS "-"
%token MUL "*"
%token DIV "/"
%token LPAR "("
%token RPAR ")"
%token ASSIGN "="
%token COMMA ","
%token COLON ":"
%token DEF "def"
%token DO "do"
%token DONE "done"
%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token OR "||"
%token AND "&&"
%token NOT "!"
%token EQUALS "=="
%token NOTEQ "!="
%token LESS "<"
%token LESSOREQ "<="
%token MORE ">"
%token MOREOREQ ">="
%token PRINT "print"

%type <std::shared_ptr<Ast>> expr stmt stmt_list node condition
%type <std::vector<std::string>> params
%type <std::vector<std::shared_ptr<Ast>>> args block

%precedence ":"
%precedence "else"

%left "||"
%left "&&"
%nonassoc "!"
%left "==" "!=" "<" "<=" ">" ">="
%left "+" "-"
%left "*" "/"
%nonassoc UMINUS

%%

input: node { result = $1; }

node: stmt_list { $$ = $1; }
    | "def" NAME "(" params ")" ":" stmt_list { $$ = new_definition($2, $4, $7); }

stmt_list: stmt "\n" { $$ = $1; }
    | "do" "\n" block "done" "\n" { $$ = new_block($3); }
    | "if" condition ":" stmt_list {$$ = new_if($2, $4); }
    | "if" condition ":" stmt_list "else" ":" stmt_list  {$$ = new_if_else($2, $4, $7); }
    | "while" condition ":" stmt_list {$$ = new_while($2, $4); }

block : stmt_list { $$ = std::vector<std::shared_ptr<Ast>>{$1}; }
    | block stmt_list { $1.push_back($2); $$ = std::move($1); }

stmt: expr { $$ = $1; }
    | NAME "=" stmt { $$ = new_assignment($1, $3); }
    | NAME "(" args ")" { $$ = new_call($1, $3); }
    | "print" "(" stmt ")" { $$ = new_print($3); }

params : NAME { $$ = std::vector<std::string>{$1}; }
    | params "," NAME { $1.push_back($3); $$ = std::move($1); }

args : expr { $$ = std::vector<std::shared_ptr<Ast>>{$1}; }
    | args "," expr { $1.push_back($3); $$ = std::move($1); }

condition: expr { $$ = $1; }
    | condition "||" condition { $$ = new_binary_cond(Opcode::or_, $1, $3); }
    | condition "&&" condition { $$ = new_binary_cond(Opcode::and_, $1, $3); }
    | condition "==" condition { $$ = new_binary_cond(Opcode::equals, $1, $3); }
    | condition "!=" condition { $$ = new_binary_cond(Opcode::noteq, $1, $3); }
    | condition "<" condition { $$ = new_binary_cond(Opcode::less, $1, $3); }
    | condition "<=" condition { $$ = new_binary_cond(Opcode::lessoreq, $1, $3); }
    | condition ">" condition { $$ = new_binary_cond(Opcode::more, $1, $3); }
    | condition ">=" condition { $$ = new_binary_cond(Opcode::moreoreq, $1, $3); }
    | "(" condition ")" { $$ = $2; }
    | "!" condition { $$ = new_unary_cond(Opcode::not_, $2); }


expr: NUM { $$ = new_number($1); }
    | expr "+" expr { $$ = new_binary(Opcode::plus, $1, $3); }
    | expr "-" expr { $$ = new_binary(Opcode::minus, $1, $3); }
    | expr "*" expr { $$ = new_binary(Opcode::mul, $1, $3); }
    | expr "/" expr { $$ = new_binary(Opcode::div, $1, $3); }
    | "(" expr ")" { $$ = $2; }
    | "-" %prec UMINUS expr { $$ = new_unary(Opcode::uminus, $2); }
    | NAME { $$ = new_variable($1); }
;

%%

void calc::Parser::error(const std::string& err)
{
    message = err;
}
