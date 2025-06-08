#include <cassert>
#include <fstream>
#include "koopa.h"


class BaseRSICV
{
    private:
        std::ofstream out;
        void Visit(const koopa_raw_program_t &program);
        void Visit(const koopa_raw_slice_t &slice);
        void Visit(const koopa_raw_function_t &func);
        void Visit(const koopa_raw_basic_block_t &block);
        void Visit(const koopa_raw_value_t &value);
        void Visit(const koopa_raw_integer_t &interger);
        void Visit(const koopa_raw_return_t &ret);

    public:
        BaseRSICV(const char *path) {
            out.open(path);
            assert(out.is_open());
        }
        void build(koopa_raw_program_t raw_prog);
        virtual ~BaseRSICV() = default;
};