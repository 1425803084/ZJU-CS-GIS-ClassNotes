//7-20 ���ʽת�� ��25 �֣�
/*�������ʽ��ǰ׺��ʾ������׺��ʾ���ͺ�׺��ʾ��
����ʽ���ճ�ʹ�õ��������ʽ�ǲ�����׺��ʾ��������
Ԫ�����λ����������
���м䡣����Ƴ�����׺���ʽת��Ϊ��׺���ʽ��*/
//baidu�Ĵ��벻֪����ħ��.. 

#include<stdio.h>
#include<ctype.h>
#include<vector>
using namespace std;

int main(){
	vector<char> a;
	char ch;
	int x;
	int flag=0;
	ch=getchar();
	if(ch=='(')
		a.push_back(ch);
	else if(ch!='+')
		putchar(ch);
	while((ch=getchar())!='\n'){
		if(isdigit(ch)&&flag){
			putchar(' ');
			putchar(ch);
			flag=0;
		}else if(isdigit(ch)||ch=='.')
			putchar(ch);
		else if(flag&&(ch=='-'||ch=='+')){
			putchar(' ');
			putchar(ch);
			flag=1;			
		}
		else if(ch=='*'||ch=='/'){
			flag=1;
			if(a.empty())
				a.push_back(ch);
			else if(a.back()=='+'||a.back()=='-'||a.back()=='(')
				a.push_back(ch);
			else{
				putchar(' ');
				putchar(a.back());
				a.pop_back();
				a.push_back(ch);
			}
		}else if(ch=='+'||ch=='-'){
			flag=1;
			if(a.empty()||a.back()=='(')
				a.push_back(ch);
			else{
				while(!a.empty()&&(a.back()!='+'&&a.back()!='-')){
					putchar(' ');
					putchar(a.back());
					a.pop_back();
				}if(!a.empty()){
					putchar(' ');
					putchar(a.back());
					a.pop_back();
				}
				a.push_back(ch);
			}
		}else if(ch=='('){
			a.push_back(ch);
		}else{
			while(!a.empty()&&a.back()!='('){
				putchar(' ');
				putchar(a.back());
				a.pop_back();
			}
			if(!a.empty())
				a.pop_back();
		}
	}
	while(!a.empty()){
		putchar(' ');
		putchar(a.back());
		a.pop_back();
	}
	return 0;
}
