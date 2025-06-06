#include <iostream>
#include <cstdlib>
#include <memory>

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
};

class CompUnitAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_def;
    
        void Dump() const override {
            std::cout << "CompUnitAST { ";
            func_def->Dump();
            std::cout << " }";
        }
};


class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;

        void Dump() const override {
            std::cout << "FuncDefAST { ";
            func_type->Dump();
            std::cout << ", " << ident << ", ";
            block->Dump();
            std::cout << " }";
        }
};

class FuncTypeAST : public BaseAST {
    public:
        std::string func_type;
        
        void Dump() const override {
            std::cout << "FuncTypeAST { ";
            std::cout << func_type;
            std::cout << " }";
        }
};

class BlockAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> stmt;

        void Dump() const override {
            std::cout << "BlockAST { ";
            stmt->Dump();
            std::cout << " }";
        }
};

class StmtAST : public BaseAST {
    public:
        int number;

        void Dump() const override {
            std::cout << "StmtAST { ";
            std::cout << number;
            std::cout << " }";
        }
};

// class NumberAST : public BaseAST {
//     public:
//         int num;

//         void Dump() const override {
//             std::cout << num;
//         }
// };