#include<iostream>
#include<vector>

using namespace std;

void funxiao(int *a,int n,vector<int> &res)
{
    res.push_back(a[0]);
    res.push_back(a[1]);
    for(int i=2;i<n;)
    {
        if(res.size()<2)
        {
            res.push_back(a[i]);
            i++;
        }
        else     //���ELSE����ӣ���Ȼ����Ī���Ĵ���
        {
             if(a[i]==res[res.size()-2])
            {
                if(a[i]==res[res.size()-1])
                {

                    int cnt=2;    //ͳ�ƴ���
                    while( (i<n) && (a[i]==res[res.size()-1]))//ע���ж�����Խ��C/C++����עԽ�����
                    {
                        res.push_back(a[i]);
                        i++;
                        cnt++;
                        //continue;
                        // break;
                    }
                    for(int k=0;k<cnt;k++)
                    {
                        res.pop_back();
                    }
                }
                else
                {
                    res.push_back(a[i]);
                    i++;
                }

            }
            else
            {
                res.push_back(a[i]);
                i++;
            }
        }
    }
}
int main()
{
    int a[15]={1,1,2,2,1,1,1,1,1,1,2,2,1,1,5};
    vector<int> res;
    funxiao(a,15,res);
    for(int i=0;i<res.size();i++)
    {
        cout<<res[i]<<" ";
    }
}
