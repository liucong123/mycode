#include<iostream>

using namespace std;
const int M=3;
const int N=3;
int _max(int a,int b){return a>=b ? a:b;}
int _min(int a,int b){return a<=b ? a:b;}
int findmaxPath(int a[M][N],int row_begin,int col_begin)
{
    int max1;
    //当走到最后一行时
    if(row_begin==M-1)
    {
        max1=0;
        for(int i=col_begin;i<N;i++)
        {
            max1+=a[row_begin][i];

        }
        return max1;
    }
    //当走到最后一列时
    if(col_begin==N-1)
    {
         max1=0;
         for(int i=row_begin;i<M;i++)
         {
               max1+=a[i][col_begin];


         }
         return max1;

    }

    //利用递归找寻最大和的子路径
    return _max(a[row_begin][col_begin]+findmaxPath(a,row_begin+1,col_begin),a[row_begin][col_begin]+findmaxPath(a,row_begin,col_begin+1));
//    return a[row_begin][col_begin]+findmaxPath(a,row_begin+1,col_begin)>= a[row_begin][col_begin]+findmaxPath(a,row_begin,col_begin+1)?
//    a[row_begin][col_begin]+findmaxPath(a,row_begin+1,col_begin) : a[row_begin][col_begin]+findmaxPath(a,row_begin,col_begin+1);
}
int findminPath(int a[M][N],int row_begin,int col_begin)
{
     int max1;
    //当走到最后一行时
    if(row_begin==M-1)
    {
        max1=0;
        for(int i=col_begin;i<N;i++)
        {
            max1+=a[row_begin][i];

        }
        return max1;
    }
    //当走到最后一列时
    if(col_begin==N-1)
    {
         max1=0;
         for(int i=row_begin;i<M;i++)
         {
               max1+=a[i][col_begin];

         }
         return max1;

    }
    //利用递归找寻最小和的子路径
    return _min(a[row_begin][col_begin]+findminPath(a,row_begin+1,col_begin),a[row_begin][col_begin]+findminPath(a,row_begin,col_begin+1));
}

int main()
{
    int m[M][N]=
    {
        {3, 2, 100},
        {3, 5, 5},
        {0, 2, 4}
    };
    int ml[M][N]={0};
    for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
         cout<<m[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl<<_max(5,4)<<endl;
    cout<<findmaxPath(m,0,0)<<endl;
    cout<<findminPath(m,0,0)<<endl;

    return 0;


}
