#include <LogicSymbols.hpp>
#include "SymbolTable.hpp"

namespace SynthLib2Parser {

    bool SortExprPtrEquals::operator () (const SortExpr* SortPtr1, const SortExpr* SortPtr2) const
    {
        return SortPtr1->Equals(*SortPtr2);
    }

    u32 SortExprPtrHash::operator () (const SortExpr* SortPtr) const
    {
        return SortPtr->Hash();
    }

    SortExprPtrSet LogicSymbolLoader::RegisteredTypes;
    bool LogicSymbolLoader::BVLoaded = false;

    void LogicSymbolLoader::LoadCore(SymbolTable* SymTab)
    {
        vector<const SortExpr*> UnOpVec(1);
        vector<const SortExpr*> BinOpVec(2);
        vector<const SortExpr*> ITEOpVec(3);
        UnOpVec[0] = new BoolSortExpr();
        BinOpVec[0] = new BoolSortExpr();
        BinOpVec[1] = new BoolSortExpr();
        ITEOpVec[0] = new BoolSortExpr();
        ITEOpVec[1] = new BoolSortExpr();
        ITEOpVec[2] = new BoolSortExpr();
        
        auto BS = new BoolSortExpr();

        // Load the usual boolean operators
        SymTab->BindTheoryFun("and", BinOpVec, BS);
        SymTab->BindTheoryFun("or", BinOpVec, BS);
        SymTab->BindTheoryFun("=>", BinOpVec, BS);
        SymTab->BindTheoryFun("not", UnOpVec, BS);
        SymTab->BindTheoryFun("xor", BinOpVec, BS);
        SymTab->BindTheoryFun("xnor", BinOpVec, BS);
        SymTab->BindTheoryFun("nand", BinOpVec, BS);
        SymTab->BindTheoryFun("nor", BinOpVec, BS);
        SymTab->BindTheoryFun("iff", BinOpVec, BS);
        SymTab->BindTheoryFun("=", BinOpVec, BS);
        SymTab->BindTheoryFun("ite", ITEOpVec, BS);

        delete BinOpVec[0];
        delete BinOpVec[1];
                              
        delete UnOpVec[0];
        delete ITEOpVec[0];
        delete ITEOpVec[1];
        delete ITEOpVec[2];
        delete BS;
    }

    void LogicSymbolLoader::LoadBV(SymbolTable* SymTab, u32 Size)
    {
        BVLoaded = true;

        vector<const SortExpr*> UnOpVec(1);
        vector<const SortExpr*> BinOpVec(2);

        UnOpVec[0] = new BVSortExpr(Size);
        BinOpVec[0] = new BVSortExpr(Size);
        BinOpVec[1] = new BVSortExpr(Size);
        vector<const SortExpr*> ITEOpVec(3);
        ITEOpVec[0] = new BoolSortExpr();
        ITEOpVec[1] = new BVSortExpr(Size);
        ITEOpVec[2] = new BVSortExpr(Size);
        
        auto BVS = new BVSortExpr(Size);
        auto BS = new BoolSortExpr();

        vector<const SortExpr*> IntOpVec(1);
        IntOpVec[0] = new IntSortExpr();

        SymTab->BindTheoryFun("bvand", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvor", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvxor", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvxnor", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvnand", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvnor", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvadd", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvsub", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvmul", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvudiv", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvsdiv", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvshl", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvshr", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvashr", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvlshr", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvurem", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvsrem", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvsmod", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvugt", BinOpVec, BS);
        SymTab->BindTheoryFun("bvuge", BinOpVec, BS);
        SymTab->BindTheoryFun("bvsgt", BinOpVec, BS);
        SymTab->BindTheoryFun("bvsge", BinOpVec, BS);
        SymTab->BindTheoryFun("bvule", BinOpVec, BS);
        SymTab->BindTheoryFun("bvult", BinOpVec, BS);
        SymTab->BindTheoryFun("bvsle", BinOpVec, BS);
        SymTab->BindTheoryFun("bvslt", BinOpVec, BS);
        SymTab->BindTheoryFun("bvcomp", BinOpVec, BVS);
        SymTab->BindTheoryFun("bvredor", UnOpVec, BVS);
        SymTab->BindTheoryFun("bvredand", UnOpVec, BVS);
        SymTab->BindTheoryFun("bvneg", UnOpVec, BVS);
        SymTab->BindTheoryFun("bvnot", UnOpVec, BVS);
        SymTab->BindTheoryFun("bv2nat", UnOpVec, IntOpVec[0]);
        
        //  This causes problems. Disabled for now.
        //SymTab->BindTheoryFun("nat2bv", CloneVector(IntOpVec), new BVSortExpr(Size));

        SymTab->BindTheoryFun("ite", ITEOpVec, BVS);
        SymTab->BindTheoryFun("=", BinOpVec, BS);

        delete UnOpVec[0];
        delete BinOpVec[0];
        delete BinOpVec[1];
        delete IntOpVec[0];

        delete ITEOpVec[0];
        delete ITEOpVec[1];
        delete ITEOpVec[2];
        delete BVS;
        delete BS;
    }

