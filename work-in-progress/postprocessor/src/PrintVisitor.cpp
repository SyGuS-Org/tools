#include <PrintVisitor.hpp>
#include <SymbolTable.hpp>


namespace SynthLib2Parser {

    map<string,string> PrintVisitor::InitializeLogicGrammars()
    {
        map<string,string> logicGrammars;
        logicGrammars["LIA"] = "... grammara of LIA ...";

        ostringstream StrInt;
        StrInt << 
	    "ConstantInt" << endl <<
            "  (+ StartInt StartInt)" << endl <<
            "  (- StartInt StartInt)" << endl <<
            "  (* StartInt ConstantInt)" << endl <<
            "  (* ConstantInt StartInt)" << endl <<
        //    "  (div StartInt ConstantInt)" << endl <<
        //    "  (mod StartInt ConstantInt)" << endl <<
            "  (ite StartBool StartInt StartInt)" << endl;
        logicGrammars["StartInt"] =  StrInt.str();

        ostringstream StrCnst;
	StrCnst << "(Constant Int)" << endl; 
        logicGrammars["ConstantInt"] = StrCnst.str(); 

        ostringstream StrBool;
        StrBool <<  "true   false" << endl <<
                "  (and  StartBool StartBool)" << endl <<
                "  (or   StartBool StartBool)" << endl <<
                "  (=>   StartBool StartBool)" << endl <<
                "  (xor  StartBool StartBool)" << endl <<
                "  (xnor StartBool StartBool)" << endl <<
                "  (nand StartBool StartBool)" << endl <<
                "  (nor  StartBool StartBool)" << endl <<
                "  (iff  StartBool StartBool)" << endl <<
                "  (=    StartBool StartBool)" << endl <<
                "  (not  StartBool)" << endl <<
                "  (<= StartInt StartInt)" << endl <<
                "  (<  StartInt StartInt)" << endl <<                
                "  (=  StartInt StartInt)" << endl <<
                "  (>= StartInt StartInt)" << endl <<
                "  (>  StartInt StartInt)" << endl <<                
                "  (ite StartBool StartBool StartBool)" << endl;
        logicGrammars["StartBool"] = StrBool.str();                                                         

        return logicGrammars;
    }

    map<string,string> PrintVisitor::LogicGrammars = PrintVisitor::InitializeLogicGrammars();  


    PrintVisitor::PrintVisitor(ostream& Out, const SymbolTable* SymTbl, bool Compile, bool SMTlet)
      : ASTVisitorBase("PrintVisitor"), IndentLevel(0), Out(Out), 
	    TheSymbolTable(SymTbl), SMTlet(SMTlet), Compile(Compile)
    {
        // Nothing here
    }

    PrintVisitor::~PrintVisitor()
    {
         Out.flush();
    }

    inline string PrintVisitor::GetIndent()
    {
        string Retval(IndentLevel * 4, ' ');
        return Retval;
    }

    void PrintVisitor::VisitProgram(const Program* Prog)
    {
        for(auto const& Cmd : Prog->GetCmds()) {
            Cmd->Accept(this);
        }
        Out.flush();
    }

    void PrintVisitor::VisitFunDefCmd(const FunDefCmd* Cmd)
    {
        Out << GetIndent() << "(define-fun " << Cmd->GetFunName() << " (";
        for(auto const& ASPair : Cmd->GetArgs()) {
            ASPair->Accept(this);
        }
        Out << ") ";
        Cmd->GetSort()->Accept(this);
        Out << endl;
        IndentLevel++;
        Out << GetIndent();
        Cmd->GetTerm()->Accept(this);
        Out << endl;
        IndentLevel--;
        Out << ")" << endl << endl;
    }

    void PrintVisitor::VisitFunDeclCmd(const FunDeclCmd* Cmd)
    {
        Out << GetIndent() << "(declare-fun " << Cmd->GetFunName() << " (";
        for(auto const& Sort : Cmd->GetArgSorts()) {
            Sort->Accept(this);
        }
        Out << ") ";
        Cmd->GetSort()->Accept(this);
        Out << ")" << endl << endl;
    }

