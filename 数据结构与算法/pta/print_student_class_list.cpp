//7-47 ��ӡѡ��ѧ������ ��25 �֣�
/*����ĵ�һ����������������N����40000����
ΪȫУѧ��������K����2500����Ϊ�ܿγ������˺�N�У�
ÿ�а���һ��ѧ��������3����дӢ����ĸ+1λ���֣���
һ��������C����20�����������ѡ�Ŀγ�������
�����C���γ̱�š���������γ̴�1��K��š�*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<list>
#include<vector>
using namespace std;

struct student{
	char name[5];
	int* c;//����ÿ��ѧ����ѡ�Σ� 
}stu[40001];//��ͷ��ѧ�����֣����� 

int cla[40001][20]={0};//ÿ��ѧ���Ŀγ̣� 
int count[2501]={0};//ÿ�ſγ̵������� 
int cmp(const void *a,const void *b){
	return strcmp(((struct student*)a)->name,((struct student*)b)->name);
}

int main(){
	int n,k;
	scanf("%d %d\n",&n,&k);
	int i,x,j,c,m,t;
	char name[5];
	vector<list<int> > a(k+1);
	for(i=0;i<n;i++){
		scanf("%s %d",name,&x);
		strcpy(stu[i].name,name);
		stu[i].c=cla[i]; 
		for(j=0;j<x;j++){
			scanf("%d",&t);
			cla[i][j]=t;
			count[t]++;
		}
	}
	qsort(stu,n,sizeof(struct student),cmp);
	for(i=0;i<n;i++){
		for(j=0;j<20;j++)
		if(stu[i].c[j]){
			a[stu[i].c[j]].push_back(i);			
		}//ת��ÿ���γ̵����� 
	}
	for(i=1;i<=k;i++){
		printf("%d %d\n",i,count[i]);
		list<int>::iterator a1;
		for(a1=a[i].begin();a1!=a[i].end();++a1){
			printf("%s\n",stu[*a1].name);		
		}
	}
	return 0;
}
