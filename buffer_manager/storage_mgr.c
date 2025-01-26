#include <stdio.h>
#include <io.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "storage_mgr.h"
#include <errno.h>
#include <time.h>
#include "dberror.h"
#include <unistd.h>
#include <fcntl.h>

/* Defining global variables */
FILE *filePointer;
int lastPagePos;
int totalNumPages;
int curfHandle;
int curPagePos;

void initStorageManager(void)
{
    char *msgInitStorageManager = "Storage Manager has been started successfully....\n";
    printf("%s\n", msgInitStorageManager);
}

RC createPageFile(char *fileName)
{
    int pageNumber = 4096;
    filePointer = fopen(fileName, "w+"); // File is opened in write mode
    while (filePointer != NULL)
    {
        SM_PageHandle freshPage = (SM_PageHandle)malloc(PAGE_SIZE); // Allot memory for single page
        if (freshPage != NULL)
        {
            memset(freshPage, '\0', PAGE_SIZE);           // Fill in the page with the '\0'
            fwrite(freshPage, PAGE_SIZE, 1, filePointer); // Create a file with the page in it.
        }
        fclose(filePointer); // Closing the file
        pageNumber = -1;
        free(freshPage); // Free up the memory
        return RC_OK;
    }
    if (pageNumber == 4096)
    {
        printf("Page number is not valid");
    }
    return RC_FILE_NOT_FOUND;
}

RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{

    filePointer = fopen(fileName, "r+"); // read mode is used to open file 
    int temp = 0;
    long ZERO_CONSTANT = 0L;
    if (filePointer == NULL)
    {
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        fseek(filePointer, ZERO_CONSTANT, SEEK_END);
        totalNumPages = (ftell(filePointer) / PAGE_SIZE) + 1; // Retrieve the complete number of pages.
        if (filePointer != NULL)
        {
            totalNumPages--;
            fHandle->curPagePos = 0;
            if (temp == 0)
            {
                fHandle->mgmtInfo = filePointer;
                fHandle->fileName = fileName;
            }
            fHandle->totalNumPages = totalNumPages;
        }
        if (temp == 0)
            return RC_OK;
    }
    return RC_OK;
}

RC closePageFile(SM_FileHandle *fHandle)
{
    if (fHandle == NULL)
    {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if (filePointer == NULL)
    {
        return RC_FILE_NOT_FOUND; // If the file is missing, send an error.
    }
    else if (fclose(filePointer) == 0) // Closing the file
    {
        return RC_OK; // If the file is closed, indicate success.
    }
    return RC_FILE_NOT_FOUND; // If the file is not found, return an error.
}

RC destroyPageFile(char *fileName)
{
    if (filePointer != NULL) // Verify that the file is open.
    {
        fclose(filePointer); // Close the file
    }
    if (remove(fileName) == 0) // Delete the file.
    {
        return RC_OK; // If the file is deleted, return success.
    }
    return RC_FILE_NOT_FOUND;
}

RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int pageNumber = 4096;
    if (pageNum <= fHandle->totalNumPages)
    {
        while (pageNum >= 0)
        {
            
            fseek(filePointer, pageNum * PAGE_SIZE, SEEK_SET);// Set the page's first position as the file pointer.
            if (pageNumber == PAGE_SIZE)
            {
                
                fread(memPage, PAGE_SIZE, 1, filePointer);// Read page data into memPage
                
                fHandle->curPagePos = pageNum; // Set the current position of the page

            }
            pageNumber = -1;
            return RC_OK;
        }
        if (pageNumber == 4096)
        {
            printf("Page number is not valid");
        }
    }
    return RC_READ_NON_EXISTING_PAGE;
}

