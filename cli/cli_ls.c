/*
 * cli_ls.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include "yaffs2/yaffsfs.h"

static void cli_ls_nand_ls_dir(const char* dirname)
{
	yaffs_DIR* ydir;
	struct yaffs_dirent* yd;

	ydir = yaffs_opendir(dirname);
	if(ydir == NULL){
		printf("Error open dir: %s\n", dirname);
		return;
	}

	while((yd = yaffs_readdir(ydir))){
		printf("%s\n", yd->d_name);
	}

	yaffs_closedir(ydir);
}

int cli_ls_nand(int argc, char* argv[])
{
	if(argc == 0) return -1;

	const char* root = "/nand/";
	char filename[CLI_CMDS_FILENAME_MAX + 1];

	if(argc > 1){
		int i;
		for(i = 1; i < argc; i++){
			if(strlen(argv[i]) > (CLI_CMDS_FILENAME_MAX - 6)){
				printf("too long dir name: %s\n", argv[i]);
				continue;
			}

			memset(filename, 0x0, CLI_CMDS_FILENAME_MAX + 1);
			strcpy(filename, root);
			strcat(filename, argv[i]);

			cli_ls_nand_ls_dir(filename);
		}
	}else{
		memset(filename, 0x0, CLI_CMDS_FILENAME_MAX + 1);
		strcpy(filename, root);
		cli_ls_nand_ls_dir(filename);
	}

	return 0;
}
