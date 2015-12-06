#if !defined LEAK_CHECK_DATA_STRUCTURE_H
#define LEAK_CHECK_DATA_STRUCTURE_H

#define ERROR(x) printf x //We could write error to standard error or some file etc.
#define PRINT(x) printf x

typedef struct __s_mem s_mem_t;
typedef struct __s_file s_file_t;
typedef struct __s_func s_func_t;
typedef struct __s_line s_line_t;

#ifndef __cplusplus
typedef unsigned char bool;
enum {false, true};
//#else
//
#endif //s_file_info

bool _lcInsert( s_file_t **psFileRoot, const char *pFile, const char *pFunc, const unsigned int uiLine, const void *pMem);
bool _lcDelete(s_file_t **psFileRoot, const char *pFile, const char *pFunc, const unsigned int uiLine, const void *pMem);
void _lcDebug( s_file_t **psFileRoot );
#endif // LEAK_CHECK_DATA_STRUCTURE_H