    void LogicSymbolLoader::LoadReal(SymbolTable* SymTab)
    {
        vector<const SortExpr*> BinOpVec(2);
        vector<const SortExpr*> UnOpVec(1);

        BinOpVec[0] = new RealSortExpr();
        BinOpVec[1] = new RealSortExpr();
        
        UnOpVec[0] = new RealSortExpr();

        vector<const SortExpr*> ITEOpVec(3);
        ITEOpVec[0] = new BoolSortExpr();
        ITEOpVec[1] = new RealSortExpr();
        ITEOpVec[2] = new RealSortExpr();

        vector<const SortExpr*> IntOpVec(1);
        IntOpVec[0] = new IntSortExpr();
        auto RS = new RealSortExpr();
        auto BS = new BoolSortExpr();

        SymTab->BindTheoryFun("+", BinOpVec, RS);
        SymTab->BindTheoryFun("-", BinOpVec, RS);
        SymTab->BindTheoryFun("-", UnOpVec, RS);
        SymTab->BindTheoryFun("*", BinOpVec, RS);
        SymTab->BindTheoryFun("/", BinOpVec, RS);
        
        SymTab->BindTheoryFun("<=", BinOpVec, BS);
        SymTab->BindTheoryFun(">=", BinOpVec, BS);
        SymTab->BindTheoryFun("<", BinOpVec, BS);
        SymTab->BindTheoryFun(">", BinOpVec, BS);

        SymTab->BindTheoryFun("=", BinOpVec, BS);
        SymTab->BindTheoryFun("ite", ITEOpVec, RS);

        // Conversion functions
        SymTab->BindTheoryFun("to_real", IntOpVec, RS);
        SymTab->BindTheoryFun("to_int", UnOpVec, RS);
        SymTab->BindTheoryFun("is_int", UnOpVec, RS);

        delete BinOpVec[0];
        delete BinOpVec[1];
        delete UnOpVec[0];
        
        delete ITEOpVec[0];
        delete ITEOpVec[1];
        delete ITEOpVec[2];

        delete IntOpVec[0];
        delete RS;
        delete BS;
    }

