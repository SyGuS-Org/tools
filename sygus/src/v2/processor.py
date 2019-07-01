from .. import ast
from ..resolution import SymbolTable
from ..base.processor import SygusProcessorBase

class SygusV2Processor(SygusProcessorBase):
    __slots__ = ('symbol_table')

    def visit_program(self, program: ast.Program):
        program.commands = [command for command in program.commands
                                    if not isinstance(command, ast.DeclarePrimedVarCommand)
                                    and not isinstance(command, ast.SetOptionsCommand)]
        SygusProcessorBase.visit_program(self, program)

    def __init__(self, symbol_table: SymbolTable):
        super().__init__('SygusV2Processor')
        self.symbol_table = symbol_table

    @staticmethod
    def run(program: ast.Program, symbol_table: SymbolTable) -> None:
        processor = SygusV2Processor(symbol_table)
        program.accept(processor)
