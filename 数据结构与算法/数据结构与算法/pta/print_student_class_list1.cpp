//7-49 ��ӡѧ��ѡ���嵥 ��25 �֣�
#include<bits/stdc++.h>//����ͷ�ļ�get 
using namespace std;
//�ο��� https://blog.csdn.net/Dream_Weave/article/details/81185805
//��ʹ�����ȶ��ж���������sort���� 
map<string,vector<int> > mp;
//������cin... 
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    int k,id;
    char name[7];
    for(int i=0;i<m;i++){
        scanf("%d%d",&id,&k);
        for(int j=0;j<k;j++){
            scanf("%s",name);
            mp[name].push_back(id);
        }
    }
    vector<int> tpq;
    for(int i=0;i<n;i++){
            scanf("%s",name);
            tpq=mp[name];
            printf("%s",name);
            printf(" %d",tpq.size());
            sort(tpq.begin(),tpq.end());
        vector<int>::iterator tpq1=tpq.begin();
        while(tpq1!=tpq.end()){   
            printf(" %d",*tpq1);
            ++tpq1;
        }
        printf("\n");
    }
    return 0;
}
