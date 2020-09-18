#include <SynthLib2ParserIFace.hpp>
#include <SynthLib2ParserFwd.hpp>
#include <SynthLib2ParserExceptions.hpp>
#include <SymbolTable.hpp>
#include <GrammarVisitor.hpp>
#include <LogicSymbols.hpp>
#include <InVisitor.hpp>
#include <OutVisitor.hpp>
#include <PrintVisitor.hpp>


using namespace SynthLib2Parser;

bool DoesNameAndSortMatch(const FunDefCmd* Out, const SynthFunCmd* In) {

  if (In->GetFunName() != Out->GetFunName()) 
    return false;
    
  if (In->GetSort()->GetKind() != Out->GetSort()->GetKind()) 
    return false;
      
  unsigned l = Out->GetArgs().size();
      
  if (l != In->GetArgs().size()) 
    return false;
	
  auto i = In->GetArgs().begin();
  auto o = Out->GetArgs().begin();
	
  for (unsigned k = 0; k < l  ; k++) {
    if ((*i)->GetSort()->GetKind() != (*o)->GetSort()->GetKind()) 
	    return false;
    i++; o++;
  } 

  // if all checks passed name and sorts match

  return true;
}


u32 GrammarCorrect(const FunDefCmd* OutFun, const SynthFunCmd* SynthFun, SymbolTable* SymTbl) {

  bool noGrmrRestrictions = SynthFun->GetGrammarRules().empty();
  if (!noGrmrRestrictions) {
    // find the start (= grammar axiom)
    NTDef* TheStart = NULL; 
    for(auto const& Rule : SynthFun->GetGrammarRules()) {
      if (Rule->GetName() == "Start") {
        TheStart = Rule;
      }
    }
  
    if (TheStart==NULL) {
      string FunName = SynthFun->GetFunName();
      throw SynthLib2ParserException((string)"Grammar for function " + FunName + " has no Start axiom.");
      return false;
    }

    Term* TheTerm = OutFun->GetTerm(); // func body
    GTerm* TheGrmrTerm = new SymbolGTerm(SourceLocation::None, "Start");
    GrammarVisitor GVstr(cout, TheGrmrTerm, SynthFun, OutFun, noGrmrRestrictions, SymTbl);
    GVstr.AdheresToGrammar(TheTerm);

    return GVstr.GetExprSize();
  }

  return 1;
}


int AddSetLogic(char* solvers_out, string set_logic_string, char* solvers_revised_out)
{
  FILE* fin = fopen(solvers_out, "rt");
  FILE* fout = fopen(solvers_revised_out, "wt");

  fprintf(fout, "%s\n", set_logic_string.c_str());
  char c;
  while ((c=fgetc(fin)) != EOF)
    fputc(c, fout);
  fprintf(fout,"\n");

  fclose(fin);
  fclose(fout);
  return 0;
}


// ///////////////////////////////////////

// enum ElementKind
// {
//     EK_NAME,
//     EK_LIST,
// };

// struct element {
//     ElementKind kind;
//     int from;
//     int to;

//     element* next;
    
//     element* subs;
// };

// element head = {EK_LIST, 0, 0, NULL, NULL};

// bool is_blank(int ch)
// {
//     return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
// }


// void advance_to_next_token(char* file_buff, int& i)
// {
//     bool in_remark = false;

//     while (file_buff[i] && (in_remark || file_buff[i] == ';' || is_blank(file_buff[i])))
//     {
//         if (file_buff[i] == '\n' || file_buff[i] == '\r')
//             in_remark = false;
//         else if (file_buff[i] == ';')
//             in_remark = true;
       
//         i++;
//     }
// }

// bool get_name(char* buff, int& i, int& from, int& to)
// {
//     advance_to_next_token(buff, i);    
    
//     if (buff[i] == '(' || buff[i] == ')' || buff[i] == 0)
//         return false;

//     from = i;
//     while (!is_blank(buff[i]) && buff[i] != ')' && buff[i] != '(' && buff[i] != ';')
//         i++;

//     to = i;
    
//     return true;
// }

// void parse_next(char* file_buff, int& i, element* prev)
// {
//     int from, to;
//     if (get_name(file_buff, i, from, to))
//     {
//         prev->next = new element;
//         prev->next->kind = EK_NAME;
//         prev->next->from = from;
//         prev->next->to = to;

//         prev->next->next = NULL;        
//         prev->next->subs = NULL;

//         parse_next(file_buff, i, prev->next);
//     }
//     else if (file_buff[i] == '(')
//     {
//            prev->next = new element;
//            prev->next->kind = EK_LIST;
//            prev->next->from = 0;
//            prev->next->to = 0;
    
//            prev->next->next = NULL;
//            prev->next->subs = NULL;

