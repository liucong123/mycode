#include<iostream>
using namespace std;

int longestPD(const char*s,int n)
{
	int i,j,theMax,temp;
	
	if(s==0 || n<1)
	{
		return 0;
	}
	
	for(i=0;i<n;i++)
	{
		//回文长度为奇数 
		for(j=0;(i-j>=0)||(i+j<n);j++)
		{
			if(s[i-j]!=s[i+j])
			{
				break;
			}
			temp=j*2+1;
		}		
		//回文长度为偶数 
		for(j=0;(i-j>0)||(i+j+1<n);j++)
		{
			if(s[i-j]!=s[i+j+1])
			{
				break;
			}
			temp=j*2+2;
		}
		if(temp>theMax)
		{
			theMax=temp;
		}
	}
	return theMax;
 } 
 
 int main()
 {
 	const int size=100;
	char s[size]="";
	char *ps=s;
	int count=0;
	cout<<"please input a string: ";
	cin>>s;
	
	while(*ps !='\0')
	{
		count++;
		ps++;
	}
	
	cout<<endl<<"your input string is : "<<s;
	cout<<endl<<"the size of "<<s<<" is : "<<count;
	cout<<endl<<"the longest Palindrome is : "<<longestPD(s,count)<<endl;
 	
 } 