    void LogicSymbolLoader::LoadLIA(SymbolTable* SymTab)
    {
        vector<const SortExpr*> BinOpVec(2);
        vector<const SortExpr*> UnOpVec(1);

        BinOpVec[0] = new IntSortExpr();
        BinOpVec[1] = new IntSortExpr();
        
        UnOpVec[0] = new IntSortExpr();

        vector<const SortExpr*> ITEOpVec(3);
        ITEOpVec[0] = new BoolSortExpr();
        ITEOpVec[1] = new IntSortExpr();
        ITEOpVec[2] = new IntSortExpr();
        auto IS = new IntSortExpr();
        auto BS = new BoolSortExpr();

        SymTab->BindTheoryFun("+", BinOpVec, IS);
        SymTab->BindTheoryFun("-", BinOpVec, IS);
        SymTab->BindTheoryFun("*", BinOpVec, IS);
        SymTab->BindTheoryFun("/", BinOpVec, IS);
        SymTab->BindTheoryFun("div", BinOpVec, IS);
        SymTab->BindTheoryFun("mod", BinOpVec, IS);
        SymTab->BindTheoryFun("abs", UnOpVec, IS);
        SymTab->BindTheoryFun("-", UnOpVec, IS);
        
        SymTab->BindTheoryFun("<", BinOpVec, BS);
        SymTab->BindTheoryFun("<=", BinOpVec, BS);
        SymTab->BindTheoryFun(">", BinOpVec, BS);
        SymTab->BindTheoryFun(">=", BinOpVec, BS);
        SymTab->BindTheoryFun("=", BinOpVec, BS);

        SymTab->BindTheoryFun("ite", ITEOpVec, IS);

        delete BinOpVec[0];
        delete BinOpVec[1];
        delete UnOpVec[0];
        
        delete ITEOpVec[0];
        delete ITEOpVec[1];
        delete ITEOpVec[2];

        delete IS;
        delete BS;
    }


    void LogicSymbolLoader::LoadString(SymbolTable* SymTab)
    {
        vector<const SortExpr*> TerOpVec(3);
        vector<const SortExpr*> BinOpVec(2);
        vector<const SortExpr*> UnOpVec(1);
     
        TerOpVec[0] = new StringSortExpr();
        TerOpVec[1] = new StringSortExpr();
        TerOpVec[2] = new StringSortExpr();
   
        BinOpVec[0] = new StringSortExpr();
        BinOpVec[1] = new StringSortExpr();
        
        UnOpVec[0] = new StringSortExpr();
        
        vector<const SortExpr*> ITEOpVec(3);
        ITEOpVec[0] = new BoolSortExpr();
        ITEOpVec[1] = new StringSortExpr();
        ITEOpVec[2] = new StringSortExpr();

        vector<const SortExpr*> SSIOpVec(3);
        SSIOpVec[0] = new StringSortExpr();
        SSIOpVec[1] = new StringSortExpr();
        SSIOpVec[2] = new IntSortExpr();

        vector<const SortExpr*> SIIOpVec(3);
        SIIOpVec[0] = new StringSortExpr();
        SIIOpVec[1] = new IntSortExpr();
        SIIOpVec[2] = new IntSortExpr();

        vector<const SortExpr*> SIOpVec(2);
        SIOpVec[0] = new StringSortExpr();
        SIOpVec[1] = new IntSortExpr();

        vector<const SortExpr*> IOpVec(1);
        IOpVec[0] = new IntSortExpr();

        auto SS = new StringSortExpr();
        auto BS = new BoolSortExpr();
        auto IS = new IntSortExpr();

        SymTab->BindTheoryFun("str.len", UnOpVec, IS);
        SymTab->BindTheoryFun("str.to.int", UnOpVec, IS);
        SymTab->BindTheoryFun("int.to.str", IOpVec, SS);
        SymTab->BindTheoryFun("str.++", BinOpVec, SS); // should be gernalized to arbitrary arity
        SymTab->BindTheoryFun("str.substr", SIIOpVec, SS);
        SymTab->BindTheoryFun("str.prefixof", BinOpVec, BS);
        SymTab->BindTheoryFun("str.suffixof", BinOpVec, BS);
        SymTab->BindTheoryFun("str.contains", BinOpVec, BS);
        SymTab->BindTheoryFun("str.replace", TerOpVec, SS);
        SymTab->BindTheoryFun("str.indexof", SSIOpVec, IS);
        SymTab->BindTheoryFun("str.at", SIOpVec, SS);

        // SymTab->BindTheoryFun("=", BinOpVec, SS);
        
        // SymTab->BindTheoryFun("ite", ITEOpVec, IS);
        
        delete TerOpVec[0];
        delete TerOpVec[1];
        delete TerOpVec[2];
        delete BinOpVec[0];
        delete BinOpVec[1];
        delete UnOpVec[0];
        
        delete ITEOpVec[0];
        delete ITEOpVec[1];
        delete ITEOpVec[2];
        
	delete SSIOpVec[0];
        delete SSIOpVec[1];
        delete SSIOpVec[2];
        delete SIIOpVec[0];
        delete SIIOpVec[1];
        delete SIIOpVec[2];
        delete SIOpVec[0];
        delete SIOpVec[1];
        delete IOpVec[0];

        delete IS;
        delete SS;
        delete BS;

    }


