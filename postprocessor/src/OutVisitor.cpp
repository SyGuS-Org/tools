#include <OutVisitor.hpp>

namespace SynthLib2Parser {

    OutVisitor::OutVisitor(ostream& Out, const SymbolTable* SymTbl)
      : ASTVisitorBase("OutVisitor"), IndentLevel(0), Out(Out),  TheSymbolTable(SymTbl)
    {
        // Nothing here
    }

    OutVisitor::~OutVisitor()
    {
        Out.flush();
    }

    void OutVisitor::VisitProgram(const Program* Prog)  {
      for(auto const& Cmd : Prog->GetCmds()) {
        Cmd->Accept(this);
      }
    }
  
    void OutVisitor::VisitFunDefCmd(const FunDefCmd* Cmd) {
       FunDefs.push_back(Cmd);
    }

} /* end namespace */

