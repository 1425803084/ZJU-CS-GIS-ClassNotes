/*����ĳ����A��1��A��9���Լ��Ǹ�����N��0��N��100000����
������֮��S=A+AA+AAA+?+AA?A��N��A��������A=1, N=3ʱ��
S=1+11+111=123��*/ 

#include <stdio.h>
#define MAX 100002

int main(){	
int i, j, A, N, flg, temp, carry; 
char output[MAX] = { 0 };
	scanf("%d%d", &A, &N);	flg = 0;
		
	if (N == 0){
		printf("0\n");
		return 0;			
	}

	else if (N == 1){
		printf("%d\n", A);
		return 0;
	}		
	
	carry = 0;//��λ�� 
	for (i = 0; i < N; i++){		
		output[i] = (A * (N - i) + carry) % 10;	
		carry = (A * (N - i) + carry) / 10;
		}//ÿһλ���� 
		
		while (carry){	
			output[i++] = carry % 10;	
			carry = carry / 10;		
			flg = 1;		
			}//���λ��λ 
			
			if (flg == 1){	
				for (i = N; i > 0; i--)		
					printf("%d", output[i]);	
					printf("%d\n", output[i]);		
					}	
			else{		
				for (i = N - 1; i > 0; i--)		
					printf("%d", output[i]);	
					printf("%d\n", output[i]);		
					}

 	return 0;
}

