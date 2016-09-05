#include <stdio.h>
#include<iostream>
#define N  3
#define M  3
using namespace std;
int _max(int a,int b){return a>=b ? a:b;}
int _min(int a,int b) {return a<=b ? a:b;}
/* 求最大路径长度 *///利用path记录路径，对于每一个path[i][j],0代表dp[i][j]的值从上面加过来，1代表dp[i][j]的值从左边加过来
int maxPath(int a[M][N],int path[M][N])
{
    int dp[M][N]={0};
    dp[0][0]=a[0][0];
    for(int i=1;i<N;i++)
    {
        dp[0][i]=dp[0][i-1]+a[0][i];
        path[0][i]=1;

    }
    for(int j=1;j<M;j++)
    {
        dp[j][0]=dp[j-1][0]+a[j][0];
        path[j][0]=0;

    }
    for(int i=1;i<M;i++)
    {
        for(int j=1;j<N;j++)
        {
            int flg=dp[i-1][j]>=dp[i][j-1] ? 0:1;
            dp[i][j]=_max(dp[i-1][j],dp[i][j-1])+a[i][j];
            path[i][j]=flg;
        }
    }
    return dp[M-1][N-1];
}
/* 求最小路径长度 */
int minPath(int a[M][N],int path[M][N])
{
    int dp[M][N]={0};
    dp[0][0]=a[0][0];
    for(int i=1;i<N;i++)
    {
        dp[0][i]=dp[0][i-1]+a[0][i];
        path[0][i]=1;

    }
    for(int j=1;j<M;j++)
    {
        dp[j][0]=dp[j-1][0]+a[j][0];
        path[j][0]=0;

    }
    for(int i=1;i<M;i++)
    {
        for(int j=1;j<N;j++)
        {
            int flg=dp[i-1][j]<=dp[i][j-1] ? 0:1;
            dp[i][j]=_min(dp[i-1][j],dp[i][j-1])+a[i][j];
            path[i][j]=flg;
        }
    }
    return dp[M-1][N-1];

}
int main()
{
    int maxpath, minpath;
    int a[M][N]=
    {
        {1,2,8},
        {3,5,9},
        {0,4,1}
    };
    int p[M][N]={0};
    maxpath = maxPath(a,p);
     for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
         cout<<p[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<maxpath<<endl;
    minpath = minPath(a,p);
     for(int i=0;i<M;i++)
    {
        for(int j=0;j<N;j++)
        {
         cout<<p[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<minpath<<endl;
    return 0;
}