    void PrintVisitor::VisitSynthFunCmd(const SynthFunCmd* Cmd) 
    {
        Out << GetIndent() << "(synth-fun " << Cmd->GetFunName() << " (";
        for(auto const& ASPair : Cmd->GetArgs()) {
            ASPair->Accept(this);
        }
        Out << ") ";
        Cmd->GetSort()->Accept(this);
        Out << endl;
        IndentLevel++;        
        
        if (!Cmd->GetGrammarRules().empty()) {
            Out << GetIndent() << "(";
            IndentLevel++;
            for(auto const& Rule : Cmd->GetGrammarRules()) {
                Rule->Accept(this);
                Out << endl;
            }
            IndentLevel--;
            Out << ")" << endl;
        }
        /*  
        else if (Compile) { 
            string Error = "Currently the only implicit allowed grammar is LIA;"; 
            Error += " sorts are resitrincted to Bool and Int.";
            //Out << LogicGrammars[LogicName] << endl;
            if (Cmd->GetSort()->GetKind() == SORTKIND_BOOL) {
                Out << "((Start Bool (StartBool))" << endl;
            }
            else if (Cmd->GetSort()->GetKind() == SORTKIND_INT) {
                Out << "((Start Int (StartInt))" << endl;
            }
            else {
              ;// throw SynthLib2ParserException(Error); 
            }
            string BoolArgs = "";
            string IntArgs = "";
            for(auto const& ASPair : Cmd->GetArgs()) {
                if (ASPair->GetSort()->GetKind() == SORTKIND_BOOL)
                    BoolArgs += ASPair->GetName() + " ";
                else if (ASPair->GetSort()->GetKind() == SORTKIND_INT)
                    IntArgs += ASPair->GetName() + " ";
                else 
                 ; //  throw SynthLib2ParserException(Error);
            }
            Out << "(StartInt Int (" << IntArgs << LogicGrammars["StartInt"] << "))" << endl << endl;
            Out << "(ConstantInt Int ("  << LogicGrammars["ConstantInt"] << "))" << endl;
            Out << "(StartBool Bool (" << BoolArgs << LogicGrammars["StartBool"] << ")))" << endl;
            
        }*/
        
        IndentLevel--;
        Out << ")" << endl << endl;
    }

     void PrintVisitor::VisitSynthInvCmd(const SynthInvCmd* Cmd) 
    {  

        string CmdName = Compile? "synth-fun" : "synth-inv";
        string RetSort = Compile? " Bool\n" : "\n ";

        Out << GetIndent() << "(" << CmdName << " " << Cmd->GetInvName() << " (";
        for(auto const& ASPair : Cmd->GetArgs()) {
            ASPair->Accept(this);
        }
        Out << ")" << RetSort; 
        IndentLevel++;
        
        if (!Cmd->GetGrammarRules().empty()) {
            Out << GetIndent() << "(";
            IndentLevel++;
            for(auto const& Rule : Cmd->GetGrammarRules()) {
                Rule->Accept(this);
                Out << endl;
            }
            IndentLevel--;
            Out << ")" << endl;
        }
        // else if (Compile) { 
        //     string Error = "Currently the only implicit allowed grammar is LIA;"; 
        //     Error += " sorts are restricted to Bool and Int.";
        //     //Out << LogicGrammars[LogicName] << endl;
            
        //     Out << "((Start Bool (StartBool))" << endl;
        //     string BoolArgs = "";
        //     string IntArgs = "";
        //     for(auto const& ASPair : Cmd->GetArgs()) {
        //         if (ASPair->GetSort()->GetKind() == SORTKIND_BOOL)
        //             BoolArgs += ASPair->GetName() + " ";
        //         else if (ASPair->GetSort()->GetKind() == SORTKIND_INT)
        //             IntArgs += ASPair->GetName() + " ";
        //         else 
        //             throw SynthLib2ParserException(Error);
        //     }
        //     Out << "(StartInt Int (" << IntArgs << LogicGrammars["StartInt"] << "))" << endl << endl;
        //     Out << "(ConstantInt Int (" << LogicGrammars["ConstantInt"] << "))" << endl;
        //     Out << "(StartBool Bool (" << BoolArgs << LogicGrammars["StartBool"] << ")))" << endl;
        // }

        IndentLevel--;
        Out << ")" << endl << endl;
    }   

