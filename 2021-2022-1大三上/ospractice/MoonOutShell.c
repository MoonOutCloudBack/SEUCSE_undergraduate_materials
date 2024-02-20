#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#define hist_size 1024 
char *hist[hist_size];
int f = 0; // 保存directory变化
int head = 0, filled = 0;

// 输入为字符串，输出为 空格分隔的 第一个单词
char *trim(char *string){
    int i = 0, j = 0;
    char *ptr = malloc(sizeof(char*)*strlen(string));
    for (i = 0; string[i] != '\0'; ++i)
        if (string[i] != ' ')
            ptr[j++] = string[i];
            
    ptr[j] = '\0';
    return ptr;
    /*
    为什么要这么做？？
    string = ptr;
    return string;
    */
}

// 1. 解析用户输入
void parse(char *word, char **argv){

    // 以指针的形式改变argv，使argv存储分隔后的内容，并处理一些简单命令
    int count = 0;
    memset(argv, 0, sizeof(char*)*(64)); // 清零argv中的内容
    char *lefts = NULL;
    const char *split = " ";  // 用空格来分隔
    while (1){
        char *p = strtok_r(word, split, &lefts);
        if (p == NULL){
            break;
        }
        argv[count++] = p;
        word = lefts;
    }
    if (strcmp(argv[0], "exit") == 0) exit(0); // 直接退出
    else if (strcmp(argv[0], "cd") == 0){
        int ch = chdir(argv[1]); // 用于改变当前工作目录，参数为目标目录
        f = 1;
    }
}

// 2. 执行简单命令
void execute(char **argv){
    pid_t pid; int status;

    if ((pid = fork()) < 0){
        printf("error: fork failed.\n");
        exit(1);
    }
    else if (pid == 0){ // 我是子进程
        if (execvp(argv[0], argv) < 0 && strcmp(argv[0], "cd"))
            printf("error: invalid command.\n");
        exit(0);
    }
    else{ // 我是父进程
        while (wait(&status) != pid) // 自旋等待
            ;
    }
}

// 3. 输出的重定向
void  execute_file(char **argv, char *output){
    
    pid_t pid;
    int status;
    char *file = NULL;
    if ((pid = fork()) < 0){
        printf("error: fork failed.\n");
        exit(1);
    }
    else if (pid == 0) { // 子进程的工作
        if (strstr(output, ">")>0){ // 返回字符串中 首次出现子串的地址
        
            char *p = strtok_r(output, ">", &file);
            output += 1; // change2
            file = trim(file); // 得到 > 后的第一个单词，也就是输出重定向的目标文件
            int old_stdout = dup(1);
            
            FILE *fp1 = freopen(output, "w+", stdout);
            execute_file(argv, file);
            fclose(stdout);
            FILE *fp2 = fdopen(old_stdout, "w");
            *stdout = *fp2;
            exit(0);
        }
        if (strstr(output, "<") > 0){ // 还要输入重定向的……
            char *p = strtok_r(output, "<", &file);
            file = trim(file);
            int fd = open(file, O_RDONLY);
            if (fd<0){
                printf("No such file or directory.\n");
                exit(0);
            }
        }
        if (strstr(output, "|") > 0){ // 还有管道的……
            fflush(stdout); printf("here"); fflush(stdout);
            char *p = strtok_r(output, "|", &file);
            file = trim(file);
            
            char *args[64];
            parse(file, args);
            execute(args);
        }

        int old_stdout = dup(1);
        FILE *fp1 = freopen(output, "w+", stdout);

        if (execvp(argv[0], argv) < 0) printf("error: in exec\n");

        fclose(stdout);
        FILE *fp2 = fdopen(old_stdout, "w");
        *stdout = *fp2;
        exit(0);
    }

    // 自旋等待
    else{
        while (wait(&status) != pid)
            ;
    }
}

