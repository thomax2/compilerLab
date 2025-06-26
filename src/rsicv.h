#include <cassert>
#include <fstream>
#include <map>
#include <iostream>
#include <assert.h>
#include "koopa.h"


class BaseRSICV
{
    class Env {
        std::map<koopa_raw_value_t, int> stack_offset_map;
        enum reg_status {UNUSED, USED};
        std::map<std::string, reg_status> reg_status_map;
        std::map<koopa_raw_value_t, std::string> reg_insts_map;
        public:
            int stack_size = 0;
            int cur_offset = 0;
            void stack_init(int size);
            int get_offset(koopa_raw_value_t value);
            void reg_init();
            std::string reg_alloc(koopa_raw_value_t value);
            void reg_free(std::string reg);
    };
    Env env;
    koopa_raw_value_t current_inst;
    std::string store_offset(int addr, std::string reg);
    std::string reg_load(koopa_raw_value_t value, std::string reg);

    private:
        std::ofstream out;
        std::string Visit(const koopa_raw_program_t &program);
        std::string Visit(const koopa_raw_slice_t &slice);
        std::string Visit(const koopa_raw_function_t &func);
        std::string Visit(const koopa_raw_basic_block_t &block);
        std::string Visit(const koopa_raw_value_t &value);
        std::string Visit(const koopa_raw_integer_t &interger);
        std::string Visit(const koopa_raw_return_t &ret);
        std::string Visit(const koopa_raw_binary_t &binary);
        std::string Visit(const koopa_raw_load_t &load);
        std::string Visit(const koopa_raw_store_t &store);
        std::string Visit(const koopa_raw_branch_t &branch);
        std::string Visit(const koopa_raw_jump_t &jump);


    public:
        BaseRSICV(const char *path) {
            out.open(path);
            assert(out.is_open());
        }
        void build(koopa_raw_program_t raw_prog);
        virtual ~BaseRSICV() = default;
        int func_size(const koopa_raw_function_t &func);
        int blk_size(koopa_raw_basic_block_t block);
        int inst_size(koopa_raw_value_t inst);
};