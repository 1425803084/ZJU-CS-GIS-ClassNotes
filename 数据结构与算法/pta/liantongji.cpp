//7-6 �г���ͨ�� ��25 �֣�
/*����һ����N�������E���ߵ�����ͼ������DFS��BFS�ֱ�
�г������е���ͨ�������趥���0��N?1��š���������ʱ����
���������Ǵӱ����С�Ķ������������ŵ�����˳������ڽӵ㡣*/

#include<iostream>
#include<list>
using namespace std;

int edge[10][10]={0};
int v[10];
int visit[10]={0};
//dfs�ݹ�ʵ�� 
int dfs(int v1,int n){
	int count=1,i;
	cout<<v1<<" ";
	v[v1]=0;
	for(i=0;i<n;++i){
		if(edge[v1][i]&&v[i])
				count+=dfs(i,n);
	}
	return count;
}
//bfs����ʵ�� 
int bfs(int v1,int n){
	list<int> a;
	int count=1,i,x;
	cout<<v1<<" ";
	v[v1]=0;
	visit[v1]=0;
	for(i=0;i<n;++i)
		if(edge[v1][i])
				a.push_back(i),visit[i]=0;
	while(!a.empty()){
		x=*a.begin();
		a.pop_front();
		cout<<x<<" ";
		v[x]=0;
		++count;
		for(i=0;i<n;++i){
			if(edge[x][i]&&v[i]&&visit[i])
				a.push_back(i),visit[i]=0;
	}
	}
	return count;
}

int main(){
	
	int n,e,n1;
	int i,e1,e2;
	cin>>n>>e;
	for(i=0;i<e;++i){
		cin>>e1>>e2;
		edge[e1][e2]=1;
		edge[e2][e1]=1;
	}
	//dfs 
	for(i=0;i<n;i++)
		v[i]=1;
	n1=n;
	cout<<"{ ";
	n1=n1-dfs(0,n);
	cout<<"}"<<endl;
	while(n1>0){
		i=0;
		while(!v[i])
			++i;
		cout<<"{ ";
		n1-=dfs(i,n);
		cout<<"}"<<endl;
	}
	//bfs
	for(i=0;i<n;++i)
		visit[i]=1;
	for(i=0;i<n;i++)
		v[i]=1;
	n1=n;
	cout<<"{ ";
	n1=n1-bfs(0,n);
	cout<<"}"<<endl;
	while(n1>0){
		i=0;
		while(!v[i])
			++i;
		cout<<"{ ";
		n1-=bfs(i,n);
		cout<<"}"<<endl;
	}
	return 0;
}
