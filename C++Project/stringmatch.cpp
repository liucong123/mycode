#include<iostream>
#include<string>

using namespace std;

bool match(string &model,string &str1)
{
    int len1=model.size();
    int len2=str1.size();
  //  if(model==NULL) return false;
    //if(str1==NULL) return false;
    int mark=0;  //���ڷֶα�ǣ���*���ָ���ַ���
    int pm=0,ps=0;

    while(pm<len1 &&ps<len2)
    {
        if(model[pm]=='?')
        {
            pm++;ps++;continue;
        }
        if(model[pm]=='*')
        { //����ǰ��*,��markǰ��һ�����Ѿ����ƥ�䣬�ӵ�ǰ��ʼ��һ��ƥ�䡣
            pm++;
            mark=pm;
            continue;
        }
        if(str1[ps]!=model[pm])
        {
            if (pm == 0 && ps == 0)
            {

               // ��������ַ������⴦������ͬ��ƥ��ʧ��

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
            * ����ʾ��ͼ��ʾ���ڱȵ�e��c�������
            * p2���ص�mark����
            * p1��Ҫ���ص���һ��λ�á�
            * ��Ϊ*ǰ�Ѿ����ƥ�䣬����mark���֮ǰ�Ĳ���Ҫ�ٱȽ�
            */
            ps -= pm- mark - 1;
            pm = mark;
            continue;
        }
        pm++;ps++; //������ȵ����
    }
    //ѭ���꣬��pm=len1��pm!=len1�������
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
