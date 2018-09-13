#include <iostream>
#include <cstdlib>

#define cycle %param1%
#define blocki %param2%
#define blockj %param3%
#define blockk %param4%

using namespace std;

void transpose_matrix(double* matrix, int N){
    for(int i=0;i<N;i++)
    for(int j=i+1;j<N;j++)
    {
        double temp=matrix[i*N+j];
        matrix[i*N+j]=matrix[j*N+i];
        matrix[j*N+i]=temp;
    }
}

int main(int argc, char** argv){

    int N = atoi(argv[1]);
    if (N % blocki != 0 || N % blockj != 0 || N % blockk != 0) {
      cout << "N % Block size != 0\n";
      exit(1);
    }
    double *a,*b,*c;

    if(posix_memalign((void**)&a,256,N*N*sizeof(double))){
        cout<<"Error in memalign\n";
        exit(2);
    }
    if(posix_memalign((void**)&b,256,N*N*sizeof(double))){
        cout<<"Error in memalign\n";
        exit(2);
    }
    if(posix_memalign((void**)&c,256,N*N*sizeof(double))){
        cout<<"Error in memalign\n";
        exit(2);
    }
    srand(0);
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
      {
          a[i*N+j] = rand()%2;
          b[i*N+j] = rand()%2;
          c[i*N+j] = 0;
      }

    clock_t startCPU = clock();
    
    transpose_matrix(b,N);
    
    if(cycle==1)
    {
        for (int i1 = 0; i1 < N; i1 += blocki)
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int i2 = 0; i2 < blocki; ++i2)
        {
            int inda = (i1 + i2)*N + k1;
            int indc=(i1+i2)*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==2)
    {
        for (int i1 = 0; i1 < N; i1 += blocki)
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int i2 = 0; i2 < blocki; ++i2)
        {
            int inda = (i1 + i2)*N + k1;
            int indc=(i1+i2)*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==3)
    {
        for (int i1 = 0; i1 < N; i1 += blocki)
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int i2 = 0; i2 < blocki; ++i2)
        for (int k1 = 0; k1 < N; k1 += blockk)    
        {
            int inda = (i1 + i2)*N + k1;
            int indc=(i1+i2)*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==4)
    {
        for (int i1 = 0; i1 < N; i1 += blocki)
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int i2 = 0; i2 < blocki; ++i2)
        for (int j1 = 0; j1 < N; j1 += blockj)
        {
            int inda = (i1 + i2)*N + k1;
            int indc=(i1+i2)*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==5)
    {
        for (int i = 0; i < N; i++)
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int k1 = 0; k1 < N; k1 += blockk)
        {
            int inda = i*N + k1;
            int indc=i*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==6)
    {
        for (int i = 0; i < N; i++)
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int j1 = 0; j1 < N; j1 += blockj)
        {
            int inda = i*N + k1;
            int indc = i*N + j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==7)
    {
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int i1 = 0; i1 < N; i1 += blocki)
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int i2 = 0; i2 < blocki; ++i2)
        {
            int inda = (i1 + i2)*N + k1;
            int indc=(i1+i2)*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==8)
    {
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int i = 0; i < N; i++)
        for (int k1 = 0; k1 < N; k1 += blockk)
        {
            int inda = i*N + k1;
            int indc = i*N + j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==9)
    {
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int i = 0; i < N; i++)
        {
            int inda = i*N + k1;
            int indc = i*N + j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==10)
    {
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int i1 = 0; i1 < N; i1 += blocki)
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int i2 = 0; i2 < blocki; ++i2)
        {
            int inda = (i1 + i2)*N + k1;
            int indc=(i1+i2)*N+j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==11)
    {
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int i = 0; i < N; i++)
        for (int j1 = 0; j1 < N; j1 += blockj)
        {
            int inda = i*N + k1;
            int indc = i*N + j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {               
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    if(cycle==12)
    {
        for (int k1 = 0; k1 < N; k1 += blockk)
        for (int j1 = 0; j1 < N; j1 += blockj)
        for (int i = 0; i < N; i++)
        {
            int inda = i*N + k1;
            int indc = i*N + j1;
            for (int j2 = 0; j2 < blockj; ++j2)
            {                
                int indb = (j1 + j2)*N + k1;
                double c_temp=0;
                
                for (int k2 = 0; k2 < blockk; ++k2)
                    c_temp += a[inda + k2] * b[indb + k2];
                c[indc+j2]+=c_temp;
            }
        }
    }
    double total_time = (double)(clock()-startCPU)/CLOCKS_PER_SEC;

    cout<<total_time;
   // cout <<"Performance=" << (double)N*N*N*2/total_time/1e9<<" GFlops"<< endl<<endl;
    return 0;
}
