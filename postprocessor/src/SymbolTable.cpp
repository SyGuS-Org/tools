#include <SymbolTable.hpp>

namespace {

char* fixable_ops[] = {
    "or", "and", "xor", "+", "*", NULL
};

bool str_cmp(const char* a, const char* b)
{
    while (*a && *b && *a==*b)
    {
        a++;
        b++;
    }

    return *b == 0;
}

bool fixable_op(const char* a)
{
    char** op_p = fixable_ops;

    while (*op_p)
    {
        if (str_cmp(a, *op_p)) {
            return true;
        }
        op_p++;
    }

    return false;
}

}

namespace SynthLib2Parser {

    SymbolTableEntry::SymbolTableEntry(SymtabEntryKind STEKind, const SortExpr* STESort)
        : STEKind(STEKind), STESort(STESort), FunDef(NULL), LetBoundTerm(NULL), InvSort(NULL)
    {
        if(STEKind == STENTRY_USER_FUNCTION || STEKind == STENTRY_BVARIABLE) {
            throw SymbolTableException("INTERNAL: Wrong constructor called on SymbolTableEntry");
        }
    }

    SymbolTableEntry::SymbolTableEntry(FunDefCmd* FunDef)
        : STEKind(STENTRY_USER_FUNCTION), FunDef(FunDef), LetBoundTerm(NULL), InvSort(NULL)
    {
        // Create a sort for this function type
        vector<const SortExpr*> ArgSorts;
        auto const& Args = FunDef->GetArgs();
        for(auto const& ASPair : Args) {
            ArgSorts.push_back(static_cast<const SortExpr*>(ASPair->GetSort()->Clone()));
        }
        
        STESort = new FunSortExpr(SourceLocation::None, ArgSorts, 
                                  static_cast<const SortExpr*>(FunDef->GetSort()->Clone()));
    }

    SymbolTableEntry::SymbolTableEntry(Term* LetBoundTerm, const SortExpr* TermSort)
        : STEKind(STENTRY_BVARIABLE), STESort(TermSort),
          FunDef(NULL), LetBoundTerm(LetBoundTerm), InvSort(NULL)
    {
        // Nothing here
    }

    SymbolTableEntry::SymbolTableEntry(FunSortExpr* InvSort)
        : STEKind(STENTRY_SYNTH_INVARIANT), STESort(NULL),
          FunDef(NULL), LetBoundTerm(NULL), InvSort(InvSort)
    {


    }

    SymbolTableEntry::~SymbolTableEntry()
    {
        if(STESort != NULL) {
            delete STESort;
            STESort = NULL;
        }
        if(FunDef != NULL) {
            delete FunDef;
            FunDef = NULL;
        }
        if(LetBoundTerm != NULL) {
            delete LetBoundTerm;
            LetBoundTerm = NULL;
        }
    }

    SymtabEntryKind SymbolTableEntry::GetKind() const
    {
        return STEKind;
    }

    const SortExpr* SymbolTableEntry::GetSort() const 
    {
        return STESort;
    }
    
    FunDefCmd* SymbolTableEntry::GetFunDef() const
    {
        if(GetKind() != STENTRY_USER_FUNCTION && GetKind() != STENTRY_SYNTH_INVARIANT) {
            throw SymbolTableException((string)"INTERNAL: Called SymbolTableEntry::GetFunDef() " + 
                                       "on non function STE");
        }
        return FunDef;
    }

    Term* SymbolTableEntry::GetLetBoundTerm() const
    {
        if(GetKind() != STENTRY_BVARIABLE) {
            throw SymbolTableException((string)"INTERNAL: Called " + 
                                       "SymbolTableEntry::GetLetBoundTerm() " + 
                                       "on non bound var STE");
        }
        return LetBoundTerm;
    }


    FunSortExpr* SymbolTableEntry::GetInvSort() const
    {
        if(GetKind() != STENTRY_SYNTH_INVARIANT) {
            throw SymbolTableException((string)"INTERNAL: Called " + 
                                       "SymbolTableEntry::GetInvSort() " + 
                                       "on non synth-inv STE");
        }
        return InvSort;

    }

