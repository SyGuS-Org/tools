#include <LogicSymbols.hpp>
#include <SymtabBuilder.hpp>

namespace SynthLib2Parser {

    SymtabBuilder::SymtabBuilder(SymbolTable* TheSymbolTable)
        : ASTVisitorBase("SymtabBuilder"), TheSymbolTable(TheSymbolTable)
    {
        // Nothing here
    }

    SymtabBuilder::~SymtabBuilder()
    {
        // Nothing here
    }

    void SymtabBuilder::VisitProgram(const Program* Prog)
    {
        // Delegate to base class
        ASTVisitorBase::VisitProgram(Prog);
    }

    void SymtabBuilder::VisitFunDefCmd(const FunDefCmd* Cmd)
    {
        // previsit. Push a new scope onto the symbol table
        TheSymbolTable->Push();
        // Check that the formals are okay.
        // And bind them in the new scope
        // also gather the argument sorts for later use
        int ArgsNum = 0;
        vector<const SortExpr*> ArgSorts;
        for(auto const& ASPair : Cmd->GetArgs()) {
            ArgsNum++;
            ASPair->Accept(this);
            TheSymbolTable->BindFormal(ASPair->GetName(), 
                                       static_cast<const SortExpr*>(ASPair->GetSort()->Clone()));
            ArgSorts.push_back(ASPair->GetSort());
        }

        // Check that the type of the term is the same as the one declared

        // Push the builder through the term to ensure that let-bound terms are 
        // appropriately bound
        Cmd->GetTerm()->Accept(this);

        auto TermSort = Cmd->GetTerm()->GetTermSort(TheSymbolTable);
        auto ExpTermSort = Cmd->GetSort();
        if(!TheSymbolTable->ResolveSort(TermSort)->Equals(*(TheSymbolTable->ResolveSort(ExpTermSort)))) {
            throw SynthLib2ParserException((string)"Definition of function symbol \"" + 
                                           Cmd->GetFunName() + "\" does not match expected sort\n" + 
                                           Cmd->GetLocation().ToString());
        }

        Cmd->SetScope(TheSymbolTable->Pop());

        // Finally, we check that a function of the same name 
        // is not already defined
        
        if (TheSymbolTable->LookupFun(Cmd->GetFunName(), ArgSorts) != NULL) {
            throw SynthLib2ParserException((string)"Function with name \"" + 
                                           Cmd->GetFunName() + "\" has already been defined/declared.\n" + 
                                           Cmd->GetLocation().ToString());
        }
        if (TheSymbolTable->LookupFun("_"+Cmd->GetFunName(), ArgSorts) != NULL) {
            throw SynthLib2ParserException((string)"Function with name \"" + 
                                           "_"+Cmd->GetFunName() + "\" has already been defined/declared.\n" + 
                                           Cmd->GetLocation().ToString());
        }

        // All seems good.
        // Bind the def after adding an underscore
        
        ostringstream s;
        //s << "_" << Cmd->GetFunName();
        s << Cmd->GetFunName();// << ArgsNum;
        auto CmdPrime = const_cast<FunDefCmd*> (Cmd);
        CmdPrime->SetFunName(s.str());
        auto CmdCopy = static_cast<FunDefCmd*>(CmdPrime->Clone());
        TheSymbolTable->BindUserFun(CmdCopy);

    }

    void SymtabBuilder::VisitFunDeclCmd(const FunDeclCmd* Cmd)
    {
        // Recurse to make sure all sorts are well formed
        ASTVisitorBase::VisitFunDeclCmd(Cmd);

        // Ensure that no other function symbol exists
        if (TheSymbolTable->LookupFun(Cmd->GetFunName(),
                                      Cmd->GetArgSorts()) != NULL) {
            throw SynthLib2ParserException((string)"Function with name \"" + 
                                           Cmd->GetFunName() + "\" has already been defined/declared.\n" + 
                                           Cmd->GetLocation().ToString());
        }

        // Bind this func decl
        TheSymbolTable->BindUninterpFun(Cmd->GetFunName(),
                                        CloneVector(Cmd->GetArgSorts()),
                                        static_cast<const SortExpr*>(Cmd->GetSort()));
    }

