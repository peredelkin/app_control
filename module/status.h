#ifndef MODULE_STATUS_H
#define MODULE_STATUS_H

#include <stdint.h>

//! Тип слова статуса.
typedef uint32_t status_t;

//! Общие биты статуса.
enum _E_Base_Status {
    STATUS_NONE = 0, //!< Ничего.
    STATUS_READY =		(1 << 0),	//!< Готовность.
    STATUS_VALID =		(1 << 1),	//!< Правильность выходных данных.
    STATUS_RUN =		(1 << 2),	//!< Работа.
    STATUS_ERROR =		(1 << 3),	//!< Ошибка.
    STATUS_WARNING =	(1 << 4),	//!< Предупреждение.
    STATUS_USER =		(1 << 5)	//!< Статусы модулей.
};


#endif /* MODULE_STATUS_H */
