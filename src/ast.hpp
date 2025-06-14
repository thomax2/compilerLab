#pragma once
#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include <cassert>
#include "utils.h"

using namespace std;



class BaseAST
{
    public:
        virtual ~BaseAST() = default;
        virtual void Dump() const = 0;
        virtual void* Koop() const  {
            return nullptr;
        }
        virtual void* Koop(std::vector<const void*>& insts) const {
            return nullptr;
        }
        virtual void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const {
            return nullptr;
        }
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;
    
        // CompUnitAST(std::unique_ptr<BaseAST>& func_def);
        void Dump() const override {
            // std::cout << "CompUnitAST { ";
            func_def->Dump();
            // std::cout << " }";
        }
        void* Koop() const override;
};


class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;

        void Dump() const override {
            std::cout << "fun ";
            std::cout << "@" << ident << "(): ";
            func_type->Dump();
            std::cout << "{\n";
            block->Dump();
            std::cout << "}\n";
        }
        void* Koop() const override;
};

class FuncTypeAST : public BaseAST {
    public:
        std::string func_type;
        
        void Dump() const override {
            std::cout << "i32 ";
        }
        void* Koop() const override;
};

class BlockAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "\%entry:\n";
            stmt->Dump();
        }
        void* Koop() const override;
};

class StmtAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> expr;

        void Dump() const override {
            std::cout << "\tret ";
            expr->Dump();
            std::cout << "\n";
        }
        // void* Koop() const override;
        void* Koop(std::vector<const void*>& insts) const override;
};

class NumberAST : public BaseAST {
    public:
        int num;

        void Dump() const override {
            std::cout << num;
        }
        // void* Koop() const override;
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};


class ExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> exp;
        
        void Dump() const override{}
        // void* Koop() const override;
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};

class PrimaryExpAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> expr;
        
        void Dump() const override{}
        // void* Koop() const override;
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};

class UnaryExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;
        std::unique_ptr<BaseAST> primExp;
        std::string uop;
        std::unique_ptr<BaseAST> uexp;
        
        void Dump() const override{}
        // void* Koop() const override;
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};


// class UnaryOpAST : public BaseAST {
//     public:
//         std::unique_ptr<BaseAST> ;

//         void* Koop() const override;
// };


class MulExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;

        std::unique_ptr<BaseAST> mexp;
        std::string mop;
        std::unique_ptr<BaseAST> uexp;
        void Dump() const override{}
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};

class AddExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;

        std::unique_ptr<BaseAST> mexp;
        std::string aop;
        std::unique_ptr<BaseAST> aexp;

        void Dump() const override{}
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;

};

class RelExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;

        std::unique_ptr<BaseAST> aexp;
        std::string rop;
        std::unique_ptr<BaseAST> rexp;

        void Dump() const override{}
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};

class EqExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;

        std::unique_ptr<BaseAST> rexp;
        std::string eop;
        std::unique_ptr<BaseAST> eexp;

        void Dump() const override{}
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
};

class LAndExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;

        std::unique_ptr<BaseAST> laexp;
        std::unique_ptr<BaseAST> eexp;

        void Dump() const override{}
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
        // void* Int2bool(koopa_raw_slice_t used_by, std::vector<const void*>& insts, const std::unique_ptr<BaseAST> &exp) const;
};

class LOrExpAST : public BaseAST {
    public:
        enum {EXP, OP} type;

        std::unique_ptr<BaseAST> loexp;
        std::unique_ptr<BaseAST> laexp;

        void Dump() const override{}
        void* Koop(koopa_raw_slice_t used_by, std::vector<const void*>& insts) const override;
        // void* Int2bool(koopa_raw_slice_t used_by, std::vector<const void*>& insts, std::unique_ptr<BaseAST> exp) const;
};