    void SymtabBuilder::VisitSynthFunCmd(const SynthFunCmd* Cmd)
    {
        // Push a new scope
        TheSymbolTable->Push();
        ASTVisitorBase::VisitSynthFunCmd(Cmd);
        // All is good
        Cmd->SetScope(TheSymbolTable->Pop());

        // Gather the arg sorts
        auto const& Args = Cmd->GetArgs();
        const u32 NumArgs = Args.size();
        vector<const SortExpr*> ArgSorts(NumArgs);
        for(u32 i = 0; i < NumArgs; ++i) {
            ArgSorts[i] = Args[i]->GetSort();
        }

        // Check that no function exists of the same name
        if (TheSymbolTable->LookupFun(Cmd->GetFunName(),
                                      ArgSorts) != NULL) {
            throw SynthLib2ParserException((string)"Function with name \"" + 
                                           Cmd->GetFunName() + "\" has already been defined/declared.\n" + 
                                           Cmd->GetLocation().ToString());
        }
        
        // Bind
        TheSymbolTable->BindSynthFun(Cmd->GetFunName(),
                                     CloneVector(ArgSorts),
                                     static_cast<SortExpr*>(Cmd->GetSort()->Clone()));
    }

    void SymtabBuilder::VisitSynthInvCmd(const SynthInvCmd* Cmd)
    {
        // Push a new scope
        TheSymbolTable->Push();
        ASTVisitorBase::VisitSynthInvCmd(Cmd);
        // All is good
        Cmd->SetScope(TheSymbolTable->Pop());

        // Gather the arg sorts
        auto const& Args = Cmd->GetArgs();
        const u32 NumArgs = Args.size();
        vector<const SortExpr*> ArgSorts(NumArgs);
        for(u32 i = 0; i < NumArgs; ++i) {
            ArgSorts[i] = Args[i]->GetSort();
        }

        // Check that no function exists of the same name and sorts
        if (TheSymbolTable->LookupFun(Cmd->GetInvName(),
                                      ArgSorts) != NULL) {
            throw SynthLib2ParserException((string)"Function/invariant with name \"" + 
                                           Cmd->GetInvName() + "\" has already been defined/declared.\n" + 
                                           Cmd->GetLocation().ToString());
        }

        // Check that no invariant exists of the same name
        if (TheSymbolTable->LookupInv(Cmd->GetInvName())) {
            throw SynthLib2ParserException((string)"Invariant with name \"" + 
                                           Cmd->GetInvName() + "\" has already been defined/declared.\n" + 
                                           Cmd->GetLocation().ToString());
        }
        
        // Bind
        TheSymbolTable->BindSynthInv(Cmd->GetInvName(),
                                     CloneVector(ArgSorts));
    }

    void SymtabBuilder::VisitSortDefCmd(const SortDefCmd* Cmd)
    {
        ASTVisitorBase::VisitSortDefCmd(Cmd);
        // Check for redeclaration
        vector<const SortExpr*> ArgSortDummy;
        if (TheSymbolTable->LookupSort(Cmd->GetName()) != NULL ||
            TheSymbolTable->LookupVariable(Cmd->GetName()) != NULL ||
            TheSymbolTable->LookupFun(Cmd->GetName(), ArgSortDummy) != NULL) {
            
            throw SynthLib2ParserException((string)"Identifier \"" + Cmd->GetName() + "\" has " + 
                                           "already been declared/defined as a sort/variable/constant.\n" + 
                                           Cmd->GetLocation().ToString());
        }
        // Bind
        TheSymbolTable->BindSort(Cmd->GetName(), static_cast<SortExpr*>(Cmd->GetSortExpr()->Clone()));
    }

    void SymtabBuilder::VisitSetOptsCmd(const SetOptsCmd* Cmd)
    {
        ASTVisitorBase::VisitSetOptsCmd(Cmd);
    }

    void SymtabBuilder::VisitVarDeclCmd(const VarDeclCmd* Cmd)
    {
        // Ensure that the sort is okay
        ASTVisitorBase::VisitVarDeclCmd(Cmd);
        
        // Check for redeclarations
        vector<const SortExpr*> ArgSortDummy;
        if (TheSymbolTable->LookupSort(Cmd->GetName()) != NULL ||
            TheSymbolTable->LookupVariable(Cmd->GetName()) != NULL ||
            TheSymbolTable->LookupFun(Cmd->GetName(), ArgSortDummy) != NULL) {

            throw SynthLib2ParserException((string)"Identifier \"" + Cmd->GetName() + "\" has " + 
                                           "already been declared/defined as a sort/variable/constant.\n" + 
                                           Cmd->GetLocation().ToString());            
        }
        
        // Bind
        TheSymbolTable->BindVariable(Cmd->GetName(), static_cast<SortExpr*>(Cmd->GetSort()->Clone()));
    }

