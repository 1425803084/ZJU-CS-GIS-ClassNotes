/*7-34 ������ȵĺ����� ��25 �֣�
����˵���������һ�и�����������N����100����
������1~N��š����N�У�ÿ�и���һ����������������ϣ�
���ȸ������������е���������K��
������K���������ţ�����֮�䶼�ÿո�ָ���*/

#include<iostream>
using namespace std;

int main(){
	int n,a[101][101]={0},b[101]={0};
	//aΪ�ڽӾ���bΪÿ���ڵ����ȣ� 
	cin>>n;
	int i,j;
	int count=n;//���м��������� 
	for(i=1;i<=n;i++){
		int x,k;
		cin>>x;
		b[i]=x;
		for(j=0;j<x;j++){
			cin>>k;
			a[i][k]=1;
		}
	}
	while(count){
		i=1;
		while(i<=n&&b[i])	++i;
		if(i>n) break;
		b[i]=-1;
		count--;
		for(j=1;j<=n;j++){
			if(a[j][i])
				--b[j];
		}
	}
	if(!count)
		cout<<1;
	else if(i>n)
		cout<<0;
	return 0;
}
