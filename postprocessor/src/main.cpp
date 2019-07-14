#include<iostream>
#include<sstream>
#include<stdio.h>
#include<string.h>
#include "SygusGrmrChecker.hpp"
#include "SynthLib2ParserExceptions.hpp"

int clean(const char* sourceFileName, const char*  targetFileName)
{
  FILE* fin = fopen(sourceFileName, "rt");

  if (!fin)
    {
      throw string("can not open file\n");
      return -1;
    }

  FILE* fout = fopen(targetFileName, "wt");

  if (!fout)
    {
      throw string("can not open output file\n");
      fclose(fin);
      return -1;
    }

  char line[2048];
  char* lineStart;
  char* p;
  bool foundOpenparen = false;
  bool lineChanged;

  while (fgets(line, 2048, fin))
    {
      if (strstr(line, "-----"))
        break;

      lineStart = line;
 
      if (*lineStart != '(') {     
         lineChanged = false;
      	 lineStart = strchr(lineStart, 9);   /* 9 is ascii for tab */
      	 lineChanged = lineStart? true : false;
         lineStart = lineChanged ? lineStart+1 : line;
      } 

      if (strstr(lineStart,"EOF"))
        break;

      for (p = lineStart; *p; p++);

      if (*p == 0 || *p == '(') {
        foundOpenparen = true;
      }

      if (foundOpenparen)
        fprintf(fout, "%s",  lineStart);
    }
  fprintf(fout, "\n");

  fclose(fin);
  fclose(fout);
  return 0;
}

int main(int argc, char ** argv) {

   try {
      unsigned return_value = 0;

      if (argc == 3) {
          string arg2(argv[2]);
	  bool validate = (arg2=="CMPTRK_GENERAL" || 
                           arg2=="CMPTRK_INV" || 
                           arg2=="CMPTRK_LIA" ||
                           arg2=="CMPTRK_PBE") ? true : false;
          // WHEN USED AS A BENCHMARK PROCESSOR
	  if (validate) {
             char* benchmark = argv[1];
             char* track = argv[2];
             SygusTrackValidator(benchmark, track);
          }
          // WHEN USED AS A POST_PROCESSOR
          else  {
            char* benchmark = argv[2];
            char* solversOutput = argv[1];
            clean(solversOutput, "__sygus_solversout_cleaned.sl");
            clean(benchmark, "__sygus_benchmark_cleaned.sl");
            SygusGrmrChecker("__sygus_benchmark_cleaned.sl",
                             "__sygus_solversout_cleaned.sl",
                             "__sygus_grmrchecker_output.smt");
	    //SygusGrmrChecker(benchmark,solversOutput,"__sygus_grmrchecker_output.smt");
           }
      }
      else if (argc == 4) {
        // WHEN USED AS A PARSER / COMPILER TO CORE SYGUS
	char* input = argv[1];
        char* output = argv[2];
        string arg3(argv[3]);
        bool compile = arg3=="true"? true: false;
        SygusParser(input, output, compile);
      }
      else {
         cerr << "Expecting 2 arguments (benchmark, track) for track valdiation " << endl
              << "              where track in {CMPTRK_INV, CMPTRK_LIA, CMPTRK_GENERAL, CMPTRK_PBE}, " << endl
              << "          2 arguments (solvers-out, benchmark) for post-processing, and " << endl
              << "          3 arguments (input, output, compile) for parsing / compiling to core SyGuS " << endl
              << "              where compile in {true,false}. " << endl;
      }
   
      return return_value;
   }

   catch (const exception& Ex) {
        cout << Ex.what() << endl;
   }
}