    void SymtabBuilder::VisitPrimedVarDeclCmd(const PrimedVarDeclCmd* Cmd)
    {
        // Ensure that the sort is okay
        ASTVisitorBase::VisitPrimedVarDeclCmd(Cmd);
        
        // Check for redeclarations
        vector<const SortExpr*> ArgSortDummy;
        if (TheSymbolTable->LookupSort(Cmd->GetName()) != NULL ||
            TheSymbolTable->LookupVariable(Cmd->GetName()) != NULL ||
            TheSymbolTable->LookupFun(Cmd->GetName(), ArgSortDummy) != NULL) {

            throw SynthLib2ParserException((string)"Identifier \"" + Cmd->GetName() + "\" has " + 
                                           "already been declared/defined as a sort/variable/constant.\n" + 
                                           Cmd->GetLocation().ToString());            
        }

        if (TheSymbolTable->LookupSort(Cmd->GetPrimedName()) != NULL ||
            TheSymbolTable->LookupVariable(Cmd->GetPrimedName()) != NULL ||
            TheSymbolTable->LookupFun(Cmd->GetPrimedName(), ArgSortDummy) != NULL) {

            throw SynthLib2ParserException((string)"Identifier \"" + Cmd->GetPrimedName() + "\" has " + 
                                           "already been declared/defined as a sort/variable/constant.\n" + 
                                           Cmd->GetLocation().ToString());            
        }
        
        // Bind
        TheSymbolTable->BindVariable(Cmd->GetName(), static_cast<SortExpr*>(Cmd->GetSort()->Clone()));
        TheSymbolTable->BindVariable(Cmd->GetPrimedName(), static_cast<SortExpr*>(Cmd->GetSort()->Clone()));
    }


    void SymtabBuilder::VisitConstraintCmd(const ConstraintCmd* Cmd)    
    {
        Term* TheTerm = Cmd->GetTerm();    

        // Check that the type of the term is okay
        auto Sort = TheTerm->GetTermSort(TheSymbolTable);

	if (TheSymbolTable->ResolveSort(Sort)->GetKind() != SORTKIND_BOOL) {
            throw SynthLib2ParserException((string)"Constraint terms must be boolean typed.\n" + 
                                           Cmd->GetLocation().ToString());
        }
        // Do nothing otherwise
        return;
    }

    void SymtabBuilder::VisitInvConstraintCmd(const InvConstraintCmd* Cmd)
    {   
        // Check that SynthInvName is in symbol table
        auto STE = TheSymbolTable->LookupInv(Cmd->GetSynthInvName());
        if (STE == NULL) {
            throw SynthLib2ParserException((string)"Invariant " + Cmd->GetSynthInvName() +
                                            " has not been defined. \n" + 
                                           Cmd->GetLocation().ToString());
        }
        
        // For each of the three: pre-cond, transition, post-cond
        //   check that it is in the symbol table, with correct args 
        //   (where for trans there should be twice the same args)
        //   and that it's sort is Boolean

        FunSortExpr* FSR = STE->GetInvSort();
        vector<const SortExpr*> Args = FSR->GetArgSorts();

        // For PreCondition

        auto STEpre = TheSymbolTable->LookupFun(Cmd->GetPreCondName(), Args);
        if (STEpre == NULL) {
                throw SynthLib2ParserException((string)"Pre-condition " + Cmd->GetPreCondName() +
                                            " for invariant "+ Cmd->GetSynthInvName() +
                                            " has not been defined, or has been defined" + 
                                            " with different arguments than"
                                            + Cmd->GetSynthInvName() + ". \n" + 
                                            Cmd->GetLocation().ToString());
        }
        if (STEpre->GetFunDef()->GetSort()->GetKind() != SORTKIND_BOOL) {
                throw SynthLib2ParserException((string)"Pre-condition " + Cmd->GetPreCondName() +
                                            " for invariant "+ Cmd->GetSynthInvName() +
                                            " must be of sort Bool. \n" + 
                                            Cmd->GetLocation().ToString());
        }

        
        // For PostCondition

        auto STEpost = TheSymbolTable->LookupFun(Cmd->GetPostCondName(), Args);
        if (STEpost== NULL) {
                throw SynthLib2ParserException((string)"Post-condition " + Cmd->GetPostCondName() +
                                            " for invariant "+ Cmd->GetSynthInvName() +
                                            " has not been defined, or has been defined" + 
                                            " with different arguments than"
                                            + Cmd->GetSynthInvName() + ". \n" + 
                                            Cmd->GetLocation().ToString());
        }
        if (STEpost->GetFunDef()->GetSort()->GetKind() != SORTKIND_BOOL) {
                throw SynthLib2ParserException((string)"Post-condition " + Cmd->GetPreCondName() +
                                            " for invariant "+ Cmd->GetSynthInvName() +
                                            " must be of sort Bool. \n" + 
                                            Cmd->GetLocation().ToString());
        }

        // For Transition

        // First duplicate the Args
        const u32 NumArgs = Args.size();
        vector<const SortExpr*> ArgsArgs(2*NumArgs);
        for(u32 i = 0; i < NumArgs; ++i) { ArgsArgs[i] = Args[i];}
        for(u32 i = 0; i < NumArgs; ++i) { ArgsArgs[NumArgs+i] = ArgsArgs[i];}

        auto STEtrans = TheSymbolTable->LookupFun(Cmd->GetTransName(), ArgsArgs);
        if (STEtrans == NULL) {
                throw SynthLib2ParserException((string)"Transition-condition " + Cmd->GetTransName() +
                                            " for invariant "+ Cmd->GetSynthInvName() +
                                            " has not been defined, or has been defined" + 
                                            " with different arguments than"
                                            + Cmd->GetSynthInvName() + ".  The arguments for" + 
                                            " the transition condition should be twice the length to" +
                                            " include primed version of the arguments as well.\n" + 
                                            Cmd->GetLocation().ToString());
        }
        if (STEtrans->GetFunDef()->GetSort()->GetKind() != SORTKIND_BOOL) {
                throw SynthLib2ParserException((string)"Transition-condition " + Cmd->GetPreCondName() +
                                            " for invariant "+ Cmd->GetSynthInvName() +
                                            " must be of sort Bool. \n" + 
                                            Cmd->GetLocation().ToString());
        }
        
        // Do nothing otherwise
        return;
    }

