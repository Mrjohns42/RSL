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
* File           : usbhost_fat.h
* Programmer(s)  : Ravikanth.P
* Version        :
*
**************************************************************************************************************
*/

#ifndef  USBHOST_FAT_H
#define  USBHOST_FAT_H

/*
**************************************************************************************************************
*                                       INCLUDE HEADER FILES
**************************************************************************************************************
*/

#include  "usbhost_inc.h"

/*
**************************************************************************************************************
*                                      FAT DEFINITIONS
**************************************************************************************************************
*/

#define    FAT_16                   1

#define    LAST_ENTRY               1
#define    FREE_ENTRY               2
#define    LFN_ENTRY                3
#define    SFN_ENTRY                4

#define    RDONLY                   1
#define    RDWR                     2

/*
**************************************************************************************************************
*                                       TYPE DEFINITIONS
**************************************************************************************************************
*/

typedef struct boot_sec {
    uint32_t    BootSecOffset;             /* Offset of the boot sector from sector 0                     */
    uint16_t    BytsPerSec;                /* Bytes per sector                                            */
    uint8_t     SecPerClus;                /* Sectors per cluster                                         */
    uint32_t    BytsPerClus;               /* Bytes per cluster                                           */
    uint16_t    RsvdSecCnt;                /* Reserved sector count                                       */
    uint8_t     NumFATs;                   /* Number of FAT copies                                        */
    uint16_t    RootEntCnt;                /* Root entry count                                            */
    uint16_t    TotSec16;                  /* Total sectors in the disk. !=0 if TotSec32 = 0              */
    uint32_t    TotSec32;                  /* Total sectors in the disk. !=0 if TotSec16 = 0              */
    uint16_t    FATSz16;                   /* Sectors occupied by single FAT table                        */
    uint8_t     FATType;                   /* File system type                                            */
    uint32_t    RootDirSec;                /* Sectors occupied by root directory                          */
    uint32_t    RootDirStartSec;           /* Starting sector of the root directory                       */
    uint32_t    FirstDataSec;              /* Starting sector of the first data cluster                   */
} BOOT_SEC;

typedef  struct  file_entry {
    uint32_t  FileSize;                    /* Total size of the file                                      */
    uint16_t  CurrClus;                    /* Current cluster of the cluster offset                       */
    uint32_t  CurrClusOffset;              /* Current cluster offset                                      */
    uint32_t  EntrySec;                    /* Sector where the file entry is located                      */
    uint32_t  EntrySecOffset;              /* Offset in the entry sector from where the file is located   */
	uint8_t   FileStatus;                  /* File's open status                                          */
} FILE_ENTRY;

/*
**************************************************************************************************************
*                                       FUNCTION PROTOTYPES
**************************************************************************************************************
*/

int32_t  FAT_Init        (void);

uint8_t  FAT_GetFATType  (void);
void     PrintBootSec    (void);

int32_t  FILE_Open       (          uint8_t  *file_name,
                                       uint8_t   flags);

int32_t  FAT_FindEntry   (          uint8_t  *ent_name_given,
                                       FILE_ENTRY  *entry);

void     FAT_GetSFN      (volatile  uint8_t  *entry,
                                       uint8_t  *name);

void     FAT_GetSfnName  (volatile  uint8_t  *entry,
                                       uint8_t  *name);

void     FAT_GetSfnExt   (volatile  uint8_t  *entry,
                                       uint8_t  *ext_ptr);

int32_t  FAT_StrCaseCmp  (          uint8_t  *str1,
                                       uint8_t  *str2);

uint32_t FAT_ChkEntType  (volatile  uint8_t  *ent);

uint32_t FAT_ClusRead    (          uint16_t   curr_clus,
                                       uint32_t   clus_offset,
                             volatile  uint8_t  *buffer,
                                       uint32_t   num_bytes);

uint32_t FILE_Read       (          int32_t   fd,
                             volatile  uint8_t  *buffer,
                                       uint32_t   num_bytes);
uint16_t FAT_GetNextClus (          uint16_t   clus_no);

uint32_t FAT_ClusWrite   (          uint16_t   curr_clus,
                                       uint32_t   clus_offset,
                             volatile  uint8_t  *buffer,
                                       uint32_t   num_bytes);
uint32_t FILE_Write      (          int32_t   fd,
                             volatile  uint8_t  *buffer,
                                       uint32_t   num_bytes);

void     FAT_UpdateEntry (          FILE_ENTRY  *entry);

void     FAT_UpdateFAT   (          uint16_t   curr_clus,
                                       uint16_t   value);

uint16_t FAT_GetFreeClus (void);

int32_t  FAT_GetFreeEntry(          FILE_ENTRY  *entry);

void     FAT_PutSFN      (          uint8_t  *ent_name_given,
                                       FILE_ENTRY  *entry);

int32_t  FAT_CreateEntry (          uint8_t  *ent_name_given,
                                       FILE_ENTRY  *entry);

void     FILE_Close      (          int32_t   fd);

uint16_t FAT_GetEndClus  (          uint16_t   clus_no);

#endif
