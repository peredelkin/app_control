/*
 * YAFFS: Yet another Flash File System . A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2018 Aleph One Ltd.
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free Software Foundation.
 *
 * Note: Only YAFFS headers are LGPL, YAFFS C code is covered by GPL.
 */

/*
 * Header file for using yaffs in an application via
 * a direct interface.
 */


#ifndef __YAFFS_OSGLUE_H__
#define __YAFFS_OSGLUE_H__

#include <stddef.h>
#include <time.h>
#include <malloc.h>

#include "yportenv.h"

unsigned int yaffs_trace_mask = 0;

static int yaffsfs_lastError;

void yaffsfs_SetError(int err)
{
    yaffsfs_lastError = err;
}

int yaffsfs_GetLastError(void)
{
    return yaffsfs_lastError;
}

void yaffsfs_Lock(void)
{
}

void yaffsfs_Unlock(void)
{
}

u32 yaffsfs_CurrentTime(void)
{
    return (u32)time(NULL);
}

void *yaffsfs_malloc(size_t size)
{
    return malloc(size);
}

void yaffsfs_free(void *ptr)
{
    free(ptr);
}

int yaffsfs_CheckMemRegion(const void *addr, size_t size, int write_request)
{
    if(addr == NULL) return -1;
    return 0;
}

void yaffsfs_OSInitialisation(void)
{
}

void yaffs_bug_fn(const char *file_name, int line_no)
{
}

void yaffsfs_get_malloc_values(unsigned *current, unsigned *high_water)
{
    if(current) *current = 0;
    if(high_water) *high_water = 0;
}


#endif