    void SymtabBuilder::VisitSetLogicCmd(const SetLogicCmd* Cmd)
    {
        ASTVisitorBase::VisitSetLogicCmd(Cmd);
    }

    void SymtabBuilder::VisitCheckSynthCmd(const CheckSynthCmd* Cmd)
    {
        ASTVisitorBase::VisitCheckSynthCmd(Cmd);
    }

    void SymtabBuilder::VisitArgSortPair(const ArgSortPair* ASPair)
    {
        // Just check that the sort is well formed
        ASTVisitorBase::VisitArgSortPair(ASPair);
    }

    void SymtabBuilder::VisitIntSortExpr(const IntSortExpr* Sort)
    {
        ASTVisitorBase::VisitIntSortExpr(Sort);
    }

    void SymtabBuilder::VisitStringSortExpr(const StringSortExpr* Sort)
    {
        ASTVisitorBase::VisitStringSortExpr(Sort);
        LogicSymbolLoader::RegisterSort(TheSymbolTable, Sort);
    }

    void SymtabBuilder::VisitBVSortExpr(const BVSortExpr* Sort)
    {
        ASTVisitorBase::VisitBVSortExpr(Sort);
        LogicSymbolLoader::RegisterSort(TheSymbolTable, Sort);
    }

    void SymtabBuilder::VisitNamedSortExpr(const NamedSortExpr* Sort)
    {
        // Check that the named sort actually resolves to something
        auto STE = TheSymbolTable->LookupSort(Sort->GetName());
        if(STE == NULL || STE->GetKind() != STENTRY_SORT) {
            throw SynthLib2ParserException((string)"Sort name \"" + Sort->GetName() + "\" could not " + 
                                           "be resolved to anything meaningful.\n" + 
                                           Sort->GetLocation().ToString());            
        }
        auto SortE = STE->GetSort();
        if (TheSymbolTable->ResolveSort(SortE) == NULL) {
            throw SynthLib2ParserException((string)"Sort name \"" + Sort->GetName() + "\" could not " + 
                                           "be resolved to anything meaningful.\n" + 
                                           Sort->GetLocation().ToString());
        }
        // Do nothing otherwise
        return;
    }

    void SymtabBuilder::VisitArraySortExpr(const ArraySortExpr* Sort)
    {
        ASTVisitorBase::VisitArraySortExpr(Sort);
        LogicSymbolLoader::RegisterSort(TheSymbolTable, Sort);
    }

    void SymtabBuilder::VisitRealSortExpr(const RealSortExpr* Sort)
    {
        ASTVisitorBase::VisitRealSortExpr(Sort);
    }

