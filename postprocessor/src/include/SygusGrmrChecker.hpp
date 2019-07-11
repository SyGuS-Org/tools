#if !defined __SYGUS_GRAMMAR_CHECKER_HPP
#define __SYGUS_GRAMMAR_CHECKER_HPP


int SygusTrackValidator(char* benchmark, char* track);
int SygusParser(char* input, char* output, bool compile);
int SygusGrmrChecker(char* benchmark, char* solvers_out, char* smt_file);


#endif /* __SYGUS_GRAMMAR_CHECKER_HPP */
