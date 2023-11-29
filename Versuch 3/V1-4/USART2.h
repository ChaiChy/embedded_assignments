void USART2Init(void);
void USART2WriteChar(char c);
void USART2WriteString(char *str);

extern char USART2InputBuffer[];
extern int USART2CmdFlag;

extern volatile int amplitude;
extern volatile int delay;