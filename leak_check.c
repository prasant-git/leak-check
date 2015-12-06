#include <stdio.h>
#include <stdlib.h>
#include "leak_check_data_structure.h"

static s_file_t *gp_leak_check_ds_root = NULL;

void *lcMalloc(size_t size, const char *sFile, const char *sFunc, const unsigned int iLine)
{
	void *p_ret = malloc(size);
	if ( p_ret )
	{
		if ( _lcInsert(&gp_leak_check_ds_root, sFile, sFunc, iLine, p_ret) )
		{
			//
		}
		else
			ERROR( ("lcInsert failed %s:%s:%d !\n", __FILE__, __FUNCTION__, __LINE__) );
	}
	else {}
		//abort();
	return p_ret;
}

void *lcFree(void *ptr, const char *sFile, const char *sFunc, const unsigned int uiLine)
{
	if ( _lcDelete(&gp_leak_check_ds_root, sFile, sFunc, uiLine, ptr) );
	else
	{
		ERROR( ("lcFree failed for memory 0x%x. %s:%s:%d !\n", ptr, __FILE__, __FUNCTION__, __LINE__) );
	}	
}

void lcExamin()
{
	_lcDebug(&gp_leak_check_ds_root);
}
