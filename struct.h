/*GLODARIU Ana - 311CB*/

typedef struct
{ /*cum C e maxim 64 -> N e maxim 128*/
    unsigned char id; /*nr intre 0-127*/
} Thread;

typedef struct {
    unsigned short id2; /*id Task; nr natural 1-32767*/
    Thread id1;
    unsigned char p; /*prioritate; nr natural 0-127*/
    unsigned int t; /*timp de executie; nr natural pe 4 octeti*/
    unsigned int rt; /*remaining time*/
} Task;
