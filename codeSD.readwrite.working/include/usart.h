#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

int uart_putchar(char c, char *stream);
int uart_getchar(char *stream);

void uart_init(void);
void io_redirect(void);


#endif