    void LogicSymbolLoader::LoadArrays(SymbolTable* SymTab, const ArraySortExpr* Sort)
    {
        vector<const SortExpr*> OpVec;
        OpVec.push_back(Sort);
        OpVec.push_back(Sort->GetDomainSort());
        
        SymTab->BindTheoryFun("select", OpVec, Sort->GetRangeSort());
        
        OpVec.clear();
        OpVec.push_back(Sort);
        OpVec.push_back(Sort->GetDomainSort());
        OpVec.push_back(Sort->GetRangeSort());

        SymTab->BindTheoryFun("store", OpVec, Sort);

        // equality and ite
        OpVec.clear();
        OpVec.push_back(Sort);
        OpVec.push_back(Sort);

        auto BS = new BoolSortExpr();

        SymTab->BindTheoryFun("=", OpVec, BS);

        OpVec.clear();
        OpVec.push_back(BS);
        OpVec.push_back(Sort);
        OpVec.push_back(Sort);

        SymTab->BindTheoryFun("ite", OpVec, Sort);
        delete BS;
    }

    void LogicSymbolLoader::RegisterSort(SymbolTable* SymTab,
                                         const SortExpr* NewSort)
    {
        NewSort = SymTab->ResolveSort(NewSort);
        if (RegisteredTypes.find(NewSort) != RegisteredTypes.end()) {
            return;
        } else {
            if (NewSort->GetKind() == SORTKIND_ARRAY) {
                LoadArrays(SymTab, dynamic_cast<const ArraySortExpr*>(NewSort));
            } else if (NewSort->GetKind() == SORTKIND_BV && BVLoaded) {
                LoadBV(SymTab, dynamic_cast<const BVSortExpr*>(NewSort)->GetSize());
            } else {
                // Just create the ITE and = operators
                vector<const SortExpr*> OpVec;
                OpVec.push_back(NewSort);
                OpVec.push_back(NewSort);
                auto BS = new BoolSortExpr();
                SymTab->BindTheoryFun("=", OpVec, BS);
                OpVec.clear();
                OpVec.push_back(BS);
                OpVec.push_back(NewSort);
                OpVec.push_back(NewSort);
                SymTab->BindTheoryFun("ite", OpVec, NewSort);
                delete BS;
            }
            RegisteredTypes.insert(static_cast<SortExpr*>(NewSort->Clone()));
        }
    }

    void LogicSymbolLoader::LoadAll(SymbolTable* SymTab)
    {
        auto BS = new BoolSortExpr();
        auto IS = new IntSortExpr();
        auto RS = new RealSortExpr();
        RegisteredTypes.insert(BS);
        RegisteredTypes.insert(RS);
        RegisteredTypes.insert(IS);
        LoadCore(SymTab);

        for(u32 i = 1; i <= SYNTHLIB2PARSER_MAX_BV_LEN; ++i) {
            auto BVS = new BVSortExpr(i);
            if(RegisteredTypes.find(BVS) == RegisteredTypes.end()) {
                RegisteredTypes.insert(BVS);
                LoadBV(SymTab, i);
            } else {
                delete BVS;
            }
        }

        LoadLIA(SymTab);
        LoadReal(SymTab);
        LoadString(SymTab);
    }

    void LogicSymbolLoader::Reset()
    {
        // Clear all the registered types
        for(auto const& Sort : RegisteredTypes) {
            delete Sort;
        }

        RegisteredTypes.clear();
        BVLoaded = false;
    }

} /* end namespace */
