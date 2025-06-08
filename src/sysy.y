// insert to .tab.h file, used by sysy.l 
%code requires {
    #include <memory>
    #include <string>

    #include "ast.hpp"
}


// use to this file
%{
#include <iostream>
#include <memory>
#include <string>
#include "ast.hpp"

// .l file generate
int yylex(); 
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;


%}

%parse-param {std::unique_ptr<BaseAST> &ast}


%union {
    std::string *str_val;
    int int_val;
    BaseAST *ast_val;
}

// INT RETURN just token symbol, other which have <xxxx> means have value in yylval.xxxx 
// lexer 返回的所有 token 种类的声明

%token INT RETURN
%token <str_val> IDENT
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt Number

%%

// complier unit, ast = "FuncDef", FuncDef decompoised in later context
CompUnit
    : FuncDef {
        auto comp_unit = make_unique<CompUnitAST>();
        comp_unit->func_def = unique_ptr<BaseAST>($1);
        ast = move(comp_unit);
    }
    ;

// $(x) is xth after :
// $$ is return value assign to XXX left of :
FuncDef
    : FuncType IDENT '(' ')' Block {
        auto ast = new FuncDefAST();
        ast->func_type = unique_ptr<BaseAST>($1);
        ast->ident = *unique_ptr<string>($2);
        ast->block = unique_ptr<BaseAST>($5);
        $$ = ast;
    }
    ;

// only can be int
FuncType
    : INT {
        auto ast = new FuncTypeAST();
        ast->func_type = "int";
        $$ = ast;
    }
    ;


// IDENT is token not need rule

Block
    : '{' Stmt '}' {
        auto ast = new BlockAST();
        ast->stmt = unique_ptr<BaseAST>($2);
        $$ = ast;
    }
    ;

Stmt
    : RETURN Number ';' {
        auto ast = new StmtAST();
        ast->number = unique_ptr<BaseAST>($2);
        $$ = ast;
    }
    ;

Number
    : INT_CONST {
        auto ast = new NumberAST();
        ast->num = ($1);
        $$ = ast;
    }
    ;

%%

void yyerror(std::unique_ptr<BaseAST> &ast, const char* s) {
    cerr << "error:" << s << endl;
}

