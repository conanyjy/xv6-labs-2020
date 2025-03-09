// user/primes.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 筛选质数的函数，接受一个管道作为参数
void sieve(int pleft[2]) {
    // 从左邻居读取整数
    int p;
    read(pleft[0], &p, sizeof(p));
    if(p == -1) {
        // 如果读到-1，表示结束，退出进程
        exit(0);
    }
    // 此时接收到的数字都是质数
    printf("prime %d\n", p);

    // 创建一个新的管道
    int pright[2];
    pipe(pright);

    // 右邻居
    if(fork() == 0){
        // 右邻居用不到这个管道的写端，关闭
        close(pright[1]);
        // 左邻居用不到这个管道的读端，关闭
        close(pleft[0]);
        // 递归调用筛选函数
        sieve(pright);
    } else {
        // 当前进程用不到这个管道的读端，关闭
        close(pright[0]);
        // 从左邻居接受数字
        int buf;
        while(read(pleft[0], &buf, sizeof(buf) && buf != -1)) {
            // 如果接收到的数字不是第一次接收到的数字的倍数，
            // 才往管道中给右邻居写入这个数字
            if(buf % p != 0) {
                write(pright[1], &buf, sizeof(buf));
            }
        }

        // 此时接收到了左邻居传来的-1，要给右邻居也传-1，结束右邻居进程
        buf = -1;
        write(pright[1], &buf, sizeof(buf));
        wait(0);
        exit(0);
    }
}

int main(int argc, char **argv) {
    // 创建初始管道
    int input_pipe[2];
    pipe(input_pipe);

    // 右邻居
    if(fork() == 0) {
        // 右邻居用不到这个管道的写端，关闭
        close(input_pipe[1]);
        // 调用筛选函数
        sieve(input_pipe);
        exit(0);
    } else { // 父进程
        // 父进程只会往管道中给右邻居写数据，关闭父进程的管道读文件描述符
        close(input_pipe[0]);
        int i;
        // 向管道写入2~35的整数
        for(i = 2; i <= 35; i++) {
            write(input_pipe[1], &i, sizeof(i));
        }
        // 写入结束标志
        i = -1;
        write(input_pipe[1], &i, sizeof(i));
    }

    // 等待子进程结束
    wait(0);

    // 无法等待子进程的子进程，只能等待直接子进程，无法等待间接子进程
    // 在 sieve() 中各自执行 wait(0), 形成等待链

    // 退出进程
    exit(0);
}

