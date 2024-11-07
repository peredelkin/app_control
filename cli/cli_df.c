/*
 * cli_df.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include "yaffs2/yaffsfs.h"

int cli_yaffs_freespace(int argc, char* argv[]) {
	if(argc != 2) return -1;

	Y_LOFF_T free_space = yaffs_freespace(argv[1]);

	printf("%s free space: %lld\n", argv[1], free_space);

	return 0;
}
