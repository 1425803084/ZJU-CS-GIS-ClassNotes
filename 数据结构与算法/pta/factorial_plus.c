//6-10 �׳˼���������

#include <stdio.h>

void Print_Factorial ( const int N );

int main()
{
    int N;
    scanf("%d", &N);
    Print_Factorial(N);
    return 0;
}

void Print_Factorial ( const int N ){
	
	if(N<0){
		printf("Invalid input");
		return;
	} 
	if(N==0){
		printf("1");
		return;
	} 
	
	char a[1000000]={1},c[1000000];//aΪ���ս����cΪ�м䱣����һ�ε�ֵ�������˷���a=c*i 
	char p=0;//��λ 
	int count=1,i,j,k,b,d;//number to plus��iΪ�׳�ֵ��countΪλ����jΪ������d����j��λ����bΪ�˷����е���һλ 
	for(i=1;i<=N;i++){//factorial number to plus
		
		for(j=0;j<count;j++){
				c[j]=a[j];
				a[j]=0;		
		}//set up c=a
		
		d=0;
		for(j=i;j;j/=10){
			k=j%10;//each digit
			for(b=0;b<count+1;b++){
				a[b+d]+=c[b]*k+p;
				p=0;
				if(a[b+d]>9){
					p=a[b+d]/10;
					a[b+d]=a[b+d]%10;
				}//�����λ 
			}
			if(p)	a[b+d]=p;
			d++;
		}
		 
		for(j=count+4;j>=0;j--)
			if(a[j]!=0) break;
		count=j+1;//ȷ��count���ⲽӦ������취��� 
	}
	
	for(i=count-1;i>=0;i--)
		printf("%d",a[i]);
		
}
