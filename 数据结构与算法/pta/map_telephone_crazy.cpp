//7-14 �绰������� ��25 �֣�
/*�������ȸ���������N����10?5??����Ϊ
ͨ����¼���������N�У�ÿ�и���һ��ͨ����¼��
�����������ֻ�г��������ͽ��շ���11
λ���ֹ��ɵ��ֻ����룬�����Կո�ָ���*/

#include<iostream>
#include<map>
using namespace std;
//ʹ��map in stl����һ�� 
int main(){
	map<string,int> tele;
	int n,i;
	int max=1;
	string t1,t2;
	cin>>n;
	map<string,int>::iterator tele1;
	for(i=0;i<n;++i){
		cin>>t1>>t2;
		if((tele1=tele.find(t1))!=tele.end()){
			tele1->second++;
			if(tele1->second>max)
				max=tele1->second;			
		}
		else
			tele.insert(pair<string,int>(t1,1));
		if((tele1=tele.find(t2))!=tele.end()){
			tele1->second++;
			if(tele1->second>max)
				max=tele1->second;			
		}
		else
			tele.insert(pair<string,int>(t2,1));
	}
	int count=0;
	for(tele1=tele.begin();tele1!=tele.end();++tele1){
		if(tele1->second==max&&!count){
			cout<<tele1->first<<" "<<max;
			++count;			
		}

		else if(tele1->second==max)
			++count;
	}
	if(count>1)
		cout<<" "<<count;
	return 0;
}
