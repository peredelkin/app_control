/*
 * cli_parse.c
 *
 *  Created on: 27 июл. 2024 г.
 *      Author: ruslan
 */

#include "cli.h"
#include <stdbool.h>
#include <ctype.h>

typedef enum _E_Args_Parse_State {
    ARGS_RUN,
    ARGS_END,
    ARGS_SPACE,
    ARGS_ESC,
    ARGS_QUOT,
    ARGS_TOKEN
} args_parse_state_t;

int cli_line_parse(M_cli* args)
{
    char* str = (char*)(args->m_line);

    char* put = str;
    char* get = str;
    char* tkn = str;

    args_parse_state_t state = ARGS_RUN;

    bool quot = false;

    bool done = false;

    bool has_token = false;

    int c = 0;

    while(!done){

        switch(state){
            default:
                state = ARGS_END;
                break;
            case ARGS_RUN:
                c = *get;
                if(isspace(c)){
                    state = ARGS_SPACE;
                }else if(c == '"'){
                    state = ARGS_QUOT;
                }else if(c == '\\'){
                    state = ARGS_ESC;
                }else if(c == '\0'){
                    state = ARGS_END;
                }else{
                    state = ARGS_TOKEN;
                }
                break;
            case ARGS_END:
                *put = '\0';
                done = true;
                break;
            case ARGS_SPACE:
                if(quot == false){
                    if(has_token){
                        has_token = false;
                        *put = '\0';
                        put ++;
                        tkn = put;
                    }
                    get ++;
                    state = ARGS_RUN;
                }else{
                    state = ARGS_TOKEN;
                }
                break;
            case ARGS_ESC:
                get ++;
                c = *get;

                if(c == 'n'){
                    *get = '\n';
                }else if(c == 'r'){
                    *get = '\r';
                }else if(c == 't'){
                    *get = '\t';
                }

                state = (c != 0) ? ARGS_TOKEN : ARGS_RUN;
                break;
            case ARGS_QUOT:
                if(quot == false){
                    quot = true;
                }else{
                    quot = false;
                }
                get ++;
                state = ARGS_RUN;
                break;
            case ARGS_TOKEN:
                if(has_token == false){
                    if(args->m_argc >= CLI_ARGS_MAX) return -1;

                    args->m_argv[args->m_argc] = tkn;
                    args->m_argc ++;

                    has_token = true;
                }
                *put ++ = *get ++;
                state = ARGS_RUN;
                break;
        }

    }

    if(quot == true){
    	return -2;
	}

    return 0;
}

/*int cli_line_parse(M_cli *cli) {
	char* str = cli->line;

	str = strtok(str, CLI_ARGS_DELIMS);

    while(str){
    	if(cli->argc >= CLI_ARGS_MAX) return -1;

        cli->argv[cli->argc] = str;

        cli->argc ++;

        str = strtok(NULL, CLI_ARGS_DELIMS);
    }

    return 0;
}
*/