    SymbolTableEntry* SymbolTableEntry::Clone() const
    {
        switch(STEKind) {
        case STENTRY_QVARIABLE:
        case STENTRY_ARG:
        case STENTRY_SORT:
        case STENTRY_THEORY_FUNCTION:
        case STENTRY_SYNTH_FUNCTION:
        case STENTRY_UNINTERP_FUNCTION:
            return new SymbolTableEntry(STEKind, static_cast<SortExpr*>(STESort->Clone()));

        case STENTRY_SYNTH_INVARIANT:
            return new SymbolTableEntry(static_cast<FunSortExpr*>(InvSort->Clone()));

        case STENTRY_USER_FUNCTION:
            return new SymbolTableEntry(static_cast<FunDefCmd*>(FunDef->Clone()));

        case STENTRY_BVARIABLE:
            return new SymbolTableEntry(static_cast<Term*>(LetBoundTerm->Clone()),
                                        static_cast<SortExpr*>(STESort->Clone()));
        }
        // get the compiler to stop complaining.
        // should never reach here
        return NULL;
    }

    // SymtabScope implementation
    SymbolTableScope::SymbolTableScope()
    {
        // Nothing here
    }

    SymbolTableScope::~SymbolTableScope()
    {
        // delete all the entries
        for (auto const& NameSTEPair : Bindings) {
            delete NameSTEPair.second;
        }
        Bindings.clear();
    }

    void SymbolTableScope::DeleteSynthFuncs()
    {
      for (auto it = Bindings.begin(); it != Bindings.end();  ) {
	if(it->second->GetKind()==STENTRY_SYNTH_FUNCTION) {
	  delete it->second;
	  it = Bindings.erase(it);
	}
	else
	  it++;
      }
    }

    void SymbolTableScope::PrintSynthFuncs()
    {
      for (auto it = Bindings.begin(); it != Bindings.end();  it++) {
        if(it->second->GetKind()==STENTRY_SYNTH_FUNCTION) {
	  cout << "=== synth-fun " << it->first << endl; 
        }
      }   
    } 

    void SymbolTableScope::PrintLetVars()
    {
      for (auto it = Bindings.begin(); it != Bindings.end();  it++) {
        if(it->second->GetKind()==STENTRY_BVARIABLE) {
          cout << "=== let variable " << it->first << endl;
        }
      }
    }

    u32 SymbolTableScope::Size()
    {
	return Bindings.size();
    }

    SymbolTableEntry* SymbolTableScope::Lookup(const string& Identifier) const
    {
        auto it = Bindings.find(Identifier);
        if(it == Bindings.end()) {
            return NULL;
        }
        return it->second;
    }

    void SymbolTableScope::Bind(const string& Identifier, SymbolTableEntry* STE)
    {
        Bindings[Identifier] = STE;
    }

    void SymbolTableScope::CheckedBind(const string& Identifier, SymbolTableEntry* STE)
    {
        if (Lookup(Identifier) != NULL) {
            throw SymbolTableException((string)"Error: Redeclaration of identifier \"" + 
                                       Identifier + "\"");
        }
        Bindings[Identifier] = STE;
    }

    SymbolTableScope* SymbolTableScope::Clone() const
    {
        auto Retval = new SymbolTableScope();
        for(auto const& Binding : Bindings) {
            (Retval->Bindings)[Binding.first] = Binding.second->Clone();
        }
        return Retval;
    }

    // Symbol table implementation
    SymbolTable::SymbolTable()
    {
        // Push the global scope
        Scopes.push_back(new SymbolTableScope());
    }

    SymbolTable::~SymbolTable()
    {
        for(auto const& Scope : Scopes) {
            delete Scope;
        }
    }

    void SymbolTable::DeleteSynthFuncs() 
    {
      for(auto const& Scope : Scopes) {
	Scope->DeleteSynthFuncs();
      }
    }

    void SymbolTable::PrintSynthFuncs()
    {
      for(auto const& Scope : Scopes) {
        Scope->PrintSynthFuncs();
      }
    }


    u32 SymbolTable::Size()
    {
        u32 n = 0;
        for(auto const& Scope : Scopes) {
            n += Scope->Size();              
        }
        return n; 
    }

    // Utility function for name mangling
    inline string SymbolTable::MangleName(const string& Name, 
                                          const vector<const SortExpr*>& ArgSorts) const
    {
        bool fixable = fixable_op(Name.c_str());
        string Retval = Name;
        for (size_t i = 0, size = ArgSorts.size(); i < size; i++) {
          auto ActualSort = ResolveSort(ArgSorts[i]);
          Retval += "_@_" + ActualSort->ToMangleString();

          // TODO: This is a hack to make type checking work for
          // chainable operators
          if (fixable && i == 1) {
            break;
          }
        }
        return Retval;
    }

