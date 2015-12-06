#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <assert.h>
#include "leak_check_data_structure.h"

#define MALLOC(x) malloc(x)

static s_file_t *newFile(const char *sFileName);
static s_func_t *newFunc(const char *sFuncName);
static s_line_t *newLine(const unsigned int uiLine);
static s_mem_t *newMem(const void *pMem);


struct __s_file
{
	char *s_file_name;
	s_func_t *p_func_root;
	s_file_t *p_next;
};

struct __s_func
{
	char *s_func_name;
	s_line_t *p_line_root;
	s_func_t *p_next;
} ;

struct __s_line
{
	unsigned int ui_line_no;
	s_mem_t *p_mem_root;
	struct __s_line *p_next;
};

struct __s_mem
{
	void *p_logical_memory;
	s_mem_t *p_next;
};

//
static s_file_t *newFile(const char *sFileName)
{
	s_file_t *psFile = (s_file_t *)MALLOC( (sizeof(s_file_t)) );
	assert(psFile);
	psFile->s_file_name = (char *)MALLOC( (strlen(sFileName) + 1) );
	assert(psFile->s_file_name);
	memcpy(psFile->s_file_name, sFileName, strlen(sFileName) + 1 );
	psFile->p_next = NULL;
	psFile->p_func_root = NULL;
	return psFile;
}

static s_func_t *newFunc(const char *sFuncName)
{
	s_func_t *psFunc = (s_func_t *)MALLOC ( (sizeof(s_func_t)) );
	assert(psFunc);
	psFunc->s_func_name = (char *)MALLOC( (strlen(sFuncName) + 1) );
	assert(psFunc->s_func_name);
	memcpy(psFunc->s_func_name, sFuncName, strlen(sFuncName) + 1 );
	psFunc->p_line_root = NULL;
	psFunc->p_next = NULL;
	return psFunc;
}

static s_line_t *newLine(const unsigned int uiLine)
{
	s_line_t *psLine = (s_line_t *)MALLOC( (sizeof(s_line_t)) );
	assert(psLine);
	psLine->ui_line_no = uiLine;
	psLine->p_mem_root = NULL;
	psLine->p_next = NULL;
	return psLine;
}

static s_mem_t *newMem(const void *pMem)
{
	s_mem_t *p_mem = (s_mem_t *)MALLOC( ( sizeof(s_mem_t) ) );
	assert(p_mem);
	//p_mem->p_logical_memory = (void *)MALLOC(sizeof(void *));
	//memcpy(&p_mem->p_logical_memory, pMem, sizeof(void *));
	p_mem->p_logical_memory = (void *)pMem;
	//p_mem->p_logical_memory = (void *)pMem;
	p_mem->p_next = NULL;
	return p_mem;
}


//
#define INSERT_BEG(root, n)	n->p_next = *root; \
				*root = n
#define REMOVE_NODE()

static bool isMemPresent(s_mem_t *psMemRoot, const void *pMem, s_mem_t **psMemRet)
{
	s_mem_t *p_trav = psMemRoot;
	int i_cmp_res = -1;
	for (; p_trav && (i_cmp_res = p_trav->p_logical_memory - pMem); p_trav = p_trav->p_next);
	if ( ! i_cmp_res)
	{
		*psMemRet = p_trav;
		return true;
	}
	else
		return false;
}

static bool memInsert(s_mem_t **psMemRoot, const void *pMem)
{
	s_mem_t *psMemTemp = NULL;
	if ( isMemPresent(*psMemRoot, pMem, &psMemTemp) );
	else
	{
		psMemTemp = newMem(pMem);
		INSERT_BEG(psMemRoot, psMemTemp);
	}
	return true;
	//return memInsert(&psLineTemp->p_mem_root, pMem);
}

static bool isLinePresent(s_line_t *psLineRoot, const unsigned int uiLine, s_line_t **psLineRet)
{
	s_line_t *p_trav = psLineRoot;
	int i_cmp_res = -1;
	for (; p_trav && (i_cmp_res = p_trav->ui_line_no - uiLine ); p_trav = p_trav->p_next);
	if ( ! i_cmp_res )
	{
		*psLineRet = p_trav;
		return true;
	}
	else
		return false;
}

static bool lineInsert(s_line_t **psLineRoot, const unsigned int uiLine, const void *pMem)
{
	s_line_t *psLineTemp = NULL;
	if ( isLinePresent(*psLineRoot, uiLine, &psLineTemp) );
	else
	{
		psLineTemp = newLine(uiLine);
		INSERT_BEG(psLineRoot, psLineTemp);
	}
	return memInsert(&psLineTemp->p_mem_root, pMem);
}

static bool isFuncPresent(s_func_t *psFuncRoot, const char *pFunc, s_func_t **psFuncRet)
{
	s_func_t *p_trav = psFuncRoot;
	int i_cmp_res = -1;
	for (; p_trav && (i_cmp_res = strcmp(p_trav->s_func_name, pFunc)); p_trav = p_trav->p_next);
	if ( ! i_cmp_res )
	{
		*psFuncRet = p_trav;
		return true;
	}
	else
		return false;
}

