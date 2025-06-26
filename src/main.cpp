#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <streambuf>
#include "ast.hpp"
#include "koopa.h"
#include "rsicv.h"

using namespace std;

extern FILE *yyin;
extern int yyparse(std::unique_ptr<BaseAST> &ast);
extern int yydebug;
int main(int argc, const char *argv[]) {
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];
  // yydebug = 1;

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  assert(!ret);


  koopa_raw_program_t raw_prog = *(koopa_raw_program_t *)ast->Koop();
  ast.release();

  if( std::string(mode) == "-koopa") {
    koopa_program_t prog;
    koopa_error_code_t err = koopa_generate_raw_to_koopa(&raw_prog,&prog);
    std::cout << std::to_string(err) << std::endl;
    assert(err == KOOPA_EC_SUCCESS);

    koopa_dump_to_file(prog, output);
    koopa_delete_program(prog);
  } else {
    BaseRSICV riscv(output);
    riscv.build(raw_prog);
  }

  return 0;
}
