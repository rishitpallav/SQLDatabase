#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer_mgr.h"
#include "storage_mgr.h"
#include "dberror.h"
#include "dt.h"
#include "storage_mgr.c"

#define MAX_PAGE 1000

int count = 0;
int BufferSize = 0;
int lastIndex = 0;
int *frameContent;
int *fixCount;
int *dirtyFlag;
int *LRU;
int *LFU;
int *clock;
int *clockFlag;
int *clockPointer;

RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData)
{
    if (numPages <= 0)
    {
        return RC_INVALID_BM;
    }
    if (bm == NULL)
    {
        return RC_INVALID_BM;
    }
}