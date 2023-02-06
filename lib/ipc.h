#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#define P(ipc_id, n_sem) operate_semaphore(ipc_id, n_sem, -1)
#define V(ipc_id, n_sem) operate_semaphore(ipc_id, n_sem, 1)

union semun {
  int value;
  struct semid_ds *buffer;
  ushort *array;
};

int create_semaphores(int sem_id, int nb_sem) {
  int ipc_id = semget(sem_id, nb_sem, IPC_CREAT | 0666);

  if (ipc_id == -1) {
    perror("Can't create semaphores");
    exit(1);
  }

  for(int i = 0; i < nb_sem; i++) {
    union semun arg;
    arg.value = (i == 0) ? 1 : 0;
    
    int set = semctl(ipc_id, i, SETVAL, arg);
    if (set == -1) {
      perror("Can't initialize semaphores");
      exit(1);
    }
  }

  return ipc_id;
}

int get_semaphores(int sem_id) {
  return semget(sem_id, 0, 0);
}

void operate_semaphore(int ipc_id, int num_sem, int op_sign) {
  struct sembuf op;
  op.sem_num = num_sem;
  op.sem_op = op_sign;
  op.sem_flg = 0;

  semop(ipc_id, &op, 1);
}

void destroy_semaphores(int ipc_id) {
    int rm = semctl(ipc_id, 0, IPC_RMID);
    if (rm == -1) {
      perror("Failed to destroy semaphores");
      exit(1);
    }
}