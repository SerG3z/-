#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define HASHTAB_SIZE 1024

struct node {
	int key;
	int value;
	struct node *next;
};

pthread_mutex_t mutex; 
struct node *hashtab[HASHTAB_SIZE];

void lock()
{
	pthread_mutex_lock(&mutex);
}

void unlock()
{
	pthread_mutex_unlock(&mutex);
}

int hash(int key)
{
	return key % HASHTAB_SIZE;
}

void hashtab_init(struct node **hashtab)
{
	pthread_mutex_init(&mutex, NULL);
	
	for (int i = 0; i < HASHTAB_SIZE; i++) {
		hashtab[i] = NULL;
	}
}

void hashtab_add(struct node **hashtab, int key, int value)
{
	struct node *node;
	int index = hash(key);
	
	//Вставка в начало списка
	node = malloc(sizeof(*node));
	if (node != NULL) {
		node->key = key;
		node->value = value;
		node->next = hashtab[index];
		hashtab[index] = node;
	}
}

struct node *hashtab_lookup(struct node **hashtab, int key)
{
	int index;
	struct node *node;

	index = hash(key);

	for (node = hashtab[index]; node!= NULL; node = node->next){
		if(node->key == key) {
			return node;
		}
	}
	return NULL;
}

void hashtab_delete(struct node**hashtab, int key)
{
	int index;
	struct node *p, *prev = NULL;

	index = hash(key);

	for (p = hashtab[index]; p != NULL; p = p->next) {
		if(p->key == key) {
			if (prev == NULL) 
				hashtab[index] = p->next;
			else
				prev->next = p->next;
			free(p);
			return;
		}
		prev = p;
	}
}

int main(int argc, char const *argv[])
{
	hashtab_init(hashtab);	

	for (int i = 0; i < 10; i++) {
		lock();
		hashtab_add(hashtab, i, 0);
		unlock();
	}

	for (int i = 0; i < 10; i++) {
		lock();
		hashtab_delete(hashtab, i);
		unlock();
	}

	for (int i = 0; i < 10; i++) {
		lock();
		struct node *node = hashtab_lookup(hashtab, i);
		unlock();

		if (node) {
			printf("%d\n",node->key);
		} else {
			printf("not found\n");
		}
	}
	return 0;
}