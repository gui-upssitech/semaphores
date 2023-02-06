#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void fils(int num_fils, int ipc_id, int nb_messages, int nb_lignes);

void parse_args(int argc, char *argv[], int *nb_messages, int *nb_lignes) {
  if (argc != 3) {
    fprintf(stderr, "Usage : %s {nb_messages} {nb_lignes}\n", argv[0]);
    exit(1);
  }

  *nb_messages = atoi(argv[1]);
  *nb_lignes = atoi(argv[2]);
}

void launch_processes(int nb_process, int ipc_id, int nb_messages, int nb_lignes) {
  for (int i = 0; i < nb_process; i++) {
    // fork : création d'un processus fils
    switch (fork()) {
      case -1:
        fprintf(stderr, "Erreur fork.\n");
        exit(1);
      case 0:
        fils(i, ipc_id, nb_messages, nb_lignes);
    }
  }
}

void wait_for_processes(int nb_process) {
  for (int i = 0; i < nb_process; i++) {
    wait(NULL);
  }
}