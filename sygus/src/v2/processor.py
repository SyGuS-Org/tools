from .. import ast
from ..resolution import SymbolTable
from ..base.processor import SygusProcessorBase

class SygusV2Processor(SygusProcessorBase):
    __slots__ = ['symbol_table']

    def visit_program(self, program: ast.Program):
        commands = []
        for command in program.commands:
            if isinstance(command, ast.DeclarePrimedVarCommand):
                continue
            elif isinstance(command, ast.SetOptionsCommand):
                continue
            elif isinstance(command, ast.DeclareFunCommand):
                if len(command.parameter_sorts) > 0:
                    raise NotImplementedError
                command = ast.DeclareVarCommand(command.function_name, command.function_range_sort,
                                                command.start_location, command.end_location)
            commands.append(command)
        program.commands = commands
        SygusProcessorBase.visit_program(self, program)

    def __init__(self, symbol_table: SymbolTable):
        super().__init__('SygusV2Processor')
        self.symbol_table = symbol_table

    @staticmethod
    def run(program: ast.Program, symbol_table: SymbolTable) -> None:
        processor = SygusV2Processor(symbol_table)
        program.accept(processor)
