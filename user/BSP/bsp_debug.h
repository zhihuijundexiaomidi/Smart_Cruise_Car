#ifndef _BSP_DEBUG_H
#define	_BSP_DEBUG_H

#include "main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

#define debug_printf(fmt,...)		printf(char *fmt,...)



#endif /* _BSP_DEBUG_H */