int getBlockPos(SM_FileHandle *fHandle)
{
    if (fHandle->curPagePos > -1)
    {
        return fHandle->curPagePos; // Retrieve the current page position.
    }
    if (fHandle == NULL)
    {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else
    {
        return RC_FILE_NOT_FOUND;
    }
}

RC readFirstBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle != NULL && memPage != NULL)
    {
        readBlock(0, fHandle, memPage);
        return RC_OK;
    }
    else if (fHandle == NULL)
    {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    else if (memPage == NULL)
    {
        return RC_FILE_NOT_FOUND;
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC readPreviousBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int NUM_CONST = 2;
    
    if (fHandle->curPagePos > 0)// Verify that the current page position is correct.
    {
        
        fseek(filePointer, (fHandle->curPagePos - 1) * PAGE_SIZE, SEEK_SET);// Set the page's first position as the file pointer.
        
        if (memPage != NULL && fHandle != NULL)// Read the page into memPage
        {
            fread(memPage, PAGE_SIZE, 1, filePointer);
            int curPagePos = fHandle->curPagePos;
            fHandle->curPagePos = curPagePos - NUM_CONST + 1;
        }
        return RC_OK;
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC readCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    
    if (fHandle->curPagePos >= 0 && fHandle->curPagePos <= fHandle->totalNumPages)// Verify that the current page position is correct.

    {
        fseek(filePointer, fHandle->curPagePos * PAGE_SIZE, SEEK_SET);// Set the page's first position as the file pointer.

        if (memPage != NULL)
        {
            if (fHandle != NULL)
            {
                
                fread(memPage, PAGE_SIZE, 1, filePointer);// Read the page into memPage
                return RC_OK;
            }
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC readNextBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    int NUM_CONST = 2;
    int currentPagePosition = fHandle->curPagePos;
    int totalNumberOfPages = fHandle->totalNumPages;
    
    if (currentPagePosition < totalNumberOfPages)  //Verify that the current page position is correct
    {  
        fseek(filePointer, (currentPagePosition + NUM_CONST - 1) * PAGE_SIZE, SEEK_SET);// Set the file pointer to the beginning of the page
        if (memPage != NULL)
        {
            if (fHandle != NULL)
            {
                fread(memPage, PAGE_SIZE, 1, filePointer);// Read the page into memPage
                
                fHandle->curPagePos = currentPagePosition + NUM_CONST - 1;// Set the current page position
                return RC_OK;
            }
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC readLastBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle != NULL)
    {
        fseek(filePointer, fHandle->totalNumPages * PAGE_SIZE, SEEK_SET);
        if (memPage != NULL)
        {
            fread(memPage, PAGE_SIZE, 1, filePointer);
            if (fHandle != NULL)
            {
                fHandle->curPagePos = fHandle->totalNumPages - 1;
                return RC_OK;
            }
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (pageNum >= 0 && pageNum <= fHandle->totalNumPages)
    {
        fseek(filePointer, pageNum * PAGE_SIZE, SEEK_SET);
        if (memPage != NULL)
        {
            fwrite(memPage, PAGE_SIZE, 1, filePointer);
            fHandle->curPagePos = pageNum;
            return RC_OK;
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle->curPagePos > 0 && fHandle->curPagePos <= fHandle->totalNumPages)
    {
        fseek(filePointer, fHandle->curPagePos * PAGE_SIZE, SEEK_SET);
        if (memPage != NULL)
        {
            fwrite(memPage, PAGE_SIZE, 1, filePointer);
            return RC_OK;
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    return RC_READ_NON_EXISTING_PAGE;
}

RC appendEmptyBlock(SM_FileHandle *fHandle)
{
    int totalNumberOfPages = fHandle->totalNumPages;
    if (fHandle != NULL)
    {
        fseek(filePointer, 0L, SEEK_END);
        SM_PageHandle freshPage = (SM_PageHandle)malloc(PAGE_SIZE);
        if (freshPage != NULL)
        {
            memset(freshPage, '\0', PAGE_SIZE);
            if (filePointer != NULL)
            {
                fwrite(freshPage, PAGE_SIZE, 1, filePointer);
                free(freshPage);
                fHandle->totalNumPages = totalNumberOfPages + 1;
                return RC_OK;
            }
            else
            {
                return RC_FILE_NOT_FOUND;
            }
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    else
    {
        return RC_FILE_NOT_FOUND;
    }
}

RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle)
{
    int totalNumberOfPages = fHandle->totalNumPages;
    if (numberOfPages > totalNumberOfPages)
    {
        fseek(filePointer, 0L, SEEK_END);
        SM_PageHandle freshPage = (SM_PageHandle)malloc(PAGE_SIZE);
        if (freshPage != NULL)
        {
            memset(freshPage, '\0', PAGE_SIZE);
            if (filePointer != NULL)
            {
                fwrite(freshPage, PAGE_SIZE, numberOfPages - totalNumberOfPages, filePointer);
                free(freshPage);
                fHandle->totalNumPages = numberOfPages;
                return RC_OK;
            }
            else
            {
                return RC_FILE_NOT_FOUND;
            }
        }
        else
        {
            return RC_FILE_NOT_FOUND;
        }
    }
    else
    {
        return RC_FILE_NOT_FOUND;
    }
}