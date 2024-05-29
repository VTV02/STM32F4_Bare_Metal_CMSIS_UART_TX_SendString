#include <stdio.h>
#include<stdint.h>
#include "uart_tx.h"

int main(void)
{
	uart2_tx_init();
	while(1)
	{
		write_string(11,"Hello......");

	}
}