static bool funcInsert(s_func_t **psFuncRoot, const char *pFunc, const unsigned int uiLine, const void *pMem )
{
	s_func_t *psFuncTemp = NULL;
	if ( isFuncPresent(*psFuncRoot, pFunc, &psFuncTemp) );
	else
	{
		psFuncTemp = newFunc(pFunc);
		INSERT_BEG(psFuncRoot, psFuncTemp);
	}
	return lineInsert(&psFuncTemp->p_line_root, uiLine, pMem);
}

static bool isFilePresent(s_file_t *psFileRoot, const char *pFile, s_file_t **psFileRet)
{
	s_file_t *p_trav = psFileRoot;
	int i_cmp_res = -1;
	for (; p_trav && (i_cmp_res = strcmp(p_trav->s_file_name, pFile)); p_trav = p_trav->p_next);
	if ( ! i_cmp_res )
	{
		*psFileRet = p_trav;
		return true;
	}
	else
		return false;
}

static bool fileInsert(s_file_t **psFileRoot, const char *pFile, const char *pFunc, const unsigned int uiLine, const void *pMem)
{
	s_file_t *psFileTemp = NULL; //Retrieve root node for function list.
	if ( isFilePresent(*psFileRoot, pFile, &psFileTemp) );
	else
	{
		psFileTemp = newFile(pFile);
		INSERT_BEG(psFileRoot, psFileTemp);
	}
	return funcInsert(&psFileTemp->p_func_root, pFunc, uiLine, pMem);
}

