//7-7 ���ȿռ� ��30 �֣�
/*�����ȿռ䡱�����ֳ��������ȷָ���
Six Degrees of Separation�������ۡ�
������ۿ���ͨ�׵ز���Ϊ��������κ�
һ��İ����֮����������˲��ᳬ��������Ҳ����˵�����ͨ����
��������ܹ���ʶ�κ�һ��İ���ˡ�����ͼ1��ʾ��
�������һ���罻����ͼ�������ÿ���ڵ�������
�����ȿռ䡱���۵Ľ��ռ��������İٷֱȡ�*/

#include<stdio.h>

//ʹ�õ�Դ��Ȩ���·�㷨����������·�����ȣ�
//�����ֵ�һ���ڵ�·��Ϊ7ʱ�˳�ѭ�������㾭���Ľڵ���� 
int a[10001][10001] = { 0 };// 
int v[10001] = { 0 };
//visit����ڽӾ��� 
struct queue {
	int a[10001];
	int first, last;
	int sum;
}q;

void push(int x) {
	if (q.first == 999)
		q.first = 0;
	else q.first++;
	q.a[q.first] = x;
	q.sum++;
}

int pop() {
	int t = q.a[q.last];
	if (q.last == 999)
		q.last = 0;
	else q.last++;
	q.sum--;
	return t;
}
//ѭ������ 
int main() {
	int n, m;
	int i, j;
	scanf("%d %d", &n, &m);
	int v1, v2;
	for (i = 0; i < m; ++i) {
		scanf("%d%d", &v1, &v2);
		a[v1][v2] = 1;
		a[v2][v1] = 1;
	}
	int len, now, num;//��ǰ����ֵ����Ϊ���ʱ�ǣ� 

	for (i = 1; i <= n; ++i){
		//���г�ʼ��	
		q.sum = 0;
		q.first = 999;
		q.last =0;
		len = 0;
		num = 1;
		push(i);
		for (j = 1; j <= n; ++j)
			v[j] = -1;
		v[i]=0;
		while (1) {
			if (q.sum==0) {
				break;
			}
			now = pop();
			for (j = 1; j <= n; ++j) {
				if (a[now][j] && v[j] == -1) {
					push(j);
					v[j] = v[now]+1;
					if(v[j]==7) break;
					++num;
				}
			}
			if(v[j]==7) break;
		}
		double per = 1.0*num / n * 100;
		printf("%d: %.2f%%\n", i, per);
	}
	return 0;
}
