#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

struct queue
{
	int *v;
	int head;
	int tail;
	int size;
	int maxsize;
	pthread_mutex_t mutex;  
};

/*queue_create: Creates an emptyqueue*/
struct queue *queue_create(int maxsize)
{
	struct queue *q = malloc(sizeof(*q));
	if (q != NULL) {
		q->v = malloc(sizeof(int)*(maxsize+1));

		if (q->v == NULL) {
			free(q);
			return NULL;
		}

		q->maxsize = maxsize;
		q->size = 0;
		q->head = maxsize + 1;
		q->tail = 0;
		pthread_mutex_init(&q->mutex, NULL);
	}
	return q;
}

void lock_queue(struct queue *q)
{
	pthread_mutex_lock(&q->mutex);
}

void unlock_queue(struct queue *q)
{
	pthread_mutex_unlock(&q->mutex);
}

/* queue_enqueue: Add item to the queue */
int queue_enqueue(struct queue *q, int value)
{
	lock_queue(q);

	if(q->head == q->tail + 1) {
		fprintf(stderr,"queue: Queue overflow\n");
		unlock_queue(q);
		return-1;
	}
	q->v[q->tail++] = value;
	q->tail = q->tail % (q->maxsize+1);
	q->size++;
	unlock_queue(q);
	return 0;
}

/* queue_dequeue: Gets item from the queue */
int queue_dequeue(struct queue *q)
{
	lock_queue(q);

	if (q->head % (q->maxsize + 1) == q->tail) {
		/* Queue is empty */
		fprintf(stderr,"queue: Queue is empty\n");
		unlock_queue(q);
		return -1;
	}

	q->head = q->head % (q->maxsize+ 1);
	q->size--;
	unlock_queue(q);
	return q->v[q->head++];
}

/* queue_lookueue: Lookup item in the queue */
int queue_lookqueue(struct queue *q, int value)
{
	lock_queue(q);

	if (q->head % (q->maxsize + 1) == q->tail) {
		/* Queue is empty */
		fprintf(stderr,"queue: Queue is empty\n");
		unlock_queue(q);
		return -1;
	}

	int ptr = q->head;

	ptr = q->head % (q->maxsize+ 1);
	
	for (int i = 0; i < q->size; i++) {
		if ((q->v[ptr++]) == value) { 
			unlock_queue(q);
			return value;
		}
	}
	unlock_queue(q);	
	return -1;
}

int main()
{
	struct queue *q;
	int i,val;

	q = queue_create(10);

	for(i = 1; i <= 11; i++) {
		queue_enqueue(q, i);
	}

	for(i = 1; i <= 11; i++) {
		printf("%d\n", queue_lookqueue(q, i));
	}

	for(i = 1; i <= 5; i++) {
		queue_dequeue(q);
	}

	for(i = 1; i <= 11; i++) {
		printf("%d\n", queue_lookqueue(q, i));
	}

	return 0;
}