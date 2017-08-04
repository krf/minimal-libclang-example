#include <clang-c/Index.h>

#include <algorithm>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

void printLocation(CXSourceLocation location)
{
  CXFile file;
}

/**
 * Usage:
 *   libclang_test.bin MAIN_FILE LINE COLUMN [COMPILER_OPTS]
 *
 * Example:
 *   libclang_test.bin test.cpp 0 0 -std=c++11 -I.
 */
int main(int argc, char** argv)
{
    CXIndex idx = clang_createIndex(1, 0);

    const auto mainFile = argv[1];
    int line = -1;
    int column = -1;
    if (argc > 3) {
        line = strtol(argv[2], 0, 10);
        column = strtol(argv[3], 0, 10);
    }
    std::cout << line << column << argc << std::endl;

    const auto clangArgsCount = std::max((argc - 4), 0);
    char** clangArgs = (clangArgsCount > 0 ? argv + 4 : nullptr);

    // Parse the source file into a translation unit
    CXTranslationUnit tu = clang_parseTranslationUnit(idx,
        mainFile,
        clangArgs, clangArgsCount,
        nullptr, 0,
        CXTranslationUnit_PrecompiledPreamble);

    if (!tu)
    {
        printf("Couldn't parse tu\n");
        clang_disposeIndex(idx);
        return 1;
    }

    if (line != -1 && column != -2) {
        // Perform a code completion operation
        CXCodeCompleteResults* results = clang_codeCompleteAt(tu, mainFile, line, column, nullptr, 0, clang_defaultCodeCompleteOptions());
        if (results)
        {
            clang_sortCodeCompletionResults(results->Results, results->NumResults);
            for (unsigned int i = 0; i < results->NumResults; i++)
            {
                printf("COMPLETION: ");
                CXCompletionString &compString = results->Results[i].CompletionString;
                for (unsigned int j = 0; j < clang_getNumCompletionChunks(compString); j++)
                {
                    CXString s = clang_getCompletionChunkText(compString, j);
                    printf("%s ", clang_getCString(s));
                    clang_disposeString(s);
                }
                printf("\n");
            }
            clang_disposeCodeCompleteResults(results);
        }
        else
        {
            printf("Failed to perform completion operation\n");
        }
    }

    // Show any diagnostic information if available
    for (unsigned int i = 0; i < clang_getNumDiagnostics(tu); i++)
    {
        CXDiagnostic diag = clang_getDiagnostic(tu, i);
        CXString diagSpelling = clang_getDiagnosticSpelling(diag);

        const auto location = clang_getDiagnosticLocation(diag);
        CXFile file;
        unsigned int line = 0, col = 0, offset = 0;
        clang_getSpellingLocation(location, &file, &line, &col, &offset);
        const CXString fileName = clang_getFileName(file);
        printf("DIAGNOSTIC: %s:%u:%u: %s\n", clang_getCString(fileName), line, col, clang_getCString(diagSpelling));
        clang_disposeString(fileName);
        clang_disposeString(diagSpelling);
        clang_disposeDiagnostic(diag);
    }
    printf("Total diagnostics available: %d\n", clang_getNumDiagnostics(tu));
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(idx);

    return 0;
}
