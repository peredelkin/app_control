#ifndef CLI_H
#define CLI_H

#include "module/base.h"

//! Перечисление возможных бит управления.
enum _E_Cli_Control {
    CLI_CONTROL_NONE = CONTROL_NONE,
};

//! Перечисление возможных бит статуса.
enum _E_Cli_Status {
    CLI_STATUS_NONE = STATUS_NONE,
};

//! Предварительная декларация типа модуля.
typedef struct _S_Cli M_cli;

#define CLI_LINE_MAX 255
#define CLI_LINE_ARRAY (CLI_LINE_MAX + 1)

#define CLI_ARGS_MAX 31
#define CLI_ARGS_ARRAY (CLI_ARGS_MAX + 1)

#define CLI_ARGS_DELIMS " \t"

#define CLI_CMDS_FILENAME_MAX 64
#define CLI_CMDS_BUF_MAX 64

typedef int (*cli_func_t)(int, char*[]);
typedef struct _Command {
    const char* name;
    cli_func_t func;
} cli_command_t;

//! Структура модуля.
struct _S_Cli {
    // Базовые поля.
    control_t control; //!< Слово управления.
    status_t status; //!< Слово состояния.
    // Входные данные.
    // Выходные данные.
    // Параметры.
    // Регистры.
    // Методы.
    METHOD_INIT(M_cli);
    METHOD_DEINIT(M_cli);
    METHOD_CALC(M_cli);
    // Коллбэки.
    // Внутренние данные.
    int m_read_point;
    uint8_t m_line[CLI_LINE_ARRAY];
    int m_argc;
    char* m_argv[CLI_ARGS_ARRAY];
};

EXTERN METHOD_INIT_PROTO(M_cli);
EXTERN METHOD_DEINIT_PROTO(M_cli);
EXTERN METHOD_CALC_PROTO(M_cli);

#define CLI_DEFAULTS {\
        /* Базовые поля */\
        0, /* control */\
        0, /* status */\
        /* Входные данные */\
        /* Выходные данные */\
        /* Параметры */\
        /* Регистры */\
        /* Методы */\
        METHOD_INIT_PTR(M_cli),\
        METHOD_DEINIT_PTR(M_cli),\
        METHOD_CALC_PTR(M_cli),\
        /* Коллбэки */\
        /* Внутренние данные */\
		0,/* read_point */\
		{0},/* data[] */\
		0,/* argc */\
		{0},/* argv */\
    }

#endif /* CLI_H */
