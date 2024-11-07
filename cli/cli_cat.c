/*
 * cli_cat.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include "yaffs2/yaffsfs.h"
#include <unistd.h>

int cli_cat_nand_file(int argc, char* argv[])
{
	if(argc <= 1) return -1;

	const char* root = "/nand/";
	char filename[CLI_CMDS_FILENAME_MAX + 1];
	char buf[CLI_CMDS_BUF_MAX + 1];

	int i;
	int f;
	int res;
	for(i = 1; i < argc; i++){
		if(strlen(argv[i]) > (CLI_CMDS_FILENAME_MAX - 6)){
			printf("too long file name: %s\n", argv[i]);
			continue;
		}

		memset(filename, 0x0, CLI_CMDS_FILENAME_MAX + 1);
		strcpy(filename, root);
		strcat(filename, argv[i]);

		f = yaffs_open(filename, O_RDONLY, S_IRUSR | S_IWUSR);
		if(f == -1){
			printf("Error opening file: %s!\n", argv[i]);
			continue;
		}

		memset(buf, 0x0, CLI_CMDS_BUF_MAX + 1);
        while((res = yaffs_read(f, buf, CLI_CMDS_BUF_MAX)) > 0){
        	write(1, buf, res);
        }

		yaffs_close(f);
	}

	return 0;
}