    void PrintVisitor::VisitSortDefCmd(const SortDefCmd* Cmd)
    {
        Out << GetIndent() << "(define-sort " << Cmd->GetName() << " ";
        Cmd->GetSortExpr()->Accept(this);
        Out << ")" << endl << endl;
    }

    void PrintVisitor::VisitSetOptsCmd(const SetOptsCmd* Cmd)
    {
        Out << GetIndent() << "(set-opts (";
        IndentLevel++;
        for(auto const& Opt : Cmd->GetOpts()) {
            Out << endl << GetIndent() << "(" << Opt.first << " \"" << Opt.second << "\")";
        }
        Out << endl;
        IndentLevel--;
        Out << GetIndent() << "))" << endl << endl;
    }

    void PrintVisitor::VisitVarDeclCmd(const VarDeclCmd* Cmd)
    {
        Out << GetIndent() << "(declare-var " << Cmd->GetName() << " ";
        Cmd->GetSort()->Accept(this);
        Out << ")" << endl << endl;
    }

    void PrintVisitor::VisitPrimedVarDeclCmd(const PrimedVarDeclCmd* Cmd)
    {
        if (!Compile) {
          Out << GetIndent() << "(declare-primed-var " << Cmd->GetName() << " ";
          Cmd->GetSort()->Accept(this);
          Out << ")" << endl << endl;
        }
        else {
          Out << GetIndent() << "(declare-var " << Cmd->GetName() << " ";
          Cmd->GetSort()->Accept(this);
          Out << ")" << endl;
          Out << GetIndent() << "(declare-var " << Cmd->GetName() << "! ";
          Cmd->GetSort()->Accept(this);
          Out << ")" << endl << endl;
        }
    }


    void PrintVisitor::VisitConstraintCmd(const ConstraintCmd* Cmd) 
    {
        Out << "(constraint " << endl;
        IndentLevel++;
        Cmd->GetTerm()->Accept(this);
        IndentLevel--;
        Out << endl << GetIndent() << ")" << endl << endl;
    }

    void PrintVisitor::VisitInvConstraintCmd(const InvConstraintCmd* Cmd) 
    {
    
       if (!Compile) {
            Out << "(inv-constraint " 
                << Cmd->GetSynthInvName() << " "
                << Cmd->GetPreCondName() << " "
                << Cmd->GetTransName() << " "
                << Cmd->GetPostCondName() << ") " <<endl;     
            return;
        }
        
        // else translate it to general SyGuS
        // (assumes "implies" is defined)
        // compile three constratins, for precond, trans, and postcond

        // Retrieve args and prepare a duplicate for use for trans
        string InvName = Cmd->GetSynthInvName();
        auto STE = TheSymbolTable->LookupInv(InvName);
        FunSortExpr* FSR = STE->GetInvSort();
        vector<const SortExpr*> Args = FSR->GetArgSorts();        

        const u32 NumArgs = Args.size();
        vector<const SortExpr*> ArgsArgs(2*NumArgs);
        for(u32 i = 0; i < NumArgs; ++i) { ArgsArgs[i] = Args[i];}
        for(u32 i = 0; i < NumArgs; ++i) { ArgsArgs[NumArgs+i] = ArgsArgs[i];}    
        
        // Find PreCond, print that it implies the invariant    

        auto STEpre = TheSymbolTable->LookupFun(Cmd->GetPreCondName(), Args);
        FunDefCmd* PreCond = STEpre->GetFunDef();  
        ostringstream PreArgsStr;
        for(auto const& ASPair : PreCond->GetArgs()) {
            PreArgsStr << " " << ASPair->GetName();
        } 

        Out << "(constraint (=>\n";
        Out << "            (" << PreCond->GetFunName() <<  PreArgsStr.str() << ")\n";
        Out << "            (" << InvName               <<  PreArgsStr.str() << ")\n";
        Out << ")) " <<endl <<endl; 

        // Find Trans, print that invariant and trans implies primed-invariant    

        auto STEtrans = TheSymbolTable->LookupFun(Cmd->GetTransName(), ArgsArgs);
        FunDefCmd* TransCond = STEtrans->GetFunDef();
        ostringstream AllTransArgs;
        ostringstream PrimedTransArgs;  
        ostringstream UnprimedTransArgs;    
        auto ASPairs = TransCond->GetArgs(); 
        for(u32 i=0; i < NumArgs; i++) {
            AllTransArgs << " " << ASPairs[i]->GetName();
            UnprimedTransArgs << " " << ASPairs[i]->GetName();
        } 
        for(u32 i=0; i < NumArgs; i++) {
            AllTransArgs << " " << ASPairs[NumArgs+i]->GetName(); 
            PrimedTransArgs << " " << ASPairs[NumArgs+i]->GetName(); 
        } 

        Out << "(constraint (=> \n";
        Out << "            (and (" << InvName << UnprimedTransArgs.str() << ")\n";
        Out << "                 (" << TransCond->GetFunName() << AllTransArgs.str() << "))\n";    
        Out << "            (" << InvName << PrimedTransArgs.str() << ")";
        Out << ")) " <<endl <<endl;    

        // Find PostCond, print that invariant implies it    

        auto STEpost = TheSymbolTable->LookupFun(Cmd->GetPostCondName(), Args);
        FunDefCmd* PostCond = STEpost->GetFunDef();  
        ostringstream PostArgsStr;
        for(auto const& ASPair : PostCond->GetArgs()) {
            PostArgsStr << " " << ASPair->GetName();
        } 

        Out << "(constraint (=>\n";
        Out << "            (" << InvName               <<  PostArgsStr.str() << ")\n";        
        Out << "            (" << PostCond->GetFunName() <<  PostArgsStr.str() << ")\n";
        Out << ")) " <<endl <<endl; 
    }


