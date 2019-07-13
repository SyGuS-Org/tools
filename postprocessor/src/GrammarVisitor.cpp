#include <GrammarVisitor.hpp>
#include <SymbolTable.hpp>
#include <SynthLib2ParserExceptions.hpp>
#include <InVisitor.hpp>
#include <OutVisitor.hpp>
#include <PrintVisitor.hpp>
#include <cctype>
#include <algorithm>

namespace SynthLib2Parser {

  GrammarVisitor::GrammarVisitor(ostream& Out, GTerm* GrmrTerm, const SynthFunCmd* SynthFun, 
				 const FunDefCmd* FunDef, bool noGrmrRestrictions, SymbolTable* SymTbl)
        : ASTVisitorBase("GrammarVisitor"), IndentLevel(0), Out(Out),  
	  SymTbl(SymTbl), noGrmrRestrictions(noGrmrRestrictions), SynthFun(SynthFun), FunDef(FunDef), 
	  GrmrTerm(GrmrTerm), Matched(false), ExprSize(0)
    {
      // Bind the arguments

      unsigned s = SynthFun->GetArgs().size();

      auto t = FunDef->GetArgs().begin();
      auto g = SynthFun->GetArgs().begin();

      for (unsigned k = 0; k < s  ; k++) {
	VarsBinding[(*g)->GetName()] = (*t)->GetName();
	t++; g++;
      }

    }

    GrammarVisitor::~GrammarVisitor()
    {
        Out.flush();
    }

    inline string GrammarVisitor::GetIndent()
    {
        string Retval(IndentLevel * 4, ' ');
        return Retval;
    }

    inline bool  GrammarVisitor::GetMatched() 
    {   
        return Matched; 
    }
 
    void  GrammarVisitor::SetMatched(bool b)
    {
        Matched = b;
	if (b)
	  ExprSize++;
    }

    u32 GrammarVisitor::GetExprSize() { 
      // No Grmr Restrictions 
      if (noGrmrRestrictions) 
         return ExprSize;
      // Grmr Restrictions Given
      if (!Matched) 
	return 0; 
      else 
	return ExprSize;
    }

    void GrammarVisitor::VisitProgram(const Program* Prog)
    {
        for(auto const& Cmd : Prog->GetCmds()) {
            Cmd->Accept(this);
        }
        Out.flush();
    }

