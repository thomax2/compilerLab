// insert to .tab.h file, used by sysy.l 
%debug
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
    std::vector<std::unique_ptr<BaseAST>> *ast_vec;
}

// INT RETURN just token symbol, other which have <xxxx> means have value in yylval.xxxx 
// lexer 返回的所有 token 种类的声明

%token INT RETURN CONST IF ELSE WHILE BREAK CONTINUE
%token <str_val> IDENT RELOP EQOP OR AND
%token <int_val> INT_CONST 

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt Number
%type <ast_val> Exp PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp
%type <ast_val> LVal Decl ConstDecl ConstDef ConstExp ConstInitVal BlockItem BType
%type <ast_val> VarDecl VarDef InitVal If While
%type <str_val> UnaryOp AddOp MulOp 
%type <ast_vec> BlockList ConstList VarList

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
    : '{' BlockList '}' {
        auto ast = new BlockAST();
        ast->blist = std::unique_ptr<std::vector<std::unique_ptr<BaseAST>>>($2);
        $$ = ast;
    } |
    '{' '}' {
        auto ast = new BlockAST();
        ast->blist = nullptr;
        $$ = ast;
    }
    ;

BlockList
    : BlockItem BlockList {
        auto blist = (vector<std::unique_ptr<BaseAST>>*)($2);
        auto bitem = std::unique_ptr<BaseAST>($1);
        blist->push_back(std::move(bitem));
        $$ = blist;
    } |
    BlockItem {
        auto vec = new vector<std::unique_ptr<BaseAST>> ();
        auto bitem = std::unique_ptr<BaseAST>($1);
        vec->push_back(std::move(bitem));
        $$ = vec;
    }
    ;

BlockItem : Decl | Stmt;


Stmt
    : RETURN Exp ';' {
        auto ast = new StmtAST();
        ast->type = StmtAST::RET;
        ast->expr = unique_ptr<BaseAST>($2);
        $$ = ast;
    } |
    LVal '=' Exp ';' {
        auto ast = new StmtAST();
        ast->type = StmtAST::VAL;
        ast->expr = unique_ptr<BaseAST>($3);
        ast->lval = unique_ptr<BaseAST>($1);
        $$ = ast;
    } | 
    Exp ';' {
        auto ast = new StmtAST();
        ast->expr = unique_ptr<BaseAST>($1);
        ast->type = StmtAST::EXP;
        $$ = ast;
    } |
    ';' {
        auto ast = new StmtAST();
        ast->expr = nullptr;
        $$ = ast;
    } |
    Block {
        auto ast = new StmtAST();
        ast->expr = unique_ptr<BaseAST>($1);
        ast->type = StmtAST::BLOCK;
        $$ = ast;
    } |
    RETURN ';' {
        auto ast = new StmtAST();
        ast->type = StmtAST::RET;
        ast->expr = nullptr;
        $$ = ast;
    } |
    If {
        auto ast = new StmtAST();
        ast->type = StmtAST::IF;
        ast->expr = unique_ptr<BaseAST>($1);
        ast->lval = nullptr;
        $$ = ast;
    } |
    If ELSE Stmt {
        auto ast = new StmtAST();
        ast->type = StmtAST::IF;
        ast->expr = unique_ptr<BaseAST>($1);
        ast->lval = unique_ptr<BaseAST>($3);
        $$ = ast;
    } |
    While {
        auto ast = new StmtAST();
        ast->type = StmtAST::WHILE;
        ast->expr = unique_ptr<BaseAST>($1);
        ast->lval = nullptr;
        $$ = ast;
    } |
    BREAK {
        auto ast = new StmtAST();
        ast->type = StmtAST::BREAK;
        ast->expr = nullptr;
        ast->lval = nullptr;
        $$ = ast;
    } | 
    CONTINUE {
        auto ast = new StmtAST();
        ast->type = StmtAST::CONTINUE;
        ast->expr = nullptr;
        ast->lval = nullptr;
        $$ = ast;
    }
    ;

If
    : IF '(' Exp ')' Stmt {
        auto ast = new IfAST();
        ast->expr = unique_ptr<BaseAST>($3);
        ast->stmt = unique_ptr<BaseAST>($5);
        $$ = ast;
    }
    ;

