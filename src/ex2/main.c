#include "ipc.h"

void parse_arguments(int argc, char *argv[], int *nb_process, int *process_id, int *nb_messages, int *nb_lignes) {
    if (argc != 5) {
        printf("Usage: %s <nb_process> <id_process> <nb_messages> <nb_lignes>\n", argv[0]);
        exit(1);
    }

    *nb_process = atoi(argv[1]);
    *process_id = atoi(argv[2]);
    *nb_messages = atoi(argv[3]);
    *nb_lignes = atoi(argv[4]);
}

int main(int argc, char *argv[]) {

    int nb_process, process_id, nb_messages, nb_lignes;
    parse_arguments(argc, argv, &nb_process, &process_id, &nb_messages, &nb_lignes);

    int token = 0x1234;

    int ipc_id = get_semaphores(token);
    if(ipc_id == -1) ipc_id = create_semaphores(token, nb_process);

    for(int i = 0; i < nb_messages; i++) {
        P(ipc_id, process_id);
        for(int j = 0; j < nb_lignes; j++) {
            printf("Processus %d: Ligne %d/%d du message %d/%d \n", process_id, j+1, nb_lignes, i+1, nb_messages);
        }
        printf("\n");
        V(ipc_id, (process_id + 1) % nb_process);
    }

    if (process_id == nb_process - 1) {
        destroy_semaphores(ipc_id);
    }    

    return 0;
}