    inline string SymbolTable::MangleSortName(const string& Name) const
    {
        return Name + "_@S";
    }

    // Sort resolution
    const SortExpr* SymbolTable::ResolveSort(const SortExpr* TheSort) const
    {
        if(TheSort == NULL) {
            throw SymbolTableException((string)"Interal: SymbolTable::ResolveSort() " + 
                                        "was called with a NULL argument"); 
            return NULL;
        }
        if(TheSort->GetKind() != SORTKIND_NAMED) {
            return TheSort;
        }
        auto SortAsNamed = dynamic_cast<const NamedSortExpr*>(TheSort);
        auto NamedEntry = LookupSort(SortAsNamed->GetName());
        return ResolveSort(NamedEntry->GetSort());
    }

    void SymbolTable::Push()
    {
        Scopes.push_back(new SymbolTableScope());
    }

    void SymbolTable::Push(SymbolTableScope* Scope)
    {
        Scopes.push_back(Scope);
    }

    SymbolTableScope* SymbolTable::Pop()
    {
        if (Scopes.size() == 1) {
            throw SymbolTableException("Error: SymbolTable::Pop(), tried to pop too many scopes");
        }
        auto Retval = Scopes.back();
        Scopes.pop_back();
        return Retval;
    }

    SymbolTableScope* SymbolTable::GetIthScope(u32 i) const {
      return Scopes[i];
    }

    SymbolTable* SymbolTable::Clone() const {

      SymbolTable* ClonedTable = new SymbolTable();

      for (u32  k = 0; k < Scopes.size(); k++) {
        ClonedTable->Push(GetIthScope((u32) k)->Clone());
      }

      return ClonedTable;
    }


    const SymbolTableEntry* SymbolTable::Lookup(const string& Identifier) const
    {
        SymbolTableEntry* Retval = NULL;
        const u32 NumScopes = Scopes.size();
        
        for(u32 i = NumScopes; i > 0; --i) {
            if((Retval = Scopes[i-1]->Lookup(Identifier)) != NULL) {
                return Retval;
            }
        }
        return NULL;
    }

    const SymbolTableEntry* SymbolTable::LookupSort(const string& SortName) const
    {
        auto Retval = Lookup(MangleSortName(SortName));
        if(Retval != NULL && Retval->GetKind() == STENTRY_SORT) {
            return Retval;
        }
        return NULL;
    }

    const SymbolTableEntry* SymbolTable::LookupSortRecursive(const string& SortName) const
    {
        auto Retval = LookupSort(SortName);
        if (Retval != nullptr && Retval->GetKind() == STENTRY_SORT &&
            Retval->GetSort()->GetKind() == SORTKIND_NAMED) {
            const NamedSortExpr* RetvalPrime = static_cast<const NamedSortExpr*>(Retval->GetSort());
            return LookupSortRecursive(RetvalPrime->GetName());
        } else {
            return Retval;
        }
    }

    const SymbolTableEntry* SymbolTable::LookupVariable(const string& VarName) const
    {
        auto Retval = Lookup(VarName);
        if(Retval != NULL && (Retval->GetKind() == STENTRY_QVARIABLE || 
                              Retval->GetKind() == STENTRY_BVARIABLE ||
                              Retval->GetKind() == STENTRY_ARG)) {
            return Retval;
        }
        return NULL;
    }

    const SymbolTableEntry* SymbolTable::LookupFun(const string& Name, 
                                                   const vector<const SortExpr*>& ArgSorts) const
    {
        auto MangledName = MangleName(Name, ArgSorts);
        auto Retval = Lookup(MangledName);
        if(Retval != NULL && (Retval->GetKind() == STENTRY_USER_FUNCTION ||
                              Retval->GetKind() == STENTRY_SYNTH_FUNCTION ||
                              Retval->GetKind() == STENTRY_SYNTH_INVARIANT ||
                              Retval->GetKind() == STENTRY_THEORY_FUNCTION ||
                              Retval->GetKind() == STENTRY_UNINTERP_FUNCTION )) {
            return Retval;
        }
        return NULL;
    }

    const SymbolTableEntry* SymbolTable::LookupInv(const string& Name) const
    {
        auto Retval = Lookup(Name);
        if(Retval != NULL && Retval->GetKind() == STENTRY_SYNTH_INVARIANT) {
            return Retval;
        }
        return NULL;
    }