While
    : WHILE '(' Exp ')' Stmt {
        auto ast = new WhileAST();
        ast->expr = unique_ptr<BaseAST>($3);
        ast->stmt = unique_ptr<BaseAST>($5);
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

Exp
    : LOrExp {
        auto ast = new ExpAST();
        ast->exp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;

PrimaryExp
    : '(' Exp ')' {
        auto ast = new PrimaryExpAST();
        ast->expr = unique_ptr<BaseAST>($2);
        $$ = ast;
    } | 
    LVal {
        auto ast = new PrimaryExpAST();
        ast->expr = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    Number {
        auto ast = new PrimaryExpAST();
        ast->expr = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;

LVal
    : IDENT {
        auto ast = new LValAST();
        ast->ident = *unique_ptr<string>($1);
        $$ = ast;
    }
    ;


UnaryExp
    : PrimaryExp {
        auto ast = new UnaryExpAST();
        ast->type = UnaryExpAST::EXP;
        ast->primExp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    UnaryOp UnaryExp {
        auto ast = new UnaryExpAST();
        ast->type = UnaryExpAST::OP;
        ast->uop = *unique_ptr<string>($1);
        ast->uexp = unique_ptr<BaseAST>($2);
        $$ = ast;
    }
    ;

UnaryOp
    : '+' {
        $$ = new std::string("+");
    } |
    '-' {
        $$ = new std::string("-");
    } |
    '!' {
        $$ = new std::string("!");
    }
    ;

MulExp
    : UnaryExp {
        auto ast = new MulExpAST();
        ast->type = MulExpAST::EXP;
        ast->uexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    MulExp MulOp UnaryExp {
        auto ast = new MulExpAST();
        ast->type = MulExpAST::OP;
        ast->uexp = unique_ptr<BaseAST>($3);
        ast->mop = *unique_ptr<string>($2);
        ast->mexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;

MulOp
    : '*' {
        $$ = new std::string("*");
    } |
    '/' {
        $$ = new std::string("/");
    } |
    '%' {
        $$ = new std::string("%");
    }
    ;

AddExp
    : MulExp {
        auto ast = new AddExpAST();
        ast->type = AddExpAST::EXP;
        ast->mexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    AddExp AddOp MulExp {
        auto ast = new AddExpAST();
        ast->type = AddExpAST::OP;
        ast->aexp = unique_ptr<BaseAST>($3);
        ast->aop = *unique_ptr<string>($2);
        ast->mexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;

AddOp
    : '+' {
        $$ = new std::string("+");
    } |
    '-' {
        $$ = new std::string("-");
    }
    ;

RelExp
    : AddExp {
        auto ast = new RelExpAST();
        ast->type = RelExpAST::EXP;
        ast->aexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    RelExp RELOP AddExp {
        auto ast = new RelExpAST();
        ast->type = RelExpAST::OP;
        ast->aexp = unique_ptr<BaseAST>($3);
        ast->rop = *unique_ptr<string>($2);
        ast->rexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;


EqExp
    : RelExp {
        auto ast = new EqExpAST();
        ast->type = EqExpAST::EXP;
        ast->rexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    EqExp EQOP RelExp {
        auto ast = new EqExpAST();
        ast->type = EqExpAST::OP;
        ast->rexp = unique_ptr<BaseAST>($3);
        ast->eop = *unique_ptr<string>($2);
        ast->eexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;

LAndExp
    : EqExp {
        auto ast = new LAndExpAST();
        ast->type = LAndExpAST::EXP;
        ast->eexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    LAndExp AND EqExp {
        auto ast = new LAndExpAST();
        ast->type = LAndExpAST::OP;
        ast->eexp = unique_ptr<BaseAST>($3);
        ast->laexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;

LOrExp
    : LAndExp {
        auto ast = new LOrExpAST();
        ast->type = LOrExpAST::EXP;
        ast->laexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    } |
    LOrExp OR LAndExp {
        auto ast = new LOrExpAST();
        ast->type = LOrExpAST::OP;
        ast->laexp = unique_ptr<BaseAST>($3);
        ast->loexp = unique_ptr<BaseAST>($1);
        $$ = ast;
    }
    ;


Decl : ConstDecl | VarDecl;

ConstDecl
    : CONST BType ConstList ';' {
        auto ast = new ConstDeclAST();
        ast->btype = unique_ptr<BaseAST>($2);
        ast->clist = unique_ptr<std::vector<std::unique_ptr<BaseAST>>>($3);
        $$ = ast;
    }
    ;


ConstList
    : ConstList ',' ConstDef {
        auto vec = (vector<std::unique_ptr<BaseAST>>*)($1);
        auto cdef = unique_ptr<BaseAST>($3);
        vec->push_back(std::move(cdef));
        $$ = vec;
    }
    | ConstDef {
        auto vec = new vector<std::unique_ptr<BaseAST>> ();
        auto cdef = unique_ptr<BaseAST>($1);
        vec->push_back(std::move(cdef));
        $$ = vec;
    }
    ;
    
BType
    : INT {
        auto ast = new BTypeAST();
        ast->btype = "int";
        $$ = ast;
    }
    ;

ConstDef
    : IDENT '=' ConstInitVal {
        auto ast = new ConstDefAST();
        ast->ident = *unique_ptr<string>($1);
        ast->cival = unique_ptr<BaseAST>($3);
        $$ = ast;
    }
    ;

ConstInitVal : ConstExp;

ConstExp : Exp;

VarDecl
    : BType VarList ';' {
        auto ast = new VarDeclAST();
        ast->btype = unique_ptr<BaseAST>($1);
        ast->vlist = unique_ptr<std::vector<std::unique_ptr<BaseAST>>>($2);
        $$ = ast;
    }
    ;

VarList
    : VarDef ',' VarList {
        auto vec = (vector<std::unique_ptr<BaseAST>>*)($3);
        auto vdef = unique_ptr<BaseAST>($1);
        vec->push_back(std::move(vdef));
        $$ = vec;
    } |
    VarDef {
        auto vec = new vector<std::unique_ptr<BaseAST>>;
        auto vdef = unique_ptr<BaseAST>($1);
        vec->push_back(std::move(vdef));
        $$ = vec;
    }
    ;

VarDef
    : IDENT {
        auto ast = new VarDefAST();
        ast->type = VarDefAST::IDE;
        ast->ident = *unique_ptr<string>($1);
        $$ = ast;
    } |
    IDENT '=' InitVal {
        auto ast = new VarDefAST();
        ast->type = VarDefAST::VAL;
        ast->ident = *unique_ptr<string>($1);
        ast->ival = unique_ptr<BaseAST>($3);
        $$ = ast;
    }
    ;

InitVal : Exp;


%%

void yyerror(std::unique_ptr<BaseAST> &ast, const char* s) {
    cerr << "error:" << s << endl;
}