    void GrammarVisitor::VisitFunDefCmd(const FunDefCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"define-fun is not allowed within a term.");
    }

    void GrammarVisitor::VisitFunDeclCmd(const FunDeclCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"declare-fun is not allowed within a term.");
    }

    void GrammarVisitor::VisitSynthFunCmd(const SynthFunCmd* Cmd) 
    {
      throw SynthLib2ParserException((string)"synth-fun is not allowed within a term.");
    }

    void GrammarVisitor::VisitSortDefCmd(const SortDefCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"define-sort is not allowed within a term.");
    }

    void GrammarVisitor::VisitSetOptsCmd(const SetOptsCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"set-opts is not allowed within a term.");
    }

    void GrammarVisitor::VisitVarDeclCmd(const VarDeclCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"declare-var is not allowed within a term.");
    }

    void GrammarVisitor::VisitConstraintCmd(const ConstraintCmd* Cmd) 
    {
      throw SynthLib2ParserException((string)"constraint is not allowed within a term.");
    }

    void GrammarVisitor::VisitSetLogicCmd(const SetLogicCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"set-logic is not allowed within a term.");
    }

    void GrammarVisitor::VisitCheckSynthCmd(const CheckSynthCmd* Cmd)
    {
      throw SynthLib2ParserException((string)"check-synth is not allowed within a term.");
    }

    void GrammarVisitor::VisitArgSortPair(const ArgSortPair* ASPair) 
    {
      throw SynthLib2ParserException((string)"ArgSortPair is not allowed within a term.");
    }
    
    void GrammarVisitor::VisitIntSortExpr(const IntSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitStringSortExpr(const StringSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitBVSortExpr(const BVSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitNamedSortExpr(const NamedSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitArraySortExpr(const ArraySortExpr* Sort) 
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitRealSortExpr(const RealSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitFunSortExpr(const FunSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitBoolSortExpr(const BoolSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitEnumSortExpr(const EnumSortExpr* Sort)
    {
      throw SynthLib2ParserException((string)"A sort definition is not allowed within a term.");
    }

    void GrammarVisitor::VisitLetBindingTerm(const LetBindingTerm* Binding)
    {
      throw SynthLib2ParserException((string)"LetBindingTerm is handled within LetTerm.");
    }
    
    void GrammarVisitor::VisitFunTerm(const FunTerm* TheTerm)
    {
      if (GrmrTerm->GetKind() != GTERMKIND_FUN) {
	SetMatched(false);
	return;
      }
      
      FunGTerm* FunGterm = dynamic_cast <FunGTerm*> (GrmrTerm);
      
      if (TheTerm->GetFunName() != FunGterm->GetName()) {
	SetMatched(false);
	return;
      }

      unsigned l = FunGterm->GetArgs().size();
      if (l != TheTerm->GetArgs().size()) {
	SetMatched(false);
	return;
      }

      bool AllAdhere = true;
      for (unsigned k = 0; k < l; k++) {
	GrmrTerm = FunGterm->GetArgs()[k];
	Term* ArgTerm = TheTerm->GetArgs()[k];
	SetMatched(false);
	bool Adheres = AdheresToGrammar(ArgTerm);
	AllAdhere &= Adheres;
	if (!Adheres)
	  break;
      }
      
      SetMatched(AllAdhere);
      return;
    }

  bool same_bv_match(const string bv1, const string bv2) {
	  u32 k=0;
	  for (u32 i=0; i < bv2.length(); i++)
	    k += tolower(bv1[i]) == tolower(bv2[i]);
	  return (k == bv1.length());
  }

  string bin_to_hex(const string bv) {
    stringstream res;
    for (int i = 0; i < bv.length(); i += 4)
      res << hex << std::stoi(bv.substr(i, 4), nullptr, 2);
    return res.str();
  }

  bool bv_match(const string bv1, const string bv2) {
    string hbv1 = tolower(bv1[1]) == 'x' ? bv1.substr(2) : bin_to_hex(bv1.substr(2));
    string hbv2 = tolower(bv2[1]) == 'x' ? bv2.substr(2) : bin_to_hex(bv2.substr(2));
    return same_bv_match(hbv1, hbv2);
  }

    void GrammarVisitor::VisitLiteralTerm(const LiteralTerm* TheTerm)
    {
      if (GrmrTerm->GetKind() == GTERMKIND_LITERAL) {
	auto LitG = dynamic_cast <LiteralGTerm*> (GrmrTerm);
	string Gstr = LitG->GetLiteral()->GetLiteralString();
	string Tstr = TheTerm->GetLiteral()->GetLiteralString();
	bool matched = Gstr==Tstr;
	if (!matched && Tstr[0]=='#' && (Tstr[1]=='x' || Tstr[1]=='X' || Tstr[1]=='b' || Tstr[1]=='B')) {
    matched = bv_match(Gstr, Tstr);
	} 
	SetMatched(matched);
      }
      else if (GrmrTerm->GetKind() == GTERMKIND_CONSTANT) {
	auto CnstG = dynamic_cast <ConstantGTerm*> (GrmrTerm);
	SetMatched(TheTerm->GetLiteral()->GetSort(SymTbl)->GetKind() == CnstG->GetSort()->GetKind());
      }
    }

  void GrammarVisitor::VisitSymbolTerm(const SymbolTerm* TheTerm) 
  {
    if (GrmrTerm->GetKind() == GTERMKIND_SYMBOL) {
      auto SymG = dynamic_cast <SymbolGTerm*> (GrmrTerm);
      SetMatched(TheTerm->GetSymbol() == Lookup(SymG->GetSymbol()));
    }
    else if (GrmrTerm->GetKind() == GTERMKIND_VARIABLE) {
      auto VarG = dynamic_cast <VariableGTerm*> (GrmrTerm);
      bool SameSortKind = TheTerm->GetTermSort(SymTbl)->GetKind() == VarG->GetSort()->GetKind();
      const SymbolTableEntry* TermSTE = SymTbl->LookupVariable(TheTerm->GetSymbol());
      if (TermSTE == NULL) {
	SetMatched(false);
	return;
	}
      bool SameVarKind = false;

      SymtabEntryKind TermSTEkind = TermSTE->GetKind();
      switch (VarG->GetKind()) {
      case VARKIND_ANY:
	SameVarKind = true;
	break;
      case VARKIND_INPUT:
	SameVarKind = TermSTEkind == STENTRY_QVARIABLE;
	break;
      case VARKIND_LOCAL:
	SameVarKind = TermSTEkind == STENTRY_BVARIABLE;
	break;
	}
      SetMatched(SameSortKind && SameVarKind);
    }
  }


  void GrammarVisitor::VisitLetTerm(const LetTerm* TheTerm)
  {
    if (GrmrTerm->GetKind() != GTERMKIND_LET) {
      SetMatched(false);
      return;
    } 

    auto LetGterm = dynamic_cast <LetGTerm*> (GrmrTerm);

    u32 s = TheTerm->GetBindings().size();
    
    if (LetGterm->GetBindings().size() != s) {
      SetMatched(false);
      return;
    }

    // we need to go over all permutations of size s,
    // and check that for one of them there is a match
    // we will use array perm for that

    u32* perm = new u32[s];

    for (u32 i = 0; i<s; i++) perm[i]=i;
	
    do {
      //cout << endl << "Trying permutaton [ "; 
      //for (u32 i = 0; i<s; i++) cout << perm[i] << " ";
      //cout << "]" << endl;

      for (u32 i = 0; i < s; i++) {
        string GrmrArgName = LetGterm->GetBindings()[perm[i]]->GetVarName();
        UnBind(GrmrArgName);
        //cout << "release " << GrmrArgName << endl;
      }
   
      u32 m = 0;
      for (u32 i = 0; i < s; i++) {
	LetBindingTerm* T = TheTerm->GetBindings()[i];
	LetBindingGTerm* G = LetGterm->GetBindings()[perm[i]];
        string Tname = T->GetVarName();
	string Gname = G->GetVarName();
	if (Lookup(G->GetVarName()) != "") {
	  //cout << "name " <<  Gname <<  " already exits!" << endl; 	
	  SetMatched(false);
	  break;
	}
	// verify kind matches
	else if (T->GetVarSort()->GetKind() != G->GetVarSort()->GetKind()) {
	  //cout << "kind does NOT match" << endl;
	  //cout << "T is " << T->GetVarName() << " and G is " << G->GetVarName() << endl;
	  SetMatched(false);
	  break;
	}
	else {
	  //cout << "allocate the new bindings: G = " << Gname << ", T = " << Tname << endl;
	  VarsBinding[G->GetVarName()] = T->GetVarName();
	  GrmrTerm = G->GetBoundToTerm();
	  bool Adheres = AdheresToGrammar(T->GetBoundToTerm());
	  //cout << "Adheres = " << Adheres << endl;
	  if (Adheres) 
	    m++;
	  else
	    break;
	}
      }
      
      Matched = m==s;

      if (Matched) {
	SetMatched(false);
	GrmrTerm = LetGterm->GetBoundInTerm();
	SetMatched(AdheresToGrammar(TheTerm->GetBoundInTerm()));
      }
      
      for (u32 i = 0; i < m; i++) {
	string GrmrArgName = LetGterm->GetBindings()[perm[i]]->GetVarName();
	UnBind(GrmrArgName);
	//	cout << "release " << GrmrArgName << endl;
      }
      
      if (Matched)
	break;
    } while (std::next_permutation(perm,perm+s)); 

    delete[] perm;
    return;
  }

  void GrammarVisitor::VisitLetBindingGTerm(const LetBindingGTerm* Binding) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }
    
  void GrammarVisitor::VisitFunGTerm(const FunGTerm* TheTerm) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }

  void GrammarVisitor::VisitLiteralGTerm(const LiteralGTerm* TheTerm) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }

  void GrammarVisitor::VisitSymbolGTerm(const SymbolGTerm* TheTerm) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }

  void GrammarVisitor::VisitLetGTerm(const LetGTerm* TheTerm) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }

  void GrammarVisitor::VisitConstantGTerm(const ConstantGTerm* TheTerm) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }

  void GrammarVisitor::VisitVariableGTerm(const VariableGTerm* TheTerm) 
  {
    throw SynthLib2ParserException((string)"GTerms should not be visited.");
  }
    
  void GrammarVisitor::VisitNTDef(const NTDef* Def) 
  {
    throw SynthLib2ParserException((string)"NTDef should not be visited.");
  }

  void GrammarVisitor::VisitLiteral(const Literal* TheLiteral) 
  {
    throw SynthLib2ParserException((string)"Literal should not be visited.");
  }

  const string GrammarVisitor::Lookup(const string& GTermStr) {
    auto it = VarsBinding.find(GTermStr);
    if(it == VarsBinding.end()) {
      return "";
    }
    return it->second;
  }

  bool GrammarVisitor::UnBind(const string& GTermStr) {
    return VarsBinding.erase(GTermStr); // returns one if succeeds.
  }

  bool GrammarVisitor::AdheresToGrammar(const Term* TheTerm) {
    if (GrmrTerm->GetKind() != GTERMKIND_SYMBOL) {
      Matched = false;
      TheTerm->Accept(this);
      return Matched;
    }

    SymbolGTerm* SymG = dynamic_cast <SymbolGTerm*> (GrmrTerm);

    NTDef* TheRule = NULL;
    for(auto const& Rule : SynthFun->GetGrammarRules()) {
      if (Rule->GetName() == SymG->GetSymbol()) {
	TheRule = Rule;
        break;
      }
    }

    // Haven't found the rule. Might be literal or variable
    if (TheRule == NULL) { 
      Matched = false;
      TheTerm->Accept(this);
      return Matched;
    }
    
    // Found the rule
    for(auto const& expansion : TheRule->GetExpansions()) { // Expansions is a list of GTerms          
      GrmrTerm = expansion;
      if (AdheresToGrammar(TheTerm))
	return true;
    }
   
    return false;
  }
  

} /* end namespace */
