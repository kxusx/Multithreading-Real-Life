#include <semaphore.h>

typedef sem_t Semaphore;

Semaphore *make_semaphore(int value);

void semaphore_wait(Semaphore *semaphore);

void semaphore_signal(Semaphore *semaphore);