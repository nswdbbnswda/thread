#include<iostream>
#include<pthread.h>
#include<queue>
using namespace std;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue<int> task_queue;
#define POLL_SIZE 100

void* work_thread(void *arg){
	pthread_mutex_lock(&mutex);
	while(task_queue.size() == 0){
		pthread_cond_wait(&cond, &mutex);
	}
	cout<<task_queue.front()<<endl;
	task_queue.pop();
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void init_thread_pool(int size){
	pthread_t tids[POLL_SIZE];
	for(int i = 0; i <  POLL_SIZE; i++){
		task_queue.push(i);
	}
	for(int i = 0; i < POLL_SIZE; i++){
		pthread_create(&tids[i], NULL, work_thread, NULL);
	}
	for(int i = 0; i < POLL_SIZE; i++){
		pthread_join(tids[i], NULL);
	}
}

void destroy(){
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);
}

int main(){
	init_thread_pool(POLL_SIZE);
	destroy();
	pthread_exit(0);
}