// 4. 输入的重定向
void  execute_input(char **argv, char *output){
    pid_t pid;
    int fd;
    char *file;
    int flag = 0;
    int status;
    if ((pid = fork()) < 0) {
        printf("error: fork failed\n");
        exit(1);
    }
    else if (pid == 0){ // 和上一个函数代码基本一样
        if (strstr(output, "<") > 0){
            char *p = strtok_r(output, "<", &file);
            file = trim(file);
            flag = 1;
            fd = open(output, O_RDONLY);
            if (fd<0){
                printf("No such file or directory.\n");
                exit(0);
            }
            output = file;
        }
        if (strstr(output, ">") > 0){
            char *p = strtok_r(output, ">", &file);
            file = trim(file);
            flag = 1;
            fflush(stdout);
            fflush(stdout);
            int old_stdout = dup(1);
            FILE *fp1 = freopen(file, "w+", stdout);
            execute_input(argv, output);
            fclose(stdout);
            FILE *fp2 = fdopen(old_stdout, "w");
            *stdout = *fp2;
            exit(0);
        }
        if (strstr(output, "|") > 0) {
            char *p = strtok_r(output, "|", &file);
            file = trim(file);
            flag = 1;
            char *args[64];
            parse(file, args);
            int pfds[2];
            pid_t pid, pid2;
            int status, status2;
            pipe(pfds);
            int fl = 0;
            if ((pid = fork()) < 0){
                printf("error: fork failed\n");
                exit(1);
            }
            if ((pid2 = fork()) < 0){
                printf("error: fork failed\n");
                exit(1);
            }
            if (pid == 0 && pid2 != 0){
                close(1);
                dup(pfds[1]);
                close(pfds[0]);
                close(pfds[1]);
                fd = open(output, O_RDONLY);
                close(0);
                dup(fd);
                if (execvp(argv[0], argv) < 0){
                    close(pfds[0]);
                    close(pfds[1]);
                    printf("error:in exec");
                    fl = 1;
                    exit(0);
                }
                close(fd);
                exit(0);
            }
            else if (pid2 == 0 && pid != 0 && fl != 1){
                close(0);
                dup(pfds[0]);
                close(pfds[1]);
                close(pfds[0]);
                if (execvp(args[0], args) < 0){
                    close(pfds[0]);
                    close(pfds[1]);
                    printf("error:in exec");
                    exit(0);
                }
            }
            else {
                close(pfds[0]);
                close(pfds[1]);
                while (wait(&status) != pid);
                while (wait(&status2) != pid2);
            }
            exit(0);
        }
        fd = open(output, O_RDONLY);
        close(0);
        dup(fd);
        if (execvp(argv[0], argv) < 0)
            printf("error: in exec\n");
        close(fd);
        exit(0);
    }

    else {
        while (wait(&status) != pid)
            ;
    }
}

// 5. 管道的实现1
void execute_pipe(char **argv, char *output) {
    int pfds[2], pf[2], flag;
    char *file;
    pid_t pid, pid2, pid3;
    int status, status2, old_stdout;
    pipe(pfds); // 创建管道！

    int blah = 0;
    char *args[64];
    char *argp[64];
    int fl = 0;
    if ((pid = fork()) < 0){
        printf("error: fork failed\n");
        exit(1);
    }
    if ((pid2 = fork()) < 0){
        printf("error: fork failed\n");
        exit(1);
    }
    if (pid == 0 && pid2 != 0) { // 现在是哪一个进程
        close(1);
        dup(pfds[1]);
        close(pfds[0]);
        close(pfds[1]);
        if (execvp(argv[0], argv) < 0){ // 还是要执行的
            close(pfds[0]);
            close(pfds[1]);
            printf("error: in exec");
            fl = 1;
            kill(pid2, SIGUSR1);
            exit(0);
        }
    }
    if (pid2 == 0 && pid != 0) { // 现在是哪一个进程
        if (fl == 1) exit(0);
        if (strstr(output, "<") > 0) { // 输入重定向
            char *p = strtok_r(output, "<", &file);
            file = trim(file);
            flag = 1;
            parse(output, args); // 把output parse一下，存到args里
            execute_input(args, file);
            close(pfds[0]);
            close(pfds[1]);
            exit(0);
        }
        if (strstr(output, ">") > 0) { // 输出重定向
            char *p = strtok_r(output, ">", &file);
            file = trim(file);
            flag = 1;
            parse(output, args);
            blah = 1;
        }
        else {
            parse(output, args);
        }

        close(0);
        dup(pfds[0]); // 复制文件描述词，新的文件描述词和 参数 指的是同一个文件, 共享所有的锁定、读写位置和各项权限
        close(pfds[1]);
        close(pfds[0]);
        if (blah == 1) { // 输出重定向
            old_stdout = dup(1);
            FILE *fp1 = freopen(file, "w+", stdout);
        }
        if (execvp(args[0], args) < 0) {
            fflush(stdout);
            printf("error: in exec %d\n", pid);
            kill(pid, SIGUSR1);
            close(pfds[0]);
            close(pfds[1]);
        }
        fflush(stdout);
        if (blah == 1) { // 输出重定向
            fclose(stdout);
            FILE *fp2 = fdopen(old_stdout, "w");
            *stdout = *fp2;
        }
    }
    else {
        close(pfds[0]);
        close(pfds[1]);
        while (wait(&status) != pid)
            ;
        while (wait(&status2) != pid2)
            ;
    }
}

