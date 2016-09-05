#include<iostream>

using namespace std;

bool isit(int m,int n)
{
    int i=1;
    if(m%n==1)
    {
       while(i<n&& m%n==1)
       {
           m=m-(1+m/n);
           i++;
       }
       if((m==1)||(m%n==1))
        return true;
       else
        return false;
    }
    else
    {
        return false;
    }
}
int get_numapple(int n)
{
    for(int i=n;;i++)
    {
        if(isit(i,n))
        {
            return i;
            break;
        }
    }
}

int main()
{
    int n;
    while(cin>>n)
    {
        cout<<get_numapple(n)<<endl;
    }
    return 0;
}
