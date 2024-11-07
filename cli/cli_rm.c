/*
 * cli_rm.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include "yaffs2/yaffsfs.h"

int cli_rm_nand_file(int argc, char* argv[])
{
	if(argc <= 1) return -1;

	const char* root = "/nand/";
	char filename[CLI_CMDS_FILENAME_MAX + 1];

	int i;
	int res;
	for(i = 1; i < argc; i++){
		if(strlen(argv[i]) > (CLI_CMDS_FILENAME_MAX - 6)){
			printf("too long file name: %s\n", argv[i]);
			continue;
		}

		memset(filename, 0x0, CLI_CMDS_FILENAME_MAX + 1);
		strcpy(filename, root);
		strcat(filename, argv[i]);

		res = yaffs_unlink(filename);
		if(res == -1){
			printf("Error removing file: %s!\n", argv[i]);
			continue;
		}
	}

	return 0;
}