// 6. 管道的实现2
void execute_pipe2(char **argv, char **args, char **argp){

    int status;
    int i;
    int pipes[4];
    pipe(pipes);
    pipe(pipes + 2);
    if (fork() == 0) {
        dup2(pipes[1], 1);
        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);
        if (execvp(argv[0], argv) < 0) {
            fflush(stdout);
            printf("error:in exec");
            fflush(stdout);
            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);
            exit(1);
        }
    }
    else {
        if (fork() == 0) {
            dup2(pipes[0], 0);
            dup2(pipes[3], 1);
            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);
            if (execvp(args[0], args) < 0) {
                fflush(stdout);
                printf("error:in exec");
                fflush(stdout);
                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);
                exit(1);
            }
        }
        else {
            if (fork() == 0) {
                dup2(pipes[2], 0);
                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);
                if (execvp(argp[0], argp) < 0) {
                    fflush(stdout);
                    printf("error:in exec");
                    fflush(stdout);
                    close(pipes[0]);
                    close(pipes[1]);
                    close(pipes[2]);
                    close(pipes[3]);
                    exit(1);
                }
            }
        }
    }
    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);

    for (i = 0; i < 3; i++)
        wait(&status);
}
 
int  main() {
    // 初始化
    char line[1024];
    char *argv[64];
    char *args[64];
    char *left;
    size_t size = 0;
    char ch;
    int count = 0;
    char *tri;
    char *second;
    char *file;
    int i;
    for (i = 0; i < hist_size; i++){
        hist[i] = (char *)malloc(150);
    }
    
    // 主循环
    while (1){
        count = 0;
        int flag = 0;
        char *word = NULL;
        char *dire[] = { "pwd" }; // print work directory 用于显示工作目录
        fflush(stdout); // 强制马上输出，避免错误
        printf("MoonOut> ");
        fflush(stdout);
        // execute(dire); // 打印现在的directory
        // printf("$");

        int len = getline(&word, &size, stdin); // 得到一行内容
        if (*word == '\n') // 是回车…
            continue;
        word[len - 1] = '\0';
        char *file = NULL;
        int i = 0;
        char *temp = (char *)malloc(150);
        strcpy(temp, word); // 把word的内容粘到temp里
        parse(temp, argv); // 解析命令
        
        // 存储directory
        strcpy(hist[(head + 1) % hist_size], word); 
        head = (head + 1) % hist_size;
        ++filled;

        if (strcmp(word, "exit") == 0) exit(0); // 请直接退出吧

        // 对字符进行遍历查找，修改flag，决定接下来的策略
        for (i = 0; word[i]!='\0'; ++i){
            if (word[i] == '>'){ // 输出重定向
                char *p = strtok_r(word, ">", &file);
                file = trim(file);
                flag = 1;
                break;
            }
            else if (word[i] == '<'){ // 输入重定向
                char *p = strtok_r(word, "<", &file);
                file = trim(file);
                flag = 2;
                break;
            }
            else if (word[i] == '|'){ // 管道
                char *p = strtok_r(word, "|", &left);
                flag = 3;
                break;
            }
        }        

        // 对每种情况进行分别处理
        if (flag == 1){ // 输出重定向
            parse(word, argv);   //parsed command stored in argv
            execute_file(argv, file);
        }
        else if (flag == 2){ // 输入重定向
            parse(word, argv);
            execute_input(argv, file);
        }
        else if (flag == 3){ // 管道
            char *argp[64];
            char *output, *file;
            if (strstr(left, "|") > 0){
                char *p = strtok_r(left, "|", &file);
                parse(word, argv);
                parse(left, args);
                parse(file, argp);
                execute_pipe2(argv, args, argp);
            }
            else{
                parse(word, argv);
                execute_pipe(argv, left);
            }
        }
        else{ // 简单命令
            parse(word, argv);
            execute(argv);
        }
    }
}