    void PrintVisitor::VisitSetLogicCmd(const SetLogicCmd* Cmd)
    {
        Out << GetIndent() << "(set-logic " << Cmd->GetLogicName() << ")" << endl << endl;
    }

    void PrintVisitor::VisitCheckSynthCmd(const CheckSynthCmd* Cmd)
    {
        Out << GetIndent() << "(check-synth)" << endl << endl;
    }

    void PrintVisitor::VisitArgSortPair(const ArgSortPair* ASPair) 
    {
        Out << "(" << ASPair->GetName() << " ";
        ASPair->GetSort()->Accept(this);
        Out << ")";
    }
    
    void PrintVisitor::VisitIntSortExpr(const IntSortExpr* Sort)
    {
        Out << "Int";
    }

    void PrintVisitor::VisitStringSortExpr(const StringSortExpr* Sort)
    {
        Out << "String";
    }

    void PrintVisitor::VisitBVSortExpr(const BVSortExpr* Sort)
    {
	if (Compile) // output needs to by SyGuS format
	        Out << "(BitVec " << Sort->GetSize() << ")";
	else        // output needs to be SMT format
        	Out << "(_ BitVec " << Sort->GetSize() << ")";
    }

    void PrintVisitor::VisitNamedSortExpr(const NamedSortExpr* Sort)
    {
        Out << Sort->GetName();
    }

    void PrintVisitor::VisitArraySortExpr(const ArraySortExpr* Sort) 
    {
        Out << "(Array ";
        Sort->GetDomainSort()->Accept(this);
        Out << " ";
        Sort->GetRangeSort()->Accept(this);
        Out << ")";
    }

    void PrintVisitor::VisitRealSortExpr(const RealSortExpr* Sort)
    {
        Out << "Real";
    }

    void PrintVisitor::VisitFunSortExpr(const FunSortExpr* Sort)
    {
        // Do nothing!
    }

    void PrintVisitor::VisitBoolSortExpr(const BoolSortExpr* Sort)
    {
        Out << "Bool";
    }

    void PrintVisitor::VisitEnumSortExpr(const EnumSortExpr* Sort)
    {
        Out << "(Enum (";
        for(auto const& Con : Sort->GetConstructors()) {
            Out << Con << " ";
        }
        Out << "))";
    }
    
    void PrintVisitor::VisitLetBindingTerm(const LetBindingTerm* Binding)
    {
        Out << "(" << Binding->GetVarName() << " ";
	if (!SMTlet) {
	  Binding->GetVarSort()->Accept(this);
          Out << " ";
	}
        Binding->GetBoundToTerm()->Accept(this);
        Out << ")";
    }
    
