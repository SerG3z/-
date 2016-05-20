#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

int counter_pb = 0;
int counter_pf = 0;
int counter_ppb = 0;
int counter_ppf = 0;
int counter_loo = 0;

int N = 1000; // number of operations
int T = 10; // number of threads

struct listnode
{
	int value;
	struct lisnode *next;
	struct lisnode *prev;

};

struct queue {
	struct listnode *front;
	struct listnode *back;
	int size;
	pthread_mutex_t mutex_front;
	pthread_mutex_t mutex_back;
};

/*queue_create: Creates an empty queue */
struct queue *queue_create()
{
	struct queue *q = malloc(sizeof(*q));
	if (q != NULL) {
		q->size = 0;
		q->front = NULL;
		q->back = NULL;
		pthread_mutex_init(&q->mutex_front, NULL);
		pthread_mutex_init(&q->mutex_back, NULL);
	}
	return q;
}

void lock_front(struct queue *q) { pthread_mutex_lock(&q->mutex_front); }
void unlock_front(struct queue *q) { pthread_mutex_unlock(&q->mutex_front); }
void lock_back(struct queue *q) { pthread_mutex_lock(&q->mutex_back); }
void unlock_back(struct queue *q) { pthread_mutex_unlock(&q->mutex_back); }

/* create_node: Creates a new list node */
struct listnode *create_node(int value)
{
	struct listnode *new_node = malloc(sizeof(*new_node));
	if (new_node != NULL) {
		new_node->value = value;
		new_node->next = NULL;
		new_node->prev = NULL;
	} else {
		fprintf(stderr, "ERROR: can't create new node\n");
		return NULL;
	}
	return new_node;
}

void queue_pushFront(struct queue *q, int value)
{
	counter_pf++;
	
	struct listnode *old_front;
	struct listnode *new_front;

	lock_front(q);

	old_front = q->front;

	// Create newnode
	new_front = create_node(value);
	
	if (q->back == NULL) {
		// List is empty
		q->front = new_front;	
		q->back = (struct listnode*)q->front;
	} else {
		// Add newnode to the end of list
		old_front->next = new_front;
		q->front = new_front;
		q->front->prev = old_front;
	}
	q->size++;
	unlock_front(q);
}

void queue_pushBack(struct queue *q, int value)
{
	counter_pb++;
	
	struct listnode *old_back;
	struct listnode *new_back;

	lock_back(q);

	old_back = q->back;

	// Create newnode
	new_back = create_node(value);
	
	if (q->front == NULL) {
		// List is empty
		q->back	= new_back;
		q->front = (struct listnode*)q->back;
	} else {
		// Add newnode to the end of list
		old_back->prev = new_back;
		q->back = new_back;
		q->back->next = old_back;
	}
	q->size++;
	unlock_back(q);
}

int queue_popFront(struct queue *q)
{
	int value;
	struct listnode *node;
	
	lock_front(q);

	if (q->size == 2) {
		unlock_front(q);
		return -1;
	}

	if (!q->front) {
		unlock_front(q);
		return -1;
	}

	/* Delete first node */
	value = q->front->value;
	node = (struct listnode*)q->front->prev;
	
	if (q->front) {
		free(q->front);	
	} else {
		fprintf(stderr, "ERROR: memory corruption\n");
		exit(EXIT_FAILURE);
	}
	
	q->front = node;
	q->size--;

	counter_ppf++;
	unlock_front(q);
	return value;
}

int queue_popBack(struct queue *q)
{
	int value;
	struct listnode *node;
	
	lock_back(q);

	if (q->size == 2) {
		unlock_back(q);
		return -1;
	}

	if (!q->back) {
		unlock_back(q);
		return -1;
	}

	/* Delete first node */
	value = q->back->value;
	node = (struct listnode*)q->back->next;
	
	if (q->back) {
		free(q->back);	
	} else {
		fprintf(stderr, "ERROR: memory corruption\n");
		exit(EXIT_FAILURE);
	}
	
	q->back = node;
	q->size--;

	counter_ppb++;
	unlock_back(q);
	return value;
}

int queue_lookup(struct queue *q, int value)
{
	counter_loo++;
	
	struct listnode *node;
	
	lock_back(q);
	lock_front(q);

	if (q->size == 0) {
		unlock_back(q);
		unlock_front(q);
		return -1;
	}

	node = q->back;
	
	while(node)
	{
		if (node->value = value) {
			unlock_back(q);
			unlock_front(q);
			return value;	
		}
		node = (struct listnode*)node->next;
	}
	unlock_back(q);
	unlock_front(q);
	return -1;
}


/* queue_size : Returns size of a queue */
int queue_size(struct queue *q)
{
	return q->size;
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

	if ((branch > 0) && (branch < 20)){
		for(int i = 1; i <= N; i++) {
			queue_pushBack(task_->q, (rand() % 1000));
		}
	} else if ((branch > 20) && (branch < 40)) {
		for(int i = 1; i <= N; i++) {
			queue_pushFront(task_->q, (rand() % 1000)); 
		}
	} else if ((branch > 40) && (branch < 60)) {
		for(int i = 1; i <= N; i++) { 
			queue_popBack(task_->q);
		}
	} else if ((branch > 60) && (branch < 80)) {
		for(int i = 1; i <= N; i++) {
			queue_popFront(task_->q);
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
	queue_pushBack(q, 1);
	queue_pushFront(q, 1); 

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
	printf("queue_pf : %d\n", counter_pf);
	printf("queue_pb : %d\n", counter_pb);
	printf("queue_ppf: %d\n", counter_ppf);
	printf("queue_ppb: %d\n", counter_ppb);
	printf("queue_lookup : %d\n", counter_loo);
	printf("BANDWITH     : %f(sec) %d(threads) %0.f(operations) %f(op/sec)\n", t, T, ((double)T*N), ((double)T*N) / t);
	//queue_free(q);
	
	return 0;
}