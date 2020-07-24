/////////////////////////////////////////////////////////////////////////////
//����Ӳ��ƽ̨�� Xilinx Spartan 6 FPGA
//�����׼��ͺţ� SF-SP6 ��Ȩ����
//��   Ȩ  ��   ���� �������ɡ�����ǳ����תFPGA�����ߡ���Ȩͬѧ��ԭ����
//				����SF-SP6�����׼�ѧϰʹ�ã�лл֧��
//�ٷ��Ա����̣� http://myfpga.taobao.com/
//�����������أ� �ٶ����� http://pan.baidu.com/s/1jGjAhEm
//��                ˾�� �Ϻ�������ӿƼ����޹�˾
///////////////////////////////////////////////////////////////////////////////����һ��ÿ�������16bit������16���Ʒ�ʽ��ʾ��4λ�������
module sp6(
			input ext_clk_25m,	//�ⲿ����25MHzʱ���ź�
			input ext_rst_n,	//�ⲿ���븴λ�źţ��͵�ƽ��Ч
			input [3:0] switch,
			output[3:0] dtube_cs_n,	//7�������λѡ�ź�
			output[7:0] dtube_data,	//7������ܶ�ѡ�źţ�����С����Ϊ8�Σ�
			output[7:0] led
		);													

//-------------------------------------
//PLL����
wire clk_12m5;	//PLL���12.5MHzʱ��
wire clk_25m;	//PLL���25MHzʱ��
wire clk_50m;	//PLL���50MHzʱ��
wire clk_100m;	//PLL���100MHzʱ��
wire sys_rst_n;	//PLL�����locked�źţ���ΪFPGA�ڲ��ĸ�λ�źţ��͵�ƽ��λ���ߵ�ƽ��������

  pll_controller uut_pll_controller
   (// Clock in ports
    .CLK_IN1(ext_clk_25m),      // IN
    // Clock out ports
    .CLK_OUT1(clk_12m5),     // OUT
    .CLK_OUT2(clk_25m),     // OUT
    .CLK_OUT3(clk_50m),     // OUT
    .CLK_OUT4(clk_100m),     // OUT
    // Status and control signals
    .RESET(~ext_rst_n),// IN
    .LOCKED(sys_rst_n));      // OUT		
		
/*-----------------------------------------cpu--------------*/		

wire clk, rst;
assign clk = clk_1s;
//assign clk =  ext_clk_25m;

wire clk_1s;
wire en;
wire [31:0] data_in, inst_in, wt_mem_data;
wire [31:0] data_addr, PC_ID;
wire stall,dstall;

assign led[0] = ~clk_1s;
assign led[1] = ~rst;
assign led[2] = ~en;
assign led[3] = ~stall;
assign led[4] = ~dstall;
assign led[7:5] = 3'b111;

assign rst = ~ext_rst_n;

Pipeline_CPU pCPU(
    .clk(clk),
    .reset(rst),
    .data_in(data_in),
    .inst_in(inst_in),
    .PC_out(PC_ID),
    .mem_w(en),
    .data_out(wt_mem_data),
    .addr_out(data_addr),
		.stall(stall),
		.data_stall(dstall)
);

mem d_RAM(
    .addra(data_addr[11:2]),
    .dina(wt_mem_data),
	 .dinb(32'b0),
    .addrb(PC_ID[11:2]),
    .wea(en),
	 .web(1'b0),
    .clka(clk),
	 .clkb(clk),
    .douta(data_in),
    .doutb(inst_in)
);

//-------------------------------------
//25MHzʱ�ӽ��з�Ƶ������ÿ�������16λ����
wire[15:0] display_num;	//�������ʾ���ݣ�[15:12]--�����ǧλ��[11:8]--����ܰ�λ��[7:4]--�����ʮλ��[3:0]--����ܸ�λ

assign display_num = 
							(switch[0] ? 
										(switch[2]? 
											( switch[1]? data_in[31:16] : data_in[15:0] ):		// ����
											( switch[1]? inst_in[31:16] : inst_in[15:0] )		// ָ��
													):
										(switch[2]? 						
											( switch[1]? data_addr[31:16] : data_addr[15:0] ):	// ���ݵ�ַ
											( switch[1]? PC_ID[31:16] : PC_ID[15:0] )				// ָ���ַ
													)	
										);
													
counter		uut_counter(
				.clk(clk_25m),		//ʱ���ź�
				.rst_n(sys_rst_n),	//��λ�źţ��͵�ƽ��Ч
				.timer_1s_flag(clk_1s)		//LEDָʾ�ƽӿ�	
			);
		
//-------------------------------------
//4λ�������ʾ����															

seg7		uut_seg7(
				.clk(clk_25m),		//ʱ���ź�
				.rst_n(sys_rst_n),	//��λ�źţ��͵�ƽ��Ч
				.display_num(display_num),		//LEDָʾ�ƽӿ�	
				.dtube_cs_n(dtube_cs_n),	//7�������λѡ�ź�
				.dtube_data(dtube_data)		//7������ܶ�ѡ�źţ�����С����Ϊ8�Σ�
		);

endmodule

