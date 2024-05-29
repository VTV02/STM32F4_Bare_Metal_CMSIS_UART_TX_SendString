#ifndef UART_TX_H_
#define UART_TX_H_

void uart2_tx_init(void);
void uart2_write(int ch);
void write_string(int wordlength,char* word);

#endif /* UART_TX_H_ */
