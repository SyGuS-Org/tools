#include <InVisitor.hpp>
#include <PrintVisitor.hpp>
#include <SymbolTable.hpp>

namespace SynthLib2Parser {

    InVisitor::InVisitor(ostream& Out, SymbolTable* SymTbl, CompTrack Track)
      : ASTVisitorBase("InVisitor"), IndentLevel(0), Out(Out), 
	SymTbl(SymTbl), InConstraint(false), InConstraintEquiv(false), 
        SynthFunFound(false), NumSynthCmds(0), 
        SyntacticRestrictionsGiven(false), InvConstraintCmdSpecified(false),
        SynthInvGiven(false), NonExampleConstraint(false), Track(Track)
    {
        // Nothing here
    }

    InVisitor::~InVisitor()
    {
        Out.flush();
    }

  const ASTCmd* InVisitor::GetCmd(u32 i) { 
    return Cmds[i]; 
  }
  
  void InVisitor::SetCmd(u32 i, const ASTCmd* c) { 
    Cmds[i] = c; 
  }
  
  u32 InVisitor::CmdsSize() { 
    return Cmds.size(); 
  }

  string InVisitor::GetCmdAuxString(u32 i) {
    return CmdAuxString[i];
  }

  void InVisitor::VisitProgram(const Program* Prog) {
    for(auto const& Cmd : Prog->GetCmds()) {
      Cmd->Accept(this);
    }
  }

  void InVisitor::VisitSynthFunCmd(const SynthFunCmd* Cmd) {
    Cmds.push_back(Cmd);
    CmdAuxString.push_back("");
    NumSynthCmds++;

    if (!Cmd->GetGrammarRules().empty()) { 
      SyntacticRestrictionsGiven = true;
    }
    else if (Cmd->GetGrammarRules().empty()) { 
            string Error = "Currently the only implicit allowed grammar is LIA;"; 
            Error += " sorts are resitrincted to Bool and Int.";
            if (Cmd->GetSort()->GetKind() != SORTKIND_BOOL && 
                Cmd->GetSort()->GetKind() != SORTKIND_INT) {
                //throw SynthLib2ParserException(Error); 
            }
    }
  }

  void InVisitor::VisitSynthInvCmd(const SynthInvCmd* Cmd) {
    SynthInvGiven = true;
    Cmds.push_back(Cmd);
    CmdAuxString.push_back("");
    NumSynthCmds++;

    if (!Cmd->GetGrammarRules().empty()) { 
      SyntacticRestrictionsGiven = true;
    }
    else if (Cmd->GetGrammarRules().empty()) { 
            string Error = "Currently the only implicit allowed grammar is LIA;"; 
            Error += " sorts are resitrincted to Bool and Int.";
    }
  }

  void InVisitor::VisitFunDefCmd(const FunDefCmd* Cmd) {
    Cmds.push_back(Cmd);
    CmdAuxString.push_back("");
  }

  void InVisitor::VisitFunDeclCmd(const FunDeclCmd* Cmd) {
    Cmds.push_back(Cmd);
    CmdAuxString.push_back("");
  }

  void InVisitor::VisitSortDefCmd(const SortDefCmd* Cmd) {
    Cmds.push_back(Cmd);
    CmdAuxString.push_back("");
  }

  void InVisitor::VisitSetLogicCmd(const SetLogicCmd* Cmd) {
    std::size_t found = (Cmd->GetLogicName().find("BV") != string::npos);
    if (found) {
      Logic = "(set-logic BV)";
      LogicName = "BV";
    }
    else {
      found = (Cmd->GetLogicName().find("SLIA") != string::npos);
      if (found) {
	Logic = "(set-logic SLIA)";
	LogicName = "SLIA";
      }
      else {	
      	Logic = "(set-logic AUFLIA)";
      	LogicName = "LIA";
      }
   }
  }

 void InVisitor::VisitConstraintCmd(const ConstraintCmd* Cmd) {

   // Push constraint cmd to Cmds list.
   // In addition visit the term and collect all
   // input vairables.
   // Once list is complete create an assertion string
   // (assert (forall (<collected input vars>) <constraint-term>))
   // and push it to the CmdAuxStrings.

    Cmds.push_back(Cmd);
    InConstraint = true;
    SynthFunFound = false;
    // clean FoallVars aux field for the current use.
    ForallVars.erase(ForallVars.begin(),ForallVars.end());

    Cmd->GetTerm()->Accept(this);

    if (ForallVars.size() == 0) {
      CmdAuxString.push_back("");
      InConstraint = false;
      return;
    }

    ostringstream s;
    s << "(forall (";

    for (auto v : ForallVars) {
      s << "(" << v;
      switch (SymTbl->LookupVariable(v)->GetSort()->GetKind()) {
      case SORTKIND_INT:
    	   s << " Int)";
    	   break;
      case SORTKIND_BOOL:
	       s << " Bool)";
	       break;
      case SORTKIND_BV:
	       //	BVSortExpr* bvs = dynamic_cast<const BVSortExpr*>(SymTbl->LookupVariable(v)->GetSort());
	       s << " (_ BitVec " << 32 << "))";
	       break;
      case SORTKIND_STRING:
               s << " String)";
               break;      
      default:
	       throw SynthLib2ParserException((string)"Sorts other than Int, Bool, BitVec, String are not yet supported.");	
	       break;
      }
    }
    s << ")\n           ";
    CmdAuxString.push_back(s.str());
    InConstraint = false;
  }

  
  void InVisitor::VisitInvConstraintCmd(const InvConstraintCmd* Cmd) {
    InvConstraintCmdSpecified = true;

    if (Track != CMPTRK_INV) {
      throw SynthLib2ParserException((string)"inv-constraint is only allowed in invariant synthesis track.");

    }
    else {
      // nothing to check, parser have verified the argumemts of 
      // inv-constraint corrrespond to user defined functions 
    } 

  }