    void PrintVisitor::VisitFunTerm(const FunTerm* TheTerm)
    {
        Out << "(" << TheTerm->GetFunName();
        for(auto const& Arg : TheTerm->GetArgs()) {
            Out << " ";
            Arg->Accept(this);
        }
        Out << ")";
    }

    void PrintVisitor::VisitLiteralTerm(const LiteralTerm* TheTerm)
    {
        TheTerm->GetLiteral()->Accept(this);
     }

    void PrintVisitor::VisitSymbolTerm(const SymbolTerm* TheTerm) 
    {
        Out << TheTerm->GetSymbol();
    }

    void PrintVisitor::VisitLetTerm(const LetTerm* TheTerm)
    {
        Out << "(let (";
        IndentLevel++;
        for(auto const& Binding : TheTerm->GetBindings()) {
            Binding->Accept(this);
        }
        Out << ")" << endl;
        Out << GetIndent();
        TheTerm->GetBoundInTerm()->Accept(this);
        IndentLevel--;
        Out << endl << GetIndent() << ")";
    }
    
    void PrintVisitor::VisitLetBindingGTerm(const LetBindingGTerm* Binding) 
    {
        Out << "(" << Binding->GetVarName() << " ";
        Binding->GetVarSort()->Accept(this);
        Out << " ";
        Binding->GetBoundToTerm()->Accept(this);
        Out << ")";
    }
    
    void PrintVisitor::VisitFunGTerm(const FunGTerm* TheTerm) 
    {
        Out << "(" << TheTerm->GetName();
        for(auto const& Arg : TheTerm->GetArgs()) {
            Out << " ";
            Arg->Accept(this);
        }
        Out << ")" << endl;
    }

    void PrintVisitor::VisitLiteralGTerm(const LiteralGTerm* TheTerm) 
    {
        TheTerm->GetLiteral()->Accept(this);
    }

    void PrintVisitor::VisitSymbolGTerm(const SymbolGTerm* TheTerm) 
    {
        Out << TheTerm->GetSymbol();
    }

    void PrintVisitor::VisitLetGTerm(const LetGTerm* TheTerm) 
    {
        Out << "(let (" << endl;
        IndentLevel++;
        for(auto const& Binding : TheTerm->GetBindings()) {
            Binding->Accept(this);
        }
        Out << ")" << endl;
        Out << GetIndent();
        TheTerm->GetBoundInTerm()->Accept(this);
        IndentLevel--;
        Out << endl << GetIndent() << ")";        
    }

    void PrintVisitor::VisitConstantGTerm(const ConstantGTerm* TheTerm) 
    {
        Out << "(Constant ";
        TheTerm->GetSort()->Accept(this);
        Out << ")";
    }

    void PrintVisitor::VisitVariableGTerm(const VariableGTerm* TheTerm) 
    {
        switch (TheTerm->GetKind()) {
        case VARKIND_LOCAL:
            Out << "LocalVariable ";
            break;
        case VARKIND_INPUT:
            Out << "InputVariable ";
            break;
        case VARKIND_ANY:
            Out << "Variable ";
            break;
        }

        TheTerm->GetSort()->Accept(this);
        Out << ")";
    }
    
    void PrintVisitor::VisitNTDef(const NTDef* Def) 
    {
        Out << "(" << Def->GetName() << " ";
        Def->GetSort()->Accept(this);
        Out << " (" << endl;
        IndentLevel++;
        for(auto const& Expansion : Def->GetExpansions()) {
            Out << GetIndent();
            Expansion->Accept(this);
        }
        IndentLevel--;
        Out << "))";
        Out << endl << GetIndent();
    }

    void PrintVisitor::VisitLiteral(const Literal* TheLiteral) 
    {
        Out << TheLiteral->GetLiteralString();
    }

    // The << operator for AST bases
    ostream& operator << (ostream& Out, const ASTBase& AST)
    {
        PrintVisitor Printer(Out);
        AST.Accept(&Printer);
        return Out;
    }

    // The << operator for source locations
    ostream& operator << (ostream& Out, const SourceLocation& Location)
    {
        Out << Location.ToString();
        return Out;
    }


} /* end namespace */

