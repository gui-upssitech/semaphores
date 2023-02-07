#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

  //////////////////////////////////////////////////////////
 //                SHM FUNCTIONS                         //
//////////////////////////////////////////////////////////

int create_shared_memory(int key, int size) {
  int shm_id = shmget(key, size, 0666 | IPC_CREAT);

  if(shm_id == -1) {
    perror("Create shared memory");
    exit(1);
  }

  return shm_id;
}

int get_shared_memory(int key) {
  return shmget(key, 0, 0);
}

void* attach_shared_memory(int shm_id) {
  void* ptr = shmat(shm_id, NULL, 0);

  if(ptr == (void *) -1) {
    perror("Attach shared memory");
    exit(1);
  }

  return ptr;
}

void destroy_shared_memory(int shm_id) {
  if(shmctl(shm_id, IPC_RMID, NULL) == -1) {
    perror("Destroy shared memory");
    exit(1);
  }
}