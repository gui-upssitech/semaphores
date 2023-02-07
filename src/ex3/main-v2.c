#include <stdio.h>
#include <stdlib.h>

#include "shm.h"

void parse_args(int argc, char *argv[], int *nb_process, int *process_id, int *tab_size) {
  if (argc != 4) {
    fprintf(stderr, "Usage : %s <nb_process> <process_id> <tab_size>\n", argv[0]);
    exit(1);
  }

  *nb_process = atoi(argv[1]);
  *process_id = atoi(argv[2]);
  *tab_size = atoi(argv[3]);
}

int main(int argc, char *argv[]) {

    int nb_process, process_id, tab_size;
    parse_args(argc, argv, &nb_process, &process_id, &tab_size);

    int key = 0x1234;
    int size = tab_size * sizeof(int);

    int shm_id = get_shared_memory(key);
    if(shm_id == -1) shm_id = create_shared_memory(key, size);

    for(int i = 0; i < nb_process; i++) {
        int *tab = (int *) attach_shared_memory(shm_id);

        for (int i = 0; i < tab_size; i++) {
            tab[i] += 1;
        }

        printf("Processus %d: ", process_id);
        for (int i = 0; i < tab_size; i++) printf("%d ", tab[i]);
        printf("\n");

        exit(0);
    }

    if (process_id == nb_process - 1) {
        destroy_shared_memory(shm_id);
    }    

    return 0;
}
