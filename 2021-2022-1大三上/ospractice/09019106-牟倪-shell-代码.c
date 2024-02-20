#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(int argc, char* argv[]){
    char *error="An error has occurred\n";
    int countpath=1;char path[50][50];  // 找exe的路径
    strcpy(path[0],"/bin");
    int remain=0;char remamater[60];int remaincount=0;
    int haveredir=0;

    FILE *orig_err=stderr;FILE *orig_out=stdout;

    // 读写初始化
    FILE *in=NULL,*out=stdout,*err=stderr;
    int interact=0;  // 是否交互模式
    if(argc==1) in=stdin,interact=1;
    else{
        if(argc!=2||fopen(argv[1],"r")==NULL)
            {fprintf(err,error);exit(1);}
        in=fopen(argv[1],"r");
    }

    char line[60];size_t len = 0;  // 用来读命令
    if(interact)fprintf(out,"seush> ");

    char input[50][50];int icount=0;  // input count
    // 循环体
    while(1){
        if(!remain)  // 要不要读一行新的
            if(interact)gets(line);
            else if(fgets(line,100,in)==NULL)break;

        while(line[strlen(line)-1]=='\n'||line[strlen(line)-1]==' '
            ||line[strlen(line)-1]=='&')
            line[strlen(line)-1]='\0';
        // 防止出现末尾换行&等幺蛾子
        
        // 处理&
        int posand=0;
        while(posand<strlen(line)&&line[posand]!='&')++posand;
        if(posand>=strlen(line))remain=0;
        else{
            int pos=posand+1;
            while(line[pos]==' ')++pos;
            for(int i=pos;i<strlen(line);++i)
                remamater[i-pos]=line[i];
            
            remamater[strlen(line)-pos]='\0';
            line[posand]='\0';
            remain=1;
        }
        
        // 重定向
        int redir=0;  // >的位置
        char redfile[60];
        while(redir<strlen(line)&&line[redir]!='>')++redir;
        if(redir<strlen(line)){
            if(redir==0||redir==strlen(line)-1)
                {fprintf(err,error);goto cont;}
            char mater[60];int pos=redir+1;
            while(line[pos]==' ')++pos;
            for(int i=pos;i<strlen(line);++i)
                mater[i-pos]=line[i];
            
            mater[strlen(line)-pos]='\0';
            line[redir]='\0';

            char* redfile1=strtok(mater, " ");
            if(redfile1==NULL||strtok(NULL," ")!=NULL)
                {fprintf(err,error);goto cont;}
            strcpy(redfile,".");strcat(redfile,redfile1);
            haveredir=1;
        }

        char *p=strtok(line, " ");
        while(p!=NULL){
            strcpy(input[icount],p);
            int len=strlen(input[icount]);
            if(input[icount][len-1]=='\n'||input[icount][len-1]==' ')
                input[icount][len-1]='\0';
            ++icount;
            p=strtok(NULL," ");
        }

        if(icount==0)goto cont;

        if(!strcmp(input[0],"exit")){
            if(icount!=1){fprintf(err,error);goto cont;}
            exit(0);
        }
        else if(!strcmp(input[0],"path")){
            countpath=0;
            for(int i=1;i<icount;++i)
                strcpy(path[countpath++],input[i]);
        }
        else if(!strcmp(input[0],"cd")){
            if(icount!=2){fprintf(err,error);goto cont;}
            if(chdir(input[1])){fprintf(err,error);goto cont;}
        }
        else{
            pid_t fpid=fork();
            if(fpid<0){fprintf(err,error);goto cont;}
            else if(fpid==0){
                // 执行
                int cp=0;char exeit[60];
                while(cp<countpath){
                    strcpy(exeit,path[cp]);
                    strcat(exeit,"/");
                    strcat(exeit,input[0]);
                    if(access(exeit,X_OK)==0)break;
                    ++cp;
                }
                if(cp>=countpath){
                    fprintf(err,error);exit(0);
                }
                
                char *exarg[50];
                for(int i=0;i<icount;++i){
                    exarg[i]=(char*)malloc(60);
                    if(input[i][0]=='/'&&input[i][1]!='n')  // sorry
                        {strcpy(exarg[i],".");strcat(exarg[i],input[i]);}
                    else strcpy(exarg[i],input[i]);
                }
                exarg[icount]=NULL;
                
                if(haveredir){
                    char temp[60];strcpy(temp,redfile);int i=strlen(redfile)-1;
                    for(;i>1;--i){
                        if(temp[i]=='/')
                            {temp[i]='\0';break;}
                    }
                    mkdir(temp,0777);
                    fclose(fopen(redfile,"w"));
                    freopen(redfile,"w",stdout);
                }
                
                if(execv(exeit,exarg))exit(0);
            }
            else{
                ++remaincount;
                if(!remain){
                    for(int i=0;i<remaincount;++i)wait(NULL);
                    remaincount=0;
                }
            }
        }

    cont:
        memset(input,0,sizeof(input));
        icount=0;
        haveredir=0;
        if(remain)strcpy(line,remamater);
        else if(interact)fprintf(out,"seush> ");
    }
}