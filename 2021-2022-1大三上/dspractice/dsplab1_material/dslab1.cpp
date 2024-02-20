#include<iostream>
#include<cstdio>
#include<cstring>
#include<fstream>
#include"Matrix.h"

using namespace std;

int main()
{
    int mat_size[3]={10, 100};//, 1000
    int cache_size[4]={1, 5, 10, 50};
    for(int I=0;I<2;++I)
        for(int J=0;J<4;++J){
            // declaration
            int* a=new int[mat_size[I]*mat_size[I]];
            int* c=new int[mat_size[I]*mat_size[I]];
            memset(a,0,sizeof(a));
            memset(c,0,sizeof(c));
            for(int K=0;K<mat_size[I];++K) a[K*mat_size[I]+K]=1;

            int tempa, tempb, tempc;
            // ijk
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int i=0;i<mat_size[I];++i)
                for(int j=0;j<mat_size[I];++j)
                    for(int k=0;k<mat_size[I];++k){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, ijk order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // ikj
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int i=0;i<mat_size[I];++i)
                for(int k=0;k<mat_size[I];++k)
                    for(int j=0;j<mat_size[I];++j){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, ikj order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // jik
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int j=0;j<mat_size[I];++j)
                for(int i=0;i<mat_size[I];++i)
                    for(int k=0;k<mat_size[I];++k){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, jik order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // jki
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int j=0;j<mat_size[I];++j)
                for(int k=0;k<mat_size[I];++k)
                    for(int i=0;i<mat_size[I];++i){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, jki order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // kij
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int k=0;k<mat_size[I];++k)
                for(int i=0;i<mat_size[I];++i)
                    for(int j=0;j<mat_size[I];++j){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, kij order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }

            // kji
            {
            Matrix A("./tempA", mat_size[I], cache_size[J], a);
            Matrix B("./tempB", mat_size[I], cache_size[J], a);
            Matrix C("./tempC", mat_size[I], cache_size[J], c);
            for(int k=0;k<mat_size[I];++k)
                for(int j=0;j<mat_size[I];++j)
                    for(int i=0;i<mat_size[I];++i){
                        // c[i][j]+=a[i][k]*b[k][j];
                        tempa=A.read(i, k);
                        tempb=B.read(k, j);
                        tempc=tempa*tempb;
                        C.write(i, j, tempc);
                    }
            printf("\nmat size = %d, cache size = %d, kji order\n",
                mat_size[I], cache_size[J]);
            printf("A miss = %lld, B miss = %lld, C miss = %lld\n",
                A.miss, B.miss, C.miss);
            }
            
            delete[] a; delete[] c;
        }
}
