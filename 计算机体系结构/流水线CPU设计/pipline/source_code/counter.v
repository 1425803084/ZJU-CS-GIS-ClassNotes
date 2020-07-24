/////////////////////////////////////////////////////////////////////////////
//����Ӳ��ƽ̨�� Xilinx Spartan 6 FPGA
//�����׼��ͺţ� SF-SP6 ��Ȩ����
//��   Ȩ  ��   ���� �������ɡ�����ǳ����תFPGA�����ߡ���Ȩͬѧ��ԭ����
//				����SF-SP6�����׼�ѧϰʹ�ã�лл֧��
//�ٷ��Ա����̣� http://myfpga.taobao.com/
//�����������أ� �ٶ����� http://pan.baidu.com/s/1jGjAhEm
//��                ˾�� �Ϻ�������ӿƼ����޹�˾
/////////////////////////////////////////////////////////////////////////////
module counter(
			input clk,		//ʱ���źţ�25MHz
			input rst_n,	//��λ�źţ��͵�ƽ��Ч
			//output reg[15:0] display_num	//�������ʾ���ݣ�[15:12]--�����ǧλ��[11:8]--����ܰ�λ��[7:4]--�����ʮλ��[3:0]--����ܸ�λ
			output reg timer_1s_flag
		);

//-------------------------------------------------
//1s��ʱ�����߼�
reg[24:0] timer_cnt;	//1s��������0-24999999

	//1s��ʱ����
always @(posedge clk or negedge rst_n)
	if(!rst_n) timer_cnt <= 25'd0;
	else if(timer_cnt < 25'd24_999_999) begin
		timer_cnt <= timer_cnt+1'b1;
	end
	else timer_cnt <= 25'd0;



//assign timer_1s_flag = (timer_cnt == 25'd24_999_999);		//1s��ʱ����־λ������Чһ��ʱ������

//-------------------------------------------------
//�������ݲ����߼�

	//��ʾ����ÿ�����
always @(posedge clk or negedge rst_n)
	if(!rst_n) timer_1s_flag <= 1'b0;
	else if(timer_cnt < 25'd12_499_999) timer_1s_flag <= 1'b0;
	else	timer_1s_flag <= 1'b1;
	
endmodule

