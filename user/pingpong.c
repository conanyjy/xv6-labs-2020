// user/pingpang.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// int main(int argc, char **argv) {
//     // 创建两个管道：pp2c父->子，pc2p字->父
//     int pp2c[2], pc2p[2];
//     pipe(pp2c);
//     pipe(pc2p);

//     if(fork() != 0) {
//         // 父向子发一个字符
//         write(pp2c[1], ".", 1);
//         close(pp2c[1]);

//         // 父从子读一个字符
//         char buf;
//         read(pc2p[0], &buf, 1);
//         printf("%d: received pong\n", getpid());
//         // 等待子进程结束
//         wait(0);
//     } else {
//         // 子进程从父读取一个字符
//         char buf;
//         read(pp2c[0], &buf, 1);
//         printf("%d: received ping\n", getpid());

//         // 子向父发送一个字符
//         write(pc2p[1], &buf, 1);
//         close(pc2p[1]);
//     }

//     // 关闭管道的读端
//     close(pp2c[0]);
//     close(pc2p[0]);

//     exit(0);
// }

// 改进
int main(int argc, char **argv) {
    // 创建两个管道：pp2c父->子，pc2p字->父
    int pp2c[2], pc2p[2];
    if (pipe(pp2c) < 0 || pipe(pc2p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if(fork() != 0) {
        // 父向子发一个字符
        if (write(pp2c[1], ".", 1) != 1) {
            fprintf(2, "write failed\n");
            exit(1);
        }
        close(pp2c[1]);

        // 父从子读一个字符
        char buf;
        if (read(pc2p[0], &buf, 1) != 1) {
            fprintf(2, "read failed\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        // 等待子进程结束
        wait(0);
    } else {
        // 子进程从父读取一个字符
        char buf;
        if (read(pp2c[0], &buf, 1) != 1) {
            fprintf(2, "read failed\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());

        // 子向父发送一个字符
        if (write(pc2p[1], &buf, 1) != 1) {
            fprintf(2, "write failed\n");
            exit(1);
        }
        close(pc2p[1]);
    }

    // 关闭管道的读端
    close(pp2c[0]);
    close(pc2p[0]);

    exit(0);
}