  void InVisitor::VisitVarDeclCmd(const VarDeclCmd* Cmd) {
    // Cmds.push_back(Cmd);
    // CmdAuxString.push_back("");
  }

  
  void InVisitor::VisitPrimedVarDeclCmd(const PrimedVarDeclCmd* Cmd) {
    // Cmds.push_back(Cmd);
    // CmdAuxString.push_back("");
  }

  void InVisitor::VisitSymbolTerm(const SymbolTerm* TheTerm)
  {
    string s = TheTerm->GetSymbol();
    if (SymTbl->LookupVariable(s)->GetKind() == STENTRY_QVARIABLE)
      ForallVars.insert(s);
  }

  void InVisitor::VisitLetBindingTerm(const LetBindingTerm* Binding)
  {
    Binding->GetBoundToTerm()->Accept(this);
  }

  void InVisitor::VisitFunTerm(const FunTerm* TheTerm)
  {
    for(auto const& Arg : TheTerm->GetArgs()) {
      Out << " ";
      // In CMPTRK_PBE a constraint should be 
      // an equality with LHS one of the function to be synthesized	
      // e.g.:   (constraint (= (f "abcde" "ab" "xx") "xxcde"))	
      if (Track==CMPTRK_PBE && InConstraint) {
	  if (!InConstraintEquiv && !SynthFunFound) {
	     if (TheTerm->GetFunName() == "=") {
	       InConstraintEquiv = true;
               Arg->Accept(this);
               InConstraintEquiv = false;   
             }
             else {
               NonExampleConstraint = true;
               Arg->Accept(this);
             }
          }
	  else if (!SynthFunFound) {
             const string f = TheTerm->GetFunName();
             auto const& Args = TheTerm->GetArgs();
             const u32 NumArgs = Args.size();
             vector<const SortExpr*> ArgSorts(NumArgs);
             for (u32 i = 0; i < NumArgs; ++i) {
                ArgSorts[i] = Args[i]->GetTermSort(SymTbl);
             }
             auto Entry = SymTbl->LookupFun(f, ArgSorts);
             assert (Entry != NULL); 
             auto EntryKind = Entry->GetKind();
             if (EntryKind != STENTRY_SYNTH_FUNCTION)
	        NonExampleConstraint = true;
             else
	        SynthFunFound = true;
             Arg->Accept(this);      
          }
      }
     else {
         Arg->Accept(this);  
     }	
    }
  }

  void InVisitor::VisitLetTerm(const LetTerm* TheTerm)
  {
    SymTbl->Push();
    for(auto const& Binding : TheTerm->GetBindings()) {
	    SymTbl->BindLetVariable(Binding->GetVarName(),
                                    static_cast<Term*>(Binding->GetBoundToTerm()->Clone()));

            // Check that the sort of the term is what is expected
            auto TermSort = Binding->GetBoundToTerm()->GetTermSort(SymTbl);
            auto ExpectedSort = Binding->GetVarSort();
            if (!SymTbl->ResolveSort(TermSort)->Equals(*(SymTbl->ResolveSort(ExpectedSort)))) {
               throw SynthLib2ParserException((string)"Let bound term \"" + Binding->GetVarName() +
                                           "\" does not match the expected sort.\n" +
                                           Binding->GetLocation().ToString());
            }

    }
    TheTerm->GetBoundInTerm()->Accept(this);
    SymTbl->Pop();
  }

  const bool InVisitor::AreSyntacticRestrictionsGiven()
    {
        return SyntacticRestrictionsGiven;
    }

    const string InVisitor::GetLogicName() {
        return LogicName;
    }

    const bool InVisitor::ExistsInvConstraintCmd() {
        return InvConstraintCmdSpecified;
    }

    const bool InVisitor::ExistsNonExampleConstraints() {
        return NonExampleConstraint;
    }

    const bool InVisitor::ExistsSynthInvCmd() {
        return SynthInvGiven;
    }

} /* end namespace */

