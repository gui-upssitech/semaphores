#include "helper.h"
#include "ipc.h"

#define N 2  // nombre de processus fils dans ce modèle de code

/* Comportement d'un processus fils */
void fils(int num_fils, int ipc_id, int nb_messages, int nb_lignes) {
  for (int i = 0; i < nb_messages; i++) {
    P(ipc_id, num_fils);

    for (int j = 0; j < nb_lignes; j++) {
      printf("Processus %d (%d): Ligne %d/%d du message %d/%d \n", num_fils, getpid(), j+1, nb_lignes, i+1, nb_messages);
    }
    printf("\n");

    V(ipc_id, (num_fils + 1) % N);
  }

  // printf ("Processus fils (%d) : Termine \n", getpid());
  exit(0);
}

int main (int argc, char *argv[]) {

  int nb_messages, nb_lignes;
  parse_args(argc, argv, &nb_messages, &nb_lignes);

  int ipc_id = create_semaphores(IPC_PRIVATE, N);

  launch_processes(N, ipc_id, nb_messages, nb_lignes);
  wait_for_processes(N);

  destroy_semaphores(ipc_id);

  printf ("Processus pere (%d) : Termine \n", getpid());

  return 0;
}

