#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "semaphoreP.h"

typedef sem_t Semaphore;

Semaphore *make_semaphore(int value){
          Semaphore *semaphore = (Semaphore *) malloc(sizeof(Semaphore));
          semaphore = sem_open("/semaphore", O_CREAT, 0644, value);
          sem_unlink("/semaphore");
          return semaphore;
}

void semaphore_wait(Semaphore *semaphore){
          sem_wait(semaphore);
}

void semaphore_signal(Semaphore *semaphore){
          sem_post(semaphore);
}