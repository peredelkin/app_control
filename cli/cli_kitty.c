/*
 * cli_kitty.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include <stdio.h>

int cli_print_cat(int argc, char* argv[])
{
#define CATS_COUNT 2
	static const char* cat_ascii[CATS_COUNT] = {
			" ,_     _\n"
			" |\\\\_,-~/\n"
			" / _  _ |    ,--.\n"
			"(  @  @ )   / ,-'\n"
			" \\  _T_/-._( (\n"
			" /         `. \\\n"
			"|         _  \\ |\n"
			" \\ \\ ,  /      |\n"
			"  || |-_\\__   /\n"
			" ((_/`(____,-'\n",

			"   |\\---/|\n"
			"   | ,_, |\n"
			"    \\_`_/-..----.\n"
			" ___/ `   ' ,""+   \\\n"
			"(__...'   __\\    |`.___.';\n"
			"  (_,...'(_,.`__)/'.....+",
	};

	puts(cat_ascii[(argc ^ 0x1) & 0x1]);

	return 0;
#undef CATS_COUNT
}