    void SymtabBuilder::VisitFunSortExpr(const FunSortExpr* Sort)
    {
        ASTVisitorBase::VisitFunSortExpr(Sort);
    }

    void SymtabBuilder::VisitBoolSortExpr(const BoolSortExpr* Sort)
    {
        ASTVisitorBase::VisitBoolSortExpr(Sort);
    }

    void SymtabBuilder::VisitEnumSortExpr(const EnumSortExpr* Sort)
    {
        ASTVisitorBase::VisitEnumSortExpr(Sort);
        LogicSymbolLoader::RegisterSort(TheSymbolTable, Sort);
    }

    void SymtabBuilder::VisitLetBindingTerm(const LetBindingTerm* Binding)
    {
        // Check the sorts
        ASTVisitorBase::VisitLetBindingTerm(Binding);
      
        // let bound terms can shadow anything
        TheSymbolTable->BindLetVariable(Binding->GetVarName(),
                                        static_cast<Term*>(Binding->GetBoundToTerm()->Clone()));

        // Check that the sort of the term is what is expected
        auto TermSort = Binding->GetBoundToTerm()->GetTermSort(TheSymbolTable);

        auto ExpectedSort = Binding->GetVarSort();

        if (!TheSymbolTable->ResolveSort(TermSort)->Equals(*(TheSymbolTable->ResolveSort(ExpectedSort)))) {
            throw SynthLib2ParserException((string)"Let bound term \"" + Binding->GetVarName() + 
                                           "\" does not match the expected sort.\n" + 
                                           Binding->GetLocation().ToString());
        }

        return;
    }
        
    void SymtabBuilder::VisitFunTerm(const FunTerm* TheTerm)
    {
        ASTVisitorBase::VisitFunTerm(TheTerm);
    }

    void SymtabBuilder::VisitLiteralTerm(const LiteralTerm* TheTerm)
    {
        ASTVisitorBase::VisitLiteralTerm(TheTerm);
    }

    void SymtabBuilder::VisitSymbolTerm(const SymbolTerm* TheTerm)
    {
        ASTVisitorBase::VisitSymbolTerm(TheTerm);
    }

    void SymtabBuilder::VisitLetTerm(const LetTerm* TheTerm)
    {
        // Push a new scope
        TheSymbolTable->Push();
        // Visit the bindings to ensure that everything is in order
        // This process also creates the bindings in the current scope
        ASTVisitorBase::VisitLetTerm(TheTerm);
        // Push the scope to the let term
        TheTerm->SetScope(TheSymbolTable->Pop());
        return;
    }

    void SymtabBuilder::VisitLetBindingGTerm(const LetBindingGTerm* Binding)
    {
        // We don't handle this here. 
        // except for checking the sort exprs
        ASTVisitorBase::VisitLetBindingGTerm(Binding);
    }

    void SymtabBuilder::VisitFunGTerm(const FunGTerm* TheTerm)
    {
        ASTVisitorBase::VisitFunGTerm(TheTerm);
    }

    void SymtabBuilder::VisitLiteralGTerm(const LiteralGTerm* TheTerm)
    {
        ASTVisitorBase::VisitLiteralGTerm(TheTerm);
    }

    void SymtabBuilder::VisitSymbolGTerm(const SymbolGTerm* TheTerm)
    {
        ASTVisitorBase::VisitSymbolGTerm(TheTerm);
    }

    void SymtabBuilder::VisitLetGTerm(const LetGTerm* TheTerm)
    {
        ASTVisitorBase::VisitLetGTerm(TheTerm);
    }

    void SymtabBuilder::VisitConstantGTerm(const ConstantGTerm* TheTerm)
    {
        ASTVisitorBase::VisitConstantGTerm(TheTerm);
    }

    void SymtabBuilder::VisitVariableGTerm(const VariableGTerm* TheTerm)
    {
        ASTVisitorBase::VisitVariableGTerm(TheTerm);
    }

    void SymtabBuilder::VisitNTDef(const NTDef* Def)
    {
        ASTVisitorBase::VisitNTDef(Def);
    }

    void SymtabBuilder::VisitLiteral(const Literal* TheLiteral)
    {
        ASTVisitorBase::VisitLiteral(TheLiteral);
    }

    void SymtabBuilder::Do(const Program* Prog, SymbolTable* TheSymbolTable)
    {
        SymtabBuilder Builder(TheSymbolTable);
        Prog->Accept(&Builder);
    }

} /* end namespace */

