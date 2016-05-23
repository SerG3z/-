/*
 *Потокобезопасная односвязная очередь на основе блокировок.
 *Реализовать  (в  виде  класса  или  в  виде  набора  функций)  с  использованием  блокировок 
 *потокобезопасную односвязную очередь. 
 *Потокобезопасная очередь должна поддерживать операции добавления, удаления и поиска элементов.  
 *Необходимо разработать тестовую программу,  в  которой  создаётся: 
 *- p тестовых  параллельных  потоков,  и  каждый  поток выполняет n
 *  операций  (вид  операций  выбирается  случайным  образом)  с  односвязной очередью 
 *  для случайно сгенерированных элементов. 
 *
 *Необходимо оценить пропускную способность очереди.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int counter_enq = 0;
int counter_deq = 0;
int counter_loo = 0;

int N = 100; // number of operations
int T = 90; // number of threads
/*
 * 	+-------+-----+
 * 	| value | ptr |---->
 * 	+-------+-----+
 */
struct listnode
{
	int value;
	struct lisnode *next;
};

/*
 *  +-------+------+------+
 *  | size  | HEAD | TAIL |---------------++ 
 *  +-------+------+------+               ||
 *             ||                         ||
 *             \/                         \/
 * 	+-------+-----+	  +-------+-----+   +-------+-----+
 * 	| value | ptr |-->| value | ptr |-->| value | ptr |--> 
 * 	+-------+-----+   +-------+-----+   +-------+-----+
 */
struct queue {
	struct listnode *head;
	struct listnode *tail;
	int size;
	pthread_mutex_t mutex;
};

/*queue_create: Creates an empty queue */
struct queue *queue_create()
{
	struct queue *q = malloc(sizeof(*q));
	if (q != NULL) {
		q->size = 0;
		q->head = NULL;
		q->tail = NULL;
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

/* create_node: Creates a new list node */
struct listnode *create_node(int value)
{
	struct listnode *new_node = malloc(sizeof(*new_node));
	if (new_node != NULL) {
		new_node->value = value;
		new_node->next = NULL;
	} else {
		fprintf(stderr, "ERROR: can't create new node\n");
		return NULL;
	}
	return new_node;
}

/* queue_enqueue: Add item to the queue */
void queue_enqueue(struct queue *q, int value)
{
	counter_enq++;
	
	struct listnode *old_tail;

	lock_queue(q);

	// Create newnode
	q->tail = create_node(value);

	old_tail = q->tail;
	
	if (q->head == NULL) {
		// List is empty	
		q->head = (struct listnode*)q->tail;
	} else {
		// Add newnode to the end of list
		old_tail->next = q->tail;
	}
	q->size++;
	unlock_queue(q);
}

/* queue_dequeue: Gets item from the queue */
int queue_dequeue(struct queue *q)
{
	int value;
	struct listnode *node;
	
	lock_queue(q);

	if (q->size == 0) {
		unlock_queue(q);
		return -1;
	}

	if (!q->head) {
		unlock_queue(q);
		return -1;
	}

	/* Delete first node */
	value = q->head->value;
	node = (struct listnode*)q->head->next;
	
	if (q->head) {
		free(q->head);	
	} else {
		fprintf(stderr, "ERROR: memory corruption\n");
		exit(EXIT_FAILURE);
	}
	
	q->head = node;
	q->size--;

	counter_deq++;
	unlock_queue(q);
	return value;
}

/* queue_lookup: Finds item in the queue */
int queue_lookup(struct queue *q, int value)
{
	counter_loo++;
	
	struct listnode *node;
	
	lock_queue(q);

	if (q->size == 0) {
		unlock_queue(q);
		return -1;
	}

	node = q->head;
	
	while(node)
	{
		if (node->value = value) {
			unlock_queue(q);
			return value;	
		}
		node = (struct listnode*)node->next;
	}
	unlock_queue(q);
	return -1;
}

/* queue_size : Returns size of a queue */
int queue_size(struct queue *q)
{
	return q->size;
}

/* queue_free: Removes queue */
void queue_free(struct queue *q)
{
	while (q->size > 0) {
		queue_dequeue(q);
	}
	free(q);
}

struct task
{
	struct queue *q;
	int tid;		
};

void *test(void *arg)
{
	struct task *task_ = (struct task*)arg;

	int branch = task_->tid;

	if ((branch > 0) && (branch < 30)){
		for(int i = 1; i <= N; i++) {
			queue_enqueue(task_->q, (rand() % 1000));
		}
	} else if ((branch > 30) && (branch < 60)) {
		for(int i = 1; i <= N; i++) { 
			queue_dequeue(task_->q);
		}
	} else {
		for(int i = 1; i <= N; i++) { 
			queue_lookup(task_->q, (rand() % 1000));
		}
	}		
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main(int argc, char const *argv[])
{	
	int rc;
	double t;
	struct queue *q;
	pthread_t thread[T];

	time_t time_;
   	// Intializes random number generator
   	srand((unsigned) time(&time_));

	q = queue_create();

    t = wtime();
	for (int i = 0; i < T; i++) {
		struct task *t = malloc(sizeof(*t));

		t->q = q;
		t->tid = rand() % 100;

		rc = pthread_create(&thread[i], NULL, test, (void*)t);
		if (rc) {
			fprintf(stderr,"ERROR: pthread_create() return code: %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}
	
	for (int i = 0; i < 10; i++) {
		pthread_join(thread[i], NULL);
	}

	t = wtime() - t;

	printf("Queue size   : %d\n", queue_size(q));
	printf("queue_enqueue: %d\n", counter_enq);
	printf("queue_dequeue: %d\n", counter_deq);
	printf("queue_lookup : %d\n", counter_loo);
	printf("BANDWITH     : %f(sec) %d(threads) %0.f(operations) %f(op/sec)\n", t, T, ((double)T*N), ((double)T*N) / t);
	//queue_free(q);
	
	return 0;
}