bool _lcInsert( s_file_t **psFileRoot, const char *pFile, const char *pFunc, const unsigned int uiLine, const void *pMem)
{
	//if (NULL != *psFileRoot);
	//	//return fileInsert(psFileRoot, pFile, pFunc, uiLine, pMem);
	//else
	//	*psFileRoot = newFile(pFile); 
	PRINT( ("Memory allocated from File: %s, Func: %s, Line: %d, Mem: %x\n",
		pFile,
		pFunc,
		uiLine,
		pMem) );

	return fileInsert(psFileRoot, pFile, pFunc, uiLine, pMem);
}
/*
bool static __lcDeleteMem(s_mem_t **psMemRoot, const void *pMem)
{
	bool b_ret_val = false;
	s_mem_t *p_curr = NULL, *p_prev = NULL;
	for (p_curr = *psMemRoot; p_curr; p_prev = p_curr, p_curr = p_curr->p_next)
	{
		if ( p_curr->p_logical_memory == pMem )
		{
			//Found it.
			if (NULL == p_prev)
			{
				//Fix begining pointer.
				*psMemRoot = p_curr->p_next;
			}
			else
			{
				//Fix prev next.
				//skip over the removed node.
				p_prev->p_next = p_curr->p_next;
			}
			//free(p_curr->p_logical_memory);
			free(p_curr);
			b_ret_val = true;
			break;
		}
	}
	return b_ret_val;
}

bool static __lcDeleteLine(s_line_t **psLineRoot, const unsigned int uiLine, const void *pMem )
{
	bool b_ret_val = false;
	s_line_t *p_curr = NULL, *p_prev = NULL;
	for (p_curr = *psLineRoot; p_curr; p_prev = p_curr, p_curr = p_curr->p_next)
	{
		if ( p_curr->ui_line_no ==  uiLine )
		{
			//Found it.
			if ( (b_ret_val = __lcDeleteMem(&p_curr->p_mem_root, pMem)) && (NULL == p_curr) )
			{
				if (NULL == p_prev)
				{
					//Fix begining pointer.
					*psLineRoot = p_curr->p_next;
				}
				else
				{
					//Fix prev next.
					//skip over the removed node.
					p_prev->p_next = p_curr->p_next;
				}
				//free(p_curr->s_file_name);
				free(p_curr);
				break;
			}
		}
	}
	return b_ret_val;
}

bool static __lcDeleteFunc(s_func_t **psFuncRoot, const char *pFunc, const unsigned int uiLine, const void *pMem )
{
	bool b_ret_val = false;
	s_func_t *p_curr = NULL, *p_prev = NULL;
	for (p_curr = *psFuncRoot; p_curr; p_prev = p_curr, p_curr = p_curr->p_next)
	{
		if ( 0 == strcmp(p_curr->s_func_name, pFunc) )
		{
			//Found it.
			if ( (b_ret_val = __lcDeleteLine(&p_curr->p_line_root, uiLine, pMem)) && (NULL == p_curr) )
			{
				if (NULL == p_prev)
				{
					//Fix begining pointer.
					*psFuncRoot = p_curr->p_next;
				}
				else
				{
					//Fix prev next.
					//skip over the removed node.
					p_prev->p_next = p_curr->p_next;
				}
				free(p_curr->s_func_name);
				free(p_curr);
				break;
			}
		}
	}
	return b_ret_val;
}

bool static __lcDeleteFile(s_file_t **psFileRoot, const char *pFile, const char *pFunc, const unsigned int uiLine, const void *pMem)
{
	bool b_ret_val = false;
	s_file_t *p_curr = NULL, *p_prev = NULL;
	for (p_curr = *psFileRoot; p_curr; p_prev = p_curr, p_curr = p_curr->p_next)
	{
		if (0 == strcmp(p_curr->s_file_name, pFile))
		{
			//Found it.
			if ( (b_ret_val = __lcDeleteFunc(&p_curr->p_func_root, pFunc, uiLine, pMem)) && (NULL == p_curr) )
			{
				if (NULL == p_prev)
				{
					//Fix begining pointer.
					*psFileRoot = p_curr->p_next;
				}
				else
				{
					//Fix prev next.
					//skip over the removed node.
					p_prev->p_next = p_curr->p_next;
				}
				free(p_curr->s_file_name);
				free(p_curr);
				break;
			}
		}
	}
	return b_ret_val;
}
*/
bool _lcDelete(s_file_t **psFileRoot, const char *pFile, const char *pFunc, const unsigned int uiLine, const void *pMem)
{
	//return __lcDeleteFile(psFileRoot, pFile, pFunc, uiLine, pMem);
	//Testing purpose
	bool ret_val = false;
	s_file_t *p_file_trav = NULL, *p_file_prev = NULL;
	s_func_t *p_func_trav = NULL, *p_func_prev = NULL;
	s_line_t *p_line_trav = NULL, *p_line_prev = NULL;
	s_mem_t *p_mem_trav = NULL, *p_mem_prev = NULL;
	for (p_file_trav = *psFileRoot; p_file_trav; p_file_prev = p_file_trav, p_file_trav = p_file_trav->p_next)
	{
		for (p_func_trav = p_file_trav->p_func_root; p_func_trav; p_func_prev = p_func_trav, p_func_trav = p_func_trav->p_next)
		{
			for (p_line_trav = p_func_trav->p_line_root; p_line_trav; p_line_prev = p_line_trav, p_line_trav = p_line_trav->p_next)
			{
				for (p_mem_trav = p_line_trav->p_mem_root; p_mem_trav; p_mem_prev = p_mem_trav, p_mem_trav = p_mem_trav->p_next)
				{
					if (p_mem_trav->p_logical_memory == pMem)
					{
					//Found it.
						if (NULL == p_mem_prev)
						{
							//Fix begining pointer.
							p_line_trav->p_mem_root = p_mem_trav->p_next;
						}
						else
						{
							//Fix prev next.
							//skip over the removed node.
							p_mem_prev->p_next = p_mem_trav->p_next;
						}
						//free(p_curr->p_logical_memory);
						PRINT( ("Deallocate memory allocated at File: %s, Func: %s, Line: %d, Mem: %x\n",
							p_file_trav->s_file_name,
							p_func_trav->s_func_name,
							p_line_trav->ui_line_no,
							p_mem_trav->p_logical_memory) );

						free(p_mem_trav);
						ret_val = true;
						break;
						//return true;
					}
				}
				if ( ret_val && (NULL == p_line_trav->p_mem_root) )
				{
					if (NULL == p_line_prev)
						p_func_trav->p_line_root = p_line_trav->p_next;
					else
						p_line_prev->p_next = p_line_trav->p_next;
					free(p_line_trav);
					break;
				}
			}
			if (ret_val && (NULL == p_func_trav->p_line_root))
			{

				if (NULL == p_func_prev)
					p_file_trav->p_func_root = p_func_trav->p_next;
				else
					p_func_prev->p_next = p_func_trav->p_next;
				free(p_func_trav->s_func_name);
				free(p_func_trav);
				break;
			}
		}
		if ( ret_val && (NULL == p_file_trav->p_func_root) )
		{
			if (NULL == p_file_prev)
				*psFileRoot = p_file_trav->p_next;
			else
				p_file_prev->p_next = p_file_trav->p_next;
			free(p_file_trav->s_file_name);
			free(p_file_trav);
			break;
		}
	}
	return ret_val;
}

bool _lcDebug( s_file_t **psFileRoot )
{
	//Testing purpose
	s_file_t *p_file_trav = NULL;
	s_func_t *p_func_trav = NULL;
	s_line_t *p_line_trav = NULL;
	s_mem_t *p_mem_trav = NULL;
	for (p_file_trav = *psFileRoot; p_file_trav; p_file_trav = p_file_trav->p_next)
		for (p_func_trav = p_file_trav->p_func_root; p_func_trav; p_func_trav = p_func_trav->p_next)
			for (p_line_trav = p_func_trav->p_line_root; p_line_trav; p_line_trav = p_line_trav->p_next)
				for (p_mem_trav = p_line_trav->p_mem_root; p_mem_trav; p_mem_trav = p_mem_trav->p_next)
				{
					PRINT( ("File: %s, Func: %s, Line: %d, Mem: %x\n",
						p_file_trav->s_file_name,
						p_func_trav->s_func_name,
						p_line_trav->ui_line_no,
						p_mem_trav->p_logical_memory) );
				}
}


//int main()
//{
//	return 0;
//}
