// user/sleep.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// int main(int argc, char **argv) {
//     if(argc < 2) {
//         printf("usage: sleep <ticks>\n");
//     }
//     sleep(atoi(argv[1]));
//     exit(0);
// }

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("usage: sleep <ticks>\n");
        exit(1);
    }

    int ticks = atoi(argv[1]);
    if (ticks <= 0) {
        printf("error: ticks must be a positive integer\n");
        exit(1);
    }

    printf("sleeping for %d ticks...\n", ticks);
    sleep(ticks);
    printf("woke up after %d ticks\n", ticks);
    
    exit(0);
}



