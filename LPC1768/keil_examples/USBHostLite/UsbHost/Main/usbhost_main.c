/*
**************************************************************************************************************
*                                                 NXP USB Host Stack
*
*                                     (c) Copyright 2008, NXP SemiConductors
*                                     (c) Copyright 2008, OnChip  Technologies LLC
*                                                 All Rights Reserved
*
*                                                  www.nxp.com
*                                               www.onchiptech.com
*
* File           : usbhost_main.c
* Programmer(s)  : Ravikanth.P
* Version        :
*
**************************************************************************************************************
*/

/*
**************************************************************************************************************
*                                       INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include  "usbhost_main.h"

/*
**************************************************************************************************************
*                                          MAIN FUNCTION
*
* Description: This function is the main function where the execution begins
*
* Arguments  : None
*
* Returns    : 
*
**************************************************************************************************************
*/

int main()
{
    int32_t  rc;
    uint32_t  numBlks, blkSize;
    uint8_t  inquiryResult[INQUIRY_LENGTH];

    /* SystemClockUpdate() updates the SystemFrequency variable */
    SystemClockUpdate();

    UART_Init(57600);          /* Initialize the serial port to view the log messages                       */
    Host_Init();               /* Initialize the lpc17xx host controller                                    */
    rc = Host_EnumDev();       /* Enumerate the device connected                                            */
    if (rc == OK) {
		/* Initialize the mass storage and scsi interfaces */
        rc = MS_Init( &blkSize, &numBlks, inquiryResult );
        if (rc == OK) {
            rc = FAT_Init();   /* Initialize the FAT16 file system                                          */
            if (rc == OK) {
                Main_Copy();   /* Call the application                                                      */
            } else {
                return (0);
            }
        } else {
            return (0);
        }
    } else {							
        return (0);
    }
    while(1);
}

/*
**************************************************************************************************************
*                                      READ DATA FROM DISK
*
* Description: This function is used by the user to read data from the disk
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Read (void)
{
    int32_t  fdr;
    uint32_t  bytes_read;
    

    fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        PRINT_Log("Reading from %s...\n", FILENAME_R);
        do {
            bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        } while (bytes_read);

        FILE_Close(fdr);
        PRINT_Log("Read Complete\n");
    } else {
        PRINT_Log("Could not open file %s\n", FILENAME_R);
        return;
    }
}

/*
**************************************************************************************************************
*                                      WRITE DATA TO DISK
*
* Description: This function is used by the user to write data to disk
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Write (void)
{
    int32_t  fdw;
    int32_t  fdr;
    uint32_t  tot_bytes_written;
    uint32_t  bytes_written;


    fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) {
            tot_bytes_written = 0;
            PRINT_Log("Writing to %s...\n", FILENAME_W);
            do {
                bytes_written = FILE_Write(fdw, UserBuffer, MAX_BUFFER_SIZE);
                tot_bytes_written += bytes_written;
            } while (tot_bytes_written < WRITE_SIZE);
            FILE_Close(fdw);
            PRINT_Log("Write completed\n");
        } else {
            PRINT_Log("Could not open file %s\n", FILENAME_W);
            return;
        }
        FILE_Close(fdr);
    } else {
        PRINT_Log("Could not open file %s\n", FILENAME_R);
        return;
    }
}

/*
**************************************************************************************************************
*                                    COPYING A FILE
*
* Description: This function is used by the user to copy a file
*
* Arguments  : None
*
* Returns    : None
*
**************************************************************************************************************
*/

void  Main_Copy (void)
{
    int32_t  fdr;
    int32_t  fdw;
    uint32_t  bytes_read;

	fdr = FILE_Open(FILENAME_R, RDONLY);
    if (fdr > 0) {
        fdw = FILE_Open(FILENAME_W, RDWR);
        if (fdw > 0) {
            PRINT_Log("Copying from %s to %s...\n", FILENAME_R, FILENAME_W);
            do {
                bytes_read = FILE_Read(fdr, UserBuffer, MAX_BUFFER_SIZE);
                FILE_Write(fdw, UserBuffer, bytes_read);
            } while (bytes_read);
            FILE_Close(fdw);
        } else {
            PRINT_Log("Could not open file %s\n", FILENAME_W);
            return;
        }
        FILE_Close(fdr);
        PRINT_Log("Copy completed\n");
    } else {
        PRINT_Log("Could not open file %s\n", FILENAME_R);
        return;
    }
}
