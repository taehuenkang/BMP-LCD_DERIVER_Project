/*
* circularQueue.h
*
*/


#ifndef CIRCULARQUEUE_H_
#define CIRCULARQUEUE_H_

#define TRUE 1
#define FALSE 0
#define QUEUE_MAX 100

int queue_empty(void);
int queue_full(void);
void insert_queue(unsigned char value);
unsigned char read_queue();

#endif /* CIRCULARQUEUE_H_ */
