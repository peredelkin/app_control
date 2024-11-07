/*
 * cli_append.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include "yaffs2/yaffsfs.h"

int cli_append_nand_file(int argc, char* argv[])
{
	if(argc < 3) return -1;

	const char* root = "/nand/";
	char filename[CLI_CMDS_FILENAME_MAX + 1];

	if(strlen(argv[1]) > (CLI_CMDS_FILENAME_MAX - 6)){
		printf("too long file name: %s\n", argv[1]);
		return -1;
	}

	memset(filename, 0x0, CLI_CMDS_FILENAME_MAX + 1);
	strcpy(filename, root);
	strcat(filename, argv[1]);

	int f;

	f = yaffs_open(filename, O_APPEND | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	if(f == -1){
		printf("Error opening file: %s!\n", filename);
		return -1;
	}

	int i;
	int res;
	for(i = 2; i < argc; i++){

        res = yaffs_write(f, argv[i], strlen(argv[i]));
        if(res == -1){
        	printf("Error write file!\n");
        	yaffs_close(f);
        	return -1;
        }
	}

	yaffs_close(f);

	return 0;
}
