%option noyywrap
%option nounput
%option noinput

%{

#include <cstdlib>
#include <string>

#include "sysy.tab.hpp"

using namespace std;

%}

/* 空白符和注释 */
WhiteSpace [ \r\t\n]*
LineComment "//".*
LineComment2 "/*"([^*]|\*+[^*/])*"*"+"/"


/* 标识符 */
Identifier [a-zA-Z_][a-zA-Z0-9_]*

/* 整数字面量 */
Decimal [1-9][0-9]*
Octal   0[0-7]*
Hexadecimal 0[xX][a-fA-F0-9]+

RelOp   [<>]=?
EqOp    [!=]=

%%


{WhiteSpace} {}
{LineComment} {}  
{LineComment2} {}

"int" {return INT;}
"void" {return VOID;}
"return" {return RETURN;}
"const" {return CONST;}
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"break"         { return BREAK; }
"continue"         { return CONTINUE; }


"||"            { yylval.str_val = new string(yytext); return OR; }
"&&"            { yylval.str_val = new string(yytext); return AND; }

{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }

{RelOp}         { yylval.str_val = new string(yytext); return RELOP; }
{EqOp}          { yylval.str_val = new string(yytext); return EQOP; }

.               { return yytext[0]; }

%%