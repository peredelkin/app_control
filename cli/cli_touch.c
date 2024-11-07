/*
 * cli_touch.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include "yaffs2/yaffsfs.h"

int cli_touch_nand_file(int argc, char* argv[])
{
	if(argc <= 1) return -1;

	const char* root = "/nand/";
	char filename[CLI_CMDS_FILENAME_MAX + 1];

	int i;
	int f;
	for(i = 1; i < argc; i++){
		if(strlen(argv[i]) > (CLI_CMDS_FILENAME_MAX - 6)){
			printf("too long file name: %s\n", argv[i]);
			continue;
		}

		memset(filename, 0x0, CLI_CMDS_FILENAME_MAX + 1);
		strcpy(filename, root);
		strcat(filename, argv[i]);

		f = yaffs_open(filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if(f == -1){
			printf("Error creating file: %s!\n", argv[i]);
			continue;
		}
		yaffs_close(f);
	}

	return 0;
}
