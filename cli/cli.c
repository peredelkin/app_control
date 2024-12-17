#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cli.h"

#include <unistd.h> //free
#include "fmc/sram/fmc_sram.h" //free

#include "uart/init/uart_init.h"
#include "uart/buf/usart_buf.h"
#include "modules/modules.h"
#include "yaffs2/yaffsfs.h"

METHOD_INIT_IMPL(M_cli, cli)
{
}

METHOD_DEINIT_IMPL(M_cli, cli)
{
}

/* тестовые функции */
static int cli_rgb_set(int argc, char* argv[])
{
	if(argc != 2) return -1;

	int color = (int)strtol(argv[1], NULL, 10);

	if(color > 7) return -1;

	rgb_led.in = (uint16_t)color;

	return 0;
}
/* конец тестовые функций */

extern int cli_print_cat(int argc, char* argv[]);

extern int cli_yaffs_freespace(int argc, char* argv[]);

extern int cli_touch_nand_file(int argc, char* argv[]);

extern int cli_append_nand_file(int argc, char* argv[]);

extern int cli_ls_nand(int argc, char* argv[]);

extern int cli_cat_nand_file(int argc, char* argv[]);

extern int cli_rm_nand_file(int argc, char* argv[]);

int cli_free_ram(int argc, char* argv[]) {
	if(argc != 1) return -1;

	extern char _sextsram[];
	void* cur = sbrk(0);
	void* end = _sextsram + SRAM_WORDS_8;

	ptrdiff_t free_ram = end - cur;
	ptrdiff_t used_ram = SRAM_WORDS_8 - free_ram;

	printf("External ram free: %u byte(s)\n", free_ram);
	printf("External ram used: %u byte(s)\n", used_ram);

	return 0;
}

int cli_top_sys(int argc, char* argv[]) {
	if(argc != 1) return -1;

	extern struct timeval sys_main_execution_time;

	printf("Sys main execution time: %u.%06u\n", (unsigned)sys_main_execution_time.tv_sec, (unsigned)sys_main_execution_time.tv_usec);

	return 0;
}

int cli_mso_print(int argc, char* argv[]) {
	if(argc != 1) return -1;

//	for(int i = 0; i < MSO_MAX_CHANNEL_COUNT; i++) {
//		printf("MSO ch %i ptr: %u\n", i, (unsigned int)(mso.channel[i].ptr));
//	}

	float data = 0.0;

	int first;

	for(int index = 0; index < mso.ch_data_count; index++) {
		first = 1;
		for(int ch = 0; ch < MSO_MAX_CHANNEL_COUNT; ch++) {
			if(mso.channel[ch].ptr != NULL) {
				if(first == 0){
					printf(", ");
				}
				first = 0;
				data = mso.channel[ch].ptr[index]/32768.0;
				printf("%f", data);
			}
		}
		printf("\n");
	}

	return 0;
}

#define CLI_COMMANDS_COUNT 11
const cli_command_t cli_cmds[CLI_COMMANDS_COUNT] = {
	{"df", cli_yaffs_freespace},
	{"rgb_set", cli_rgb_set},
	{"kitty", cli_print_cat},
	{"touch", cli_touch_nand_file},
	{"ls", cli_ls_nand},
	{"rm", cli_rm_nand_file},
	{"cat", cli_cat_nand_file},
	{"fappend", cli_append_nand_file},
	{"free", cli_free_ram},
	{"top_sys", cli_top_sys},
	{"mso_print", cli_mso_print}
};

int cli_call(M_cli* cli, const cli_command_t* cmds, int cmds_count, int* cmd_res)
{
    if(cli->argc == 0) return 0;

    const char* cmd = cli->argv[0];

    int res;

    int i;
    for(i = 0; i < cmds_count; i ++){
        if(strcmp(cmds[i].name, cmd) == 0){
            res = cmds[i].func(cli->argc, cli->argv);
            if(cmd_res) *cmd_res = res;
            return 0;
        }
    }

    return -1;
}

void cli_reset(M_cli *cli) {
	cli->read_point = 0;
	memset(&cli->line, 0x0, CLI_LINE_MAX);
	cli->argc = 0;
}

void cli_read_point_count(M_cli* cli) {
	if(cli->read_point >= CLI_LINE_MAX) {
		cli->read_point = 0;
	} else {
		cli->read_point++;
	}
}

extern int cli_line_parse(M_cli* args);

METHOD_CALC_IMPL(M_cli, cli)
{
	size_t res = 0;
	int cmd_res = 0;
	if (usart_buf_data_avail(&usart_6)) {
		do {
			res = usart_buf_get(&usart_6, &(cli->line[cli->read_point]));
			if (res) {
				if ((cli->line[cli->read_point] == '\r') || (cli->line[cli->read_point] == '\n')) {
					cli->line[cli->read_point] = 0;
					if(cli_line_parse(cli) == 0) {
						if(cli_call(cli, cli_cmds, CLI_COMMANDS_COUNT, &cmd_res) != 0) {
							printf("\n%s not found!\n", cli->argv[0]);
						}else{
							printf("\n%s (%d)\n", (cmd_res == 0) ? "success" : "fail", cmd_res);
						}
					} else {
						printf("Invalid arguments!\n");
					}
					cli_reset(cli);
				} else {
					cli_read_point_count(cli);
				}
			}
		} while (res);
	}
}
