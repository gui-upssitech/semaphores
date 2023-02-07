#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shm.h"


#define SHM_KEY IPC_PRIVATE

void fils(int num_fils, int shm_id, int tab_size) {
  int *tab = (int *) attach_shared_memory(shm_id);

  for (int i = 0; i < tab_size; i++) {
    tab[i] += 1;
  }

  printf("Processus %d (%d): ", num_fils, getpid());
  for (int i = 0; i < tab_size; i++) printf("%d ", tab[i]);
  printf("\n");

  exit(0);
}

void launch_processes_sync(int nb_process, int shm_id, int tab_size) {
  for (int i = 0; i < nb_process; i++) {
    // fork : création d'un processus fils
    switch (fork()) {
      case -1:
        fprintf(stderr, "Erreur fork.\n");
        exit(1);
      case 0:
        fils(i, shm_id, tab_size);
    }
  }

  for (int i = 0; i < nb_process; i++) {
    wait(NULL);
  }
}

void parse_args(int argc, char *argv[], int *nb_process, int *tab_size) {
  if (argc != 3) {
    fprintf(stderr, "Usage : %s <nb_process> <tab_size>\n", argv[0]);
    exit(1);
  }

  *nb_process = atoi(argv[1]);
  *tab_size = atoi(argv[2]);
}


int main(int argc, char *argv[]) {
    
    int nb_process, tab_size;
    parse_args(argc, argv, &nb_process, &tab_size);

    int size = tab_size * sizeof(int);
    int shm_id = create_shared_memory(SHM_KEY, size);

    launch_processes_sync(nb_process, shm_id, tab_size);
    destroy_shared_memory(shm_id);
    
    return 0;
}