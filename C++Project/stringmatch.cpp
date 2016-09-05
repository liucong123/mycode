#include<iostream>
#include<string>

using namespace std;

bool match(string &model,string &str1)
{
    int len1=model.size();
    int len2=str1.size();
  //  if(model==NULL) return false;
    //if(str1==NULL) return false;
    int mark=0;  //用于分段标记，“*”分割的字符串
    int pm=0,ps=0;

    while(pm<len1 &&ps<len2)
    {
        if(model[pm]=='?')
        {
            pm++;ps++;continue;
        }
        if(model[pm]=='*')
        { //若当前是*,则mark前面一部分已经获得匹配，从当前开始下一轮匹配。
            pm++;
            mark=pm;
            continue;
        }
        if(str1[ps]!=model[pm])
        {
            if (pm == 0 && ps == 0)
            {

               // 如果是首字符，特殊处理，不相同即匹配失败

                return false;
            }
            /*
            * pattern: ...*bdef*...
            *              ^
            *             mark
            *                ^
            *                p2
            *              ^
            *             new p2
            * str1:.....bdcf...
            *             ^
            *             p1
            *            ^
            *          new p1
            * 如上示意图所示，在比到e和c处不想等
            * p2返回到mark处，
            * p1需要返回到下一个位置。
            * 因为*前已经获得匹配，所以mark打标之前的不需要再比较
            */
            ps -= pm- mark - 1;
            pm = mark;
            continue;
        }
        pm++;ps++; //处处相等的情况
    }
    //循环完，若pm=len1和pm!=len1两种情况
        if(pm==len1)
        {
            if(ps==len2)
                return true;
            if(model[pm-1]=='*')
                return true;
        }

        else
        {
            while(pm<len1)
         	{
             if(model[pm]!='*')
                    return false;
              pm++;
          	}
        }


        return true;
}


int main()
{
    string model;
    string str1;
    while(cin>>model>>str1)
    {
        if(match(model,str1))
        {
            cout<<"true"<<endl;
        }
        else
        {
            cout<<"false"<<endl;
        }
    }
    return 0;
}
