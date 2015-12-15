#include<iostream>
using namespace std;

bool isPalindrome(const char *s,int n)
{
	//非法输入
	if(s==NULL || n<1)
	{
		return false;
	 } 
	  const char *front,*tail;
	 
	 //初始化头指针和尾指针
	 front=s;
	 tail=s+n-1;
	 while(front<tail)
	 {
	 	if(*front != * tail)
	 	{
	 		return false;
		 }
		front++;
		tail--;
	  } 
	  return true;
 } 
 int main(){
 	const int size=100;
	 char s[size] ="";
	 char *p=s;
	 int count=0;
	 cout<<"please input your string: ";
	 cin>>s;
	 cout<<endl<<s<<endl<<sizeof(s)<<endl;
	 while(*p != '\0')
	 {
	 	count++;
	 	p++;
	 }
	 cout<<count<<endl;
	 if(isPalindrome(s,count)){
	 	cout<<endl<<s<<" is a palindrome ";
	 }
	 else{
	 	cout<<endl<<s<<" is not a palindrome";
	 }
 }
