#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 运行指定的程序，接受参数
void run(char* program, char** args) {
    // 创建子进程，在子进程中执行指定的程序
    if(fork() == 0) {
        exec(program, args);
        exit(0);
    }
    return;
}

int main(int argc, char *argv[]) {
    char buf[2048];
    char* p = buf, * last_p = buf;
    char* argsbuf[128];
    char** args = argsbuf;

    // 首先将 xargs 的参数复制到 argsbuf 中
    for(int i = 1; i < argc; i++) {
        *args = argv[i];
        args++;
    }

    // 记录当前参数的位置
    char** pa = args;

    // 从标准输入读取数据，存储在缓冲区buf中
    while(read(0, p, 1) != 0) {
        // 使用指针p遍历缓冲区，遇到空格或者换行符时，将其替换为字符串结束符 \0
        if(*p == ' ' || *p == '\n') {
            *p = '\0';

            // 将参数添加到参数缓冲区argsbuf中
            *(pa++) = last_p;
            last_p = p + 1;

            // 每当遇到换行符\n时，表示一组参数读取完毕，调用run函数执行程序，传递参数
            if(*p == '\n') {
                *pa = 0;
                run(argv[1], argsbuf);
                pa = args;
            }
        }
        // 继续读取数据
        p++;
    }

    // 如果最后一行不是空行，同样的逻辑再处理一次
    if(pa != args) {
        *p = '\0';
        *(pa++) = last_p;
        *pa = 0;

        run(argv[1], argsbuf);
    }

    // 等待所有的子进程结束
    while(wait(0) != -1){};
    exit(0);
}

