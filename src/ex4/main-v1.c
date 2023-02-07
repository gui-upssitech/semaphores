#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shm.h"
#include "ipc.h"

struct pargs_t {
  int sem_id;
  int nb_process;
  int shm_id;
  int tab_size;
};

typedef struct pargs_t pargs_t;

void fils(int num_fils, pargs_t args) {
  int *tab = (int *) attach_shared_memory(args.shm_id);

  P(args.sem_id, num_fils);

  for (int i = 0; i < args.tab_size; i++) {
    tab[i] += 1;
  }

  printf("Processus %d (%d): ", num_fils, getpid());
  for (int i = 0; i < args.tab_size; i++) printf("%d ", tab[i]);
  printf("\n");

  V(args.sem_id, (num_fils + 1) % args.nb_process);

  exit(0);
}

void launch_processes_sync(int nb_process, pargs_t args) {
  for (int i = 0; i < nb_process; i++) {
    // fork : création d'un processus fils
    switch (fork()) {
      case -1:
        fprintf(stderr, "Erreur fork.\n");
        exit(1);
      case 0:
        fils(i, args);
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

    pargs_t args = {
      .sem_id = create_semaphores(IPC_PRIVATE, nb_process),
      .shm_id = create_shared_memory(IPC_PRIVATE, size),
      .nb_process = nb_process,
      .tab_size = tab_size
    };

    launch_processes_sync(nb_process, args);

    destroy_semaphores(args.sem_id);
    destroy_shared_memory(args.shm_id);
    
    return 0;
}