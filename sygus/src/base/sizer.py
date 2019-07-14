from .. import ast

class SygusSizerBase(ast.ASTVisitor):
    __slots__ = ['result']

    def visit_identifier_term(self, identifier_term: ast.IdentifierTerm):
        self.result += 1

    def visit_literal_term(self, literal_term: ast.LiteralTerm):
        self.result += 1

    def visit_function_application_term(self, function_application_term: ast.FunctionApplicationTerm):
        self.result += 1
        if str(function_application_term.function_identifier) == '-' and len(function_application_term.arguments) == 1 and isinstance(function_application_term.arguments[0], ast.LiteralTerm):
            # (- 1) and -1 are of the same size = 1
            return
        for argument in function_application_term.arguments:
            argument.accept(self)

    def visit_quantified_term(self, quantified_term: ast.QuantifiedTerm):
        self.result += 1
        for _ in quantified_term.quantified_variables:
            self.result += 2 # 1 for name, 1 for sort
        quantified_term.term_body.accept(self)

    def visit_let_term(self, let_term: ast.LetTerm):
        self.result += 1
        for variable_binding in let_term.variable_bindings:
            self.result += 1
            variable_binding[1].accept(self)
        let_term.let_body.accept(self)

    def visit_define_fun_command(self, define_fun_command: ast.DefineFunCommand):
        define_fun_command.function_body.accept(self)

    def visit_program(self, program: ast.Program):
        for command in program.commands:
            command.accept(self)

    def __init__(self, name: str = 'SygusSizerBase'):
        super().__init__(name)
        self.result = 0

    @staticmethod
    def run(program: ast.Program) -> int:
        sizer = SygusSizerBase()
        program.accept(sizer)
        return sizer.result
