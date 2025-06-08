#pragma once
#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>
#include "utils.h"

using namespace std;

// struct CompUnit
// {
//     FuncDef func_def;
// };

// struct FuncDef
// {
//     FuncType func_type;
//     std::string ident;
//     Block block;
// };


class BaseAST
{
    public:
        virtual ~BaseAST() = default;
        virtual void Dump() const = 0;
        virtual void* Koop() const = 0;
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
        std::unique_ptr<BaseAST> number;

        void Dump() const override {
            std::cout << "\tret ";
            number->Dump();
            std::cout << "\n";
        }
        void* Koop() const override;
};

class NumberAST : public BaseAST {
    public:
        int num;

        void Dump() const override {
            std::cout << num;
        }
        void* Koop() const override;
};