//            i++;
//            parse_next(file_buff, i, prev->next);
//            prev->next->subs = prev->next->next;
//            prev->next->next = NULL;
        
//            if (file_buff[i] != ')')
//               printf("Expected ')'\n");

//            i++;
//            parse_next(file_buff, i, prev->next);
//     }
// }

// void print_tree(element* head, char* file_buff, FILE* fout, int level = 0)
// {
//     element* curr = head;
//     char tmp;
    
//     while (curr)
//     {
//         if (curr->kind == EK_NAME)
//         {
//             tmp = file_buff[curr->to];
//             file_buff[curr->to] = 0;
//             fprintf(fout, "%s ", file_buff+curr->from);
//             file_buff[curr->to] = tmp;
//         }
//         else
//         {   
// 	    if (level == 0)	
//               fprintf(fout, "\n(");
//             else
// 	      fprintf(fout, "(");
//             print_tree(curr->subs, file_buff, fout, level+1);

//             fprintf(fout, ")");
//         }
        
//         curr = curr->next;
//     }

// }



// void clear_tree(element* tree)
// {
//     if (tree->next)
//     {
//         clear_tree(tree->next);
//         delete(tree->next);
// 	tree->next = NULL;
//     }

//     if (tree->subs)
//     {
//         clear_tree(tree->subs);
//         delete(tree->subs);
//  	tree->subs = NULL;
//     }
// }

// int AnyArityToTwoArity(char* benchmark, char* benchmark_p) {

//     FILE* fin;
//     FILE* fout;
    
//     fin = fopen(benchmark, "rt");
    
//     if (fin == NULL)
//     {
//         printf("could not open file\n");
//         return 1;
//     }
   
//     fout = fopen(benchmark_p, "wt");
   
//     fseek(fin, 0, SEEK_END);
//     long file_size = ftell(fin);
//     char* file_buff = new char[file_size+1];
//     fseek(fin, 0, SEEK_SET);
    
//     fread(file_buff, file_size, 1, fin);
//     file_buff[file_size] = 0;
//     fclose(fin);
    
//     int i=0;
//     parse_next(file_buff, i, &head);
     
//     print_tree(head.next, file_buff, fout);
//     fprintf(fout, "\n");
//     fclose(fout);
    
//     clear_tree(&head);

//     delete [] file_buff;
    
//     return 0;
// }


// //////////////////////////////////////

int SygusTrackValidator(char* benchmark, char* track)
{
    int return_value = -1;

    try {
	CompTrack Track;
        string tr(track);
	if (tr=="CMPTRK_GENERAL") Track = CMPTRK_GENERAL;
        else if (tr=="CMPTRK_INV") Track = CMPTRK_INV;
        else if (tr=="CMPTRK_LIA") Track = CMPTRK_LIA;
        else if (tr=="CMPTRK_PBE") Track = CMPTRK_PBE;
        else throw SynthLib2ParserException((string)"Error: Unknown track");

        // Read the SyGuS Problem - the input to a SyGuS Solver

        // First change any arity to two arity
        // string benchmark_p = string(benchmark) + "___p";
        // AnyArityToTwoArity(benchmark,(char *)benchmark_p.c_str());
        
        // Then read instead the processed benchmark
        SynthLib2Parser::SynthLib2Parser* ParserIn = new SynthLib2Parser::SynthLib2Parser();
        (*ParserIn)(benchmark);
        SymbolTable* InFileTable = ParserIn->GetSymbolTable();
        Program* InFileProg = ParserIn->GetProgram();

        InVisitor Spec(cout, InFileTable, Track);
        InFileProg->Accept(&Spec);

        cout << "set-logic=" << Spec.GetLogic() << endl;

        if (Spec.GetNumSynthCmds()==0) {
          cout << "error= no functions to synthesize" << endl;
          cout << "starexec-valid=false" << endl;
        }
        else {
          cout << "num-of-funcs-to-synth =" <<  Spec.GetNumSynthCmds() << endl;

          // General Track 
          if (Track == CMPTRK_GENERAL) {
              if (Spec.ExistsInvConstraintCmd() == true) {
                cout << "error=inv-constraint command is not allowed in the general track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              if (Spec.ExistsSynthInvCmd() == true) {
                cout << "error=synth-inv command is not allowed in the general track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else {
                cout << "starexec-valid=true" << endl;
                cout << "starexec-expected-result=correct" << endl;
              }
          }

	  // Example Constraints Track 
          else if (Track == CMPTRK_PBE) {
              if (Spec.ExistsNonExampleConstraints() == true) {
	        cout << "error=only example constrained are allowed in the PBE track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else if (Spec.GetLogicName() != "SLIA" && Spec.GetLogicName() != "BV") {
                cout << "error=only SLIA and BV logics are allowed in the PBE track." << endl;
                cout << "starexec-valid=false" << endl;
              }

              else if (Spec.ExistsInvConstraintCmd() == true) {
                cout << "error=inv-constraint command is not allowed in the PBE track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else if (Spec.ExistsSynthInvCmd() == true) {
                cout << "error=synth-inv command is not allowed in the PBE track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else {
                cout << "starexec-valid=true" << endl;
                cout << "starexec-expected-result=correct" << endl;
              }
          }

          // Conditional Linear Arithmetic Track
          else if (Track == CMPTRK_LIA) {
              if (Spec.GetLogicName() != "LIA") {
                cout << "error=only LIA logic is allowed in Conditional Linear Arithmetic track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else if (Spec.AreSyntacticRestrictionsGiven()) {
                cout << "error=no syntactic restrictions are allowed in Conditional Linear Arithmetic track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else if (Spec.ExistsInvConstraintCmd() == true) {
                cout << "error=inv-constraint command is not allowed in Conditional Linear Arithmetic track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              if (Spec.ExistsSynthInvCmd() == true) {
                cout << "error=synth-inv command is not allowed in the Conditional Linear Arithmetic track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else {
                 cout << "starexec-valid=true" << endl;
                 cout << "starexec-expected-result=correct" << endl;
              }
          }

          // Invariant Synthesis Track 
          else { // (Track == CMPTRK_INV)
              if (Spec.GetLogicName() != "LIA") {
                cout << "error=only LIA logic is allowed in invariant synthesis track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else if (Spec.AreSyntacticRestrictionsGiven()) {
                cout << "error=no syntactic restrictions are allowed in invariant synthesis track." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else if (Spec.ExistsInvConstraintCmd() == false) {
                cout << "error=invariant synthesis track demands an inv-constraint command." << endl;
                cout << "starexec-valid=false" << endl;
              }
              else {
                cout << "starexec-valid=true" << endl;
                cout << "starexec-expected-result=correct" << endl;
              }
          }
          return_value = 0;
        }
        delete ParserIn;

    }
    catch (const exception& Ex) {
     cout << "starexec-valid=false" << endl;
      cout << "syntax-error={" << Ex.what() << "}" << endl;
      return -1;
    }

    return return_value;
}




// reads a sygus file, and outputs another
// if compile = true, the output is in core Sygus
// otherwise just simple manipulations like removing arbitrary arity
int SygusParser(char* input, char* output, bool compile)
{
    try {
        
        SynthLib2Parser::SynthLib2Parser* Parser = new SynthLib2Parser::SynthLib2Parser();
        (*Parser)(input);
        SymbolTable* SymTbl = Parser->GetSymbolTable();
        Program* Prog = Parser->GetProgram();

        ofstream OutFile;
        OutFile.open(output);
        PrintVisitor bench(OutFile,SymTbl,compile);
        Prog->Accept(&bench);
	OutFile.close();

        delete Parser;
        SynthLib2Parser::LogicSymbolLoader::Reset(); 
        return 0;
    } catch (const exception& Ex) {
        cout << Ex.what() << endl;
        return -1;;
    }
}


// takes a benchmark input file 
// and a solution file 
// both assumed to be in in SyGuS-COMP'14 format (needs to be reverted first if not)
// and check whether the solution is gramatically correct
// and outouts a combined file to be given to an SMT solver 
// to check whether the solution is semantically correct.

int SygusGrmrChecker(char* benchmark, char* solvers_out, char* smt_file)
{

    SynthLib2Parser::SynthLib2Parser* ParserIn; 
    SynthLib2Parser::SynthLib2Parser* ParserOut;

    int return_value = -3;

    try {

        // Read the SyGuS Problem - the input to a SyGuS Solver
        
        //first compile to core sygus 
        string benchmark_p = "__sygus_benchmark";
        ofstream CmpldBnc;
        CmpldBnc.open(benchmark_p);
        SygusParser(benchmark,(char *)benchmark_p.c_str(),true);
        CmpldBnc.close();
	
        // now parse it
	ParserIn = new SynthLib2Parser::SynthLib2Parser();
        (*ParserIn)(benchmark_p);
        SymbolTable* InFileTable = ParserIn->GetSymbolTable();
        Program* InFileProg = ParserIn->GetProgram();

        InVisitor Spec(cout, InFileTable,CMPTRK_GENERAL);
        InFileProg->Accept(&Spec);

        // Read the SyGuS Solution - the output of a SyGuS Solver
        // When reading it, start with the symbol table of 
        // the benchmark file, to obtain user defined functions
        // but remove first the functions to be synthesized to avoid conflicts
        InFileTable->DeleteSynthFuncs();

        // first remove arbirtrary arity
        //string solvers_out_p = "__sygus_solversout";
        //AnyArityToTwoArity(solvers_out,(char *)solvers_out_p.c_str());
        ParserOut = new SynthLib2Parser::SynthLib2Parser(InFileTable->Clone());
        // now parse it
        (*ParserOut)(solvers_out);
        SymbolTable* OutFileTable = ParserOut->GetSymbolTable();
        Program* OutFileProg = ParserOut->GetProgram();

        OutVisitor Impl(cout, OutFileTable);
        OutFileProg->Accept(&Impl);

        // Search for all functions to be synthesize (from input file)
        // For each function look for a potential implementation (from output file)
        // and check whether it adheres to the grammar
        // Output the result: adheres/doesn't adhere 
        //                   + expr size (if it adheres)
        // If it adheres replace the function to be synthesized with its implementation
        // as a prepration for an SMT query to check for correctness of the implementations.

        ostringstream strm;
        bool allAdhere = true;
        bool allSolved = true;
        int totalSize = 0;
        for (u32 i = 0; i < Spec.CmdsSize(); i++) {

          if (Spec.GetCmd(i)->GetKind() != CMD_SYNTHFUN)
            continue;

          const SynthFunCmd* mystFun = dynamic_cast<SynthFunCmd*> (const_cast<ASTCmd*>(Spec.GetCmd(i)));
          string name =  mystFun->GetFunName();
          bool matchExists = false;

          for (auto const* impl : Impl.GetFunDefs()) {
            bool matched = DoesNameAndSortMatch(impl,mystFun);
            matchExists |= matched;
            if (matched) {
              Spec.SetCmd(i,impl);
              u32 exprSize = GrammarCorrect(impl,mystFun,InFileTable->Clone());
              strm << "(" << name << " " << exprSize << ") ";
              totalSize += exprSize;
              if (!exprSize) {
                allAdhere = false;
                return_value = -2;
              }
            }
          }
          if (!matchExists) {
            strm << "(" << name << " -1) "; // "-1" stands for not generated "0" stands for violation
            allSolved = false;
            return_value = -1;
          }
        }
        if (!allAdhere || !allSolved) {
          cout << "exprs-total-size=" << totalSize << endl
               << "exprs-details=" << strm.str() << endl;
          if (!allSolved) {
            cout << "adheres= n/a (not all solved)" << endl;
            cout << "starexec-result=unsolved" << endl;
          }
          else {
            cout << "adheres=false" << endl;
            cout << "starexec-result=violates_grammar" << endl;
          }
        }
        else {
          return_value = 0;
          cout << "adheres=true " << endl
               << "exprs-total-size=" << totalSize << endl
               << "exprs-details=" << strm.str() << endl;

          // Prepare the smt file to check the correctness of the proposed solution
          // The functions to be synthesized have already been replaced with the 
          // corresponding implementatoin. 
          // Need to add universal quantification in all constraints referring to 
          // imlemented functions.
          // And make some other small changes (e.g. set-logic, check-sat).

          ofstream SmtFile;
          SmtFile.open (smt_file);
          PrintVisitor Pvstr(SmtFile,InFileTable,false,true);
          SmtFile << Spec.GetLogic() << endl;

          for (u32 i = 0; i < Spec.CmdsSize(); i++) {

            switch(Spec.GetCmd(i)->GetKind()) {

            case CMD_FUNDEF:
            case CMD_SYNTHFUN:
            case CMD_SYNTHINV:
            case CMD_FUNDECL:
            case CMD_SORTDEF:
              Spec.GetCmd(i)->Accept(&Pvstr);
              break;

            case CMD_CONSTRAINT:
              {
                auto Cnstr = dynamic_cast<ConstraintCmd*> (const_cast<ASTCmd*>(Spec.GetCmd(i)));
                SmtFile << "(assert ";
                string s = Spec.GetCmdAuxString(i);
                SmtFile << s;
                Cnstr->GetTerm()->Accept(&Pvstr);
                if (s != "") {
                  SmtFile << "))" << endl;
                }
                else {
                  SmtFile << ")" << endl;
                }
                break;
              }
            case CMD_SETLOGIC:
            case CMD_SETOPTS:
            case CMD_VARDECL:
            case CMD_PRIMEDVARDECL:      
            case CMD_CHECKSYNTH:
            case CMD_INV_CONSTRAINT:
              // nothing to do
                 break;   
            }

          }
          SmtFile << "(check-sat)" << endl;
          SmtFile.close();
        }
    }
    catch (const exception& Ex) {
        cout << "syntax-error={" << Ex.what() << "}" << endl;
        return -100;
    }

    delete ParserIn;
    delete ParserOut;

    return return_value;
}