    void SymbolTable::BindSort(const string& Name, SortExpr* Sort)
    {
        Scopes[0]->CheckedBind(MangleSortName(Name),
                                   new SymbolTableEntry(STENTRY_SORT, Sort));
    }

    void SymbolTable::BindVariable(const string& Name, SortExpr* Sort)
    {
        Scopes.back()->CheckedBind(Name, new SymbolTableEntry(STENTRY_QVARIABLE, Sort));
    }

    void SymbolTable::BindFormal(const string& Name, const SortExpr* Sort)
    {
        Scopes.back()->CheckedBind(Name, new SymbolTableEntry(STENTRY_ARG, Sort));
    }

    void SymbolTable::BindLetVariable(const string& Name, Term* LetBoundTerm)
    {
        Scopes.back()->
            CheckedBind(Name, 
                        new 
                        SymbolTableEntry(static_cast<Term*>(LetBoundTerm->Clone()),
                                         static_cast<SortExpr*>
                                         (LetBoundTerm->GetTermSort(this)->Clone())));
    }

    void SymbolTable::BindTheoryFun(const string& Name, 
                                    const vector<const SortExpr*>& ArgSorts, 
                                    const SortExpr* RetSort)
    {
        auto FunSort = new FunSortExpr(SourceLocation::None, CloneVector(ArgSorts), 
                                       static_cast<const SortExpr*>(RetSort->Clone()));
        auto MangledName = MangleName(Name, ArgSorts);

        Scopes[0]->CheckedBind(MangledName, new SymbolTableEntry(STENTRY_THEORY_FUNCTION, FunSort));
    }

    void SymbolTable::BindUserFun(FunDefCmd* FunDef)
    {
        // get the arg sorts
        vector<const SortExpr*> ArgSorts;
        for(auto const& ASPair : FunDef->GetArgs()) {
            ArgSorts.push_back(ASPair->GetSort());
        }
        auto MangledName = MangleName(FunDef->GetFunName(), ArgSorts);
        Scopes.back()->CheckedBind(MangledName, new SymbolTableEntry(FunDef));
    }

    void SymbolTable::BindSynthFun(const string& Name, 
                                   const vector<const SortExpr*>& ArgSorts, 
                                   const SortExpr* RetSort)
    {
        auto FunSort = new FunSortExpr(SourceLocation::None, CloneVector(ArgSorts),
                                       static_cast<const SortExpr*>(RetSort->Clone()));
        auto MangledName = MangleName(Name, ArgSorts);
        Scopes.back()->CheckedBind(MangledName, 
                                   new SymbolTableEntry(STENTRY_SYNTH_FUNCTION, FunSort));
    }

    void SymbolTable::BindSynthInv(const string& Name, 
                                   const vector<const SortExpr*>& ArgSorts)
    {
        auto FunSort = new FunSortExpr(SourceLocation::None, CloneVector(ArgSorts),
                                       static_cast<const SortExpr*>(BoolSortExpr().Clone()));
        auto InvSort = new FunSortExpr(SourceLocation::None, CloneVector(ArgSorts),
                                       static_cast<const SortExpr*>(BoolSortExpr().Clone()));
        auto MangledName = MangleName(Name, ArgSorts);
        // Save it with the arguments sorts, to make sure no other functions with
        // same name and sort is defined
        Scopes.back()->CheckedBind(MangledName, 
                                   new SymbolTableEntry(STENTRY_SYNTH_INVARIANT, FunSort));
        // Save it with just the name so that it be possible to find it when processing
        // inv-constraint (where the argumetns are not given)
        Scopes.back()->CheckedBind(Name, 
                                   new SymbolTableEntry(InvSort));

    }

    void SymbolTable::BindUninterpFun(const string& Name,
                                      const vector<const SortExpr*>& ArgSorts,
                                      const SortExpr* RetSort)
    {
        auto FunSort = new FunSortExpr(SourceLocation::None,
                                       CloneVector(ArgSorts),
                                       static_cast<const SortExpr*>(RetSort->Clone()));
        auto MangledName = MangleName(Name, ArgSorts);
        Scopes.back()->CheckedBind(MangledName, new SymbolTableEntry(STENTRY_UNINTERP_FUNCTION, 
                                                                     FunSort));
    }

} /* end namespace */

