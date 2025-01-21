This file defines each function that is presented in the storage manager file.

The functions are:
1. initStorageManager
2. createPageFile
3. openPageFile
4. closePageFile
5. destroyPageFile
6. readBlock
7. getBlockPos
8. readFirstBlock
9. readPreviousBlock
10. readCurrentBlock
11. readNextBlock
12. readLastBlock
13. writeBlock
14. writeCurrentBlock
15. appendEmptyBlock
16. ensureCapacity

initStorageManager:
This function is used to start the storage manager.

createPageFile:
This function is used to create a new page file.
This function has the filename as the argument.
The file is opened in the write mode which creates a new file if it does not exist.
Using SM_PageHandle and malloc function, we allocate memory to the page handle object.
The created object is filled with '\0' characters using memset function.
The created object is written to the file using fwrite function.
The size of the page created is 4096 bytes which is defined in the dberror.h file as PAGE_SIZE.
The function returns RC_OK if the file is created successfully.

openPageFile:
This function is used to open an existing page file.
This function has the filename and the file handle as the arguments.
The file is opened in the read mode.
Using fseek we move the file pointer to the end of the file.
Using ftell we get the size of the file and we divide it by the page size to get the total number of pages.
We modify the file handle object with the updated data.
The function returns RC_OK if the file is opened successfully.

closePageFile:
This function is used to close an opened page file.
This function has the file handle as the argument.
The function also checks if the file pointer or file handle is NULL and returns respective error messages.
The function returns RC_OK if the file is closed successfully.

destroyPageFile:
This function is used to destroy an existing page file.
This function has the filename as the argument.
The function also checks if the file pointer is NULL and closes the file if the file is still open.
The function returns RC_OK if the file is destroyed successfully.

readBlock:
This function is used to read a block from the file.
This function has the pageNum and memPage as the arguments.
The function first checks if the pageNum is within the boundaries of the file.
Once the check is successful, the file pointer is moved to the pageNum using fseek. We multiply the pageNum with the page size to get the exact offset.
The data is read from the file and stored in the memPage object.
The function returns RC_OK if the block is read successfully.

getBlockPos:
This function is used to get the current block position of the file.
This function has the file handle as the argument.
The function also checks if the file handle is NULL and returns respective error messages.
The function returns the current block position of the file.

readFirstBlock:
This function is used to read the first block of the file.
This function has the file handle and memPage as the arguments.
The function calls the readBlock function with pageNum as 0 to read the first block of the file.
The function also checks if the file handle is NULL and returns respective error messages.
The function returns RC_OK if the first block is read successfully.

readPreviousBlock:
This function is used to read the previous block of the file.
This function has the file handle and memPage as the arguments.
The function uses fseek to move the file pointer to the previous block using the current block position - 1.
The function then uses fread to read the data from the file and store it in the memPage object.
We further update the current block position by subtracting 1 from the current block position.
The function returns RC_OK if the previous block is read successfully.

readCurrentBlock:
This function is used to read the current block of the file.
This function has the file handle and memPage as the arguments.
The function checks if the current page position is within the boundaries of the file.
The function then uses fseek to move the file pointer to the current block using the current block position.
fread is used to read the data from the file and store it in the memPage object.
The function returns RC_OK if the current block is read successfully.

readNextBlock:
This function is used to read the next block of the file.
This function has the file handle and memPage as the arguments.
The function checks if the current page position is within the boundaries of the file.
The function then uses fseek to move the file pointer to the next block using the current block position + 1.
And fread is used to read the data from the file and store it in the memPage object.
We further update the current block position.
The function returns RC_OK if the next block is read successfully.

readLastBlock:
This function is used to read the last block of the file.
This function has the file handle and memPage as the arguments.
The function checks if the current page position is within the boundaries of the file.
We use fseek to move the file pointer to the last block using the total number of pages.
We then use fread to read the data from the file and store it in the memPage object.
We further update the current block position.
The function returns RC_OK if the last block is read successfully.

writeBlock:
This function is used to write a block to the file.
This function has the pageNum and memPage as the arguments.
The function first checks if the pageNum is within the boundaries of the file.
We use fWrite to write the data from the memPage object to the file.
The function returns RC_OK if the block is written successfully.

writeCurrentBlock:
This function is used to write the current block to the file.
This function has the file handle and memPage as the arguments.
The function checks if the current page position is within the boundaries of the file.
We use fseek to move the file pointer to the current block using the current block position.
We then use fwrite to write the data from the memPage object to the file.
The function returns RC_OK if the current block is written successfully.

appendEmptyBlock:
This function is used to append an empty block at the end of the file.
This function has the file handle as the argument.
The function uses malloc to allocate memory to the fresh page which is an SM_PageHandle object.
The object is filled with '\0' characters using memset function.
We use fseek to move the file pointer to the end of the file.
We then use fwrite to write the data from the fresh page object to the file.
We further update the total number of pages and current block position.
The function returns RC_OK if the empty block is appended successfully.

ensureCapacity:
This function is used to ensure the capacity of the file.
This function has the numberOfPages and file handle as the arguments.
The function checks if the numberOfPages is greater than the total number of pages.
After checking that, we use fseek to move the file pointer to the end of the file.
We then use fwrite to write the data from the fresh page object to the file.
We further update the total number of pages and current block position.
The function returns RC_OK if the capacity is ensured successfully.