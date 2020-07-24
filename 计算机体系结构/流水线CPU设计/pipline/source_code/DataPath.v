`timescale 1ns / 1ps
module Pipeline_CPU(
	input clk,
	input reset,
    input [31:0] data_in,   //��Mem���������, ֵΪMem[addr_out]
    input [31:0] inst_in,   //��Mem�����ָ��, ֵΪMem[PC_out]
    output [31:0] PC_out,   //ID��PC
    output mem_w,           //�����дʹ��
    output [31:0] data_out, //д����ʱ������
    output [31:0] addr_out,  //д�����ַ
	 output data_stall,
	 output stall
);

//wire stall, data_stall;
//---------------------IF--------------------
wire [1:0] BranchFlag_ID;
reg [1:0] BranchFlag_EXE;
//PC_out -> memory -> inst_in
reg [31:0] PC_in;
REG32 regPC (
    .clk(clk),
    .rst(reset),
    .CE(~stall),
    .D(PC_in),
    .Q(PC_out) 
    );
//TF��ַ��EXE��ȷ��, ���ֻ��Ҫ���ID���Ƿ�����ת, ����control_stallʱIF��ID����nop, EXEΪ��֧��תʱID��Ϊnop
assign stall = data_stall | |(BranchFlag_ID);

//-------------------IF/ID---------------------------
reg [31:0] inst_ID, PC_ID;
always @(posedge clk or posedge reset) begin
    if(reset == 1) begin
        inst_ID <= 32'b0;
        PC_ID <= 32'b0;
    end
    else if(~data_stall) begin
        inst_ID <= inst_in & {32{~|(BranchFlag_EXE | BranchFlag_ID)}}; //���ID/EXE�з�ָ֧��, ��ID����nop
        PC_ID <= PC_out + 32'h4;
    end
end

//---------------------ID--------------------------
reg [31:0] wt_data_WB;
reg [4:0] wt_addr_WB, wt_addr_MEM, wt_addr_EXE;
reg RegWrite_WB, RegWrite_EXE, RegWrite_MEM;
//�������źŴ��䵽������
wire [17:0] ctrl_signal;
Controller ctrl(
    .inst_in(inst_ID),
    .ctrl_signal(ctrl_signal)
);

//�����ź��б�
 //EXE_forward��ʾ����ܷ���EXE�λش�
wire [1:0] MemtoReg_ID, rd_addr_valid, RegDst_ID;
wire SignExtend, cond_ID, ALUsrc_B_ID, mem_w_ID, RegWrite_ID, EXE_forward_ID;
wire [3:0] ALU_op_ID;
assign {BranchFlag_ID, MemtoReg_ID, mem_w_ID, cond_ID, ALU_op_ID, RegWrite_ID, rd_addr_valid, EXE_forward_ID, SignExtend, RegDst_ID, ALUsrc_B_ID} = ctrl_signal;

//ȷ��δ����WB����Ҫд��ĵ�ַ
reg [4:0] wt_addr_ID;
reg [4:0] shamt_ID;
wire [4:0] rs, rt, rd;
assign rs = inst_ID[25:21];
assign rt = inst_ID[20:16];
assign rd = inst_ID[15:11];

always@(*)begin
	case(RegDst_ID)
		2'b00:wt_addr_ID <= rt; //rt
		2'b01:wt_addr_ID <= rd; //rd
		2'b10:wt_addr_ID <= 5'b11111;    //$ra = 5'b11111, jalʹ��
		2'b11:wt_addr_ID <= 5'b0;        //δ����
	endcase
	//shift amount
	shamt_ID <= inst_ID[10:6];
end

//��չ������
wire [31:0] Imm32_ID;
assign Imm32_ID = SignExtend ? {{16{inst_ID[15]}}, inst_ID[15:0]} : {{16{1'b0}}, inst_ID[15:0]};

//�Ĵ�����
wire [31:0] Reg_A, Reg_B;
regs Regs (
    .clk(clk),
    .rst(reset),
    .L_S(RegWrite_WB),
    .R_addr_A(rs),
    .R_addr_B(rt),
    .Wt_addr(wt_addr_WB),
    .Wt_data(wt_data_WB),
    .rdata_A(Reg_A),
    .rdata_B(Reg_B)
    );

//RAW
//����д��ַ��Чʱ, ������ַ��EXE/MEM��ָ���д��ַһ�»ᷢ��data hazard
//���,��RAW��, ����Stall������: ����ַ��Ч, ����EXE��д��ַһ��, ���Ҳ�����EXE�λش�
reg EXE_forward;
reg [31:0] data_forward_EXE, data_forward_MEM;
wire [1:0] data_hazard; //valid={valid_rs, valid_rt}, data_hazard = {data_hazard_EXE, data_hazard_MEM}
assign data_hazard = {RegWrite_EXE & ((rd_addr_valid[1] & (wt_addr_EXE == rs)) | (rd_addr_valid[0] & (wt_addr_EXE == rt))), 
                           RegWrite_MEM & ((rd_addr_valid[1] & (wt_addr_MEM == rs)) | (rd_addr_valid[0] & (wt_addr_MEM == rt)))};
assign data_stall = data_hazard[1] & ~EXE_forward;

reg [31:0] Reg_A_ID, Reg_B_ID;
//����forwarding������:
//��д��ַ��Ч, ����EXE��д��ַһ��, ���Իش�
//��д��ַ��Ч, ����MEM��д��ַһ��
always@* begin
    case(data_hazard)
        2'b11: begin 
              //��EXE_forwardΪ0ʱ, �����ֵδ����
              if(wt_addr_EXE == wt_addr_MEM) begin
                    //EXE MEM��forwardingһ��, �ش�EXE������, ��һ������Դһ����regs
                    if (rs == wt_addr_EXE) begin
                        Reg_A_ID <= data_forward_EXE;
                        Reg_B_ID <= Reg_B;
                    end else begin
                        Reg_A_ID <= Reg_A;
                        Reg_B_ID <= data_forward_EXE;
                    end
              end else begin
                    //��һ��, ID��ָ�����������Դ����Ҫforwarding
                    if (rs == wt_addr_EXE) begin
                        Reg_A_ID <= data_forward_EXE;
                        Reg_B_ID <= data_forward_MEM;
                    end else begin
                        Reg_A_ID <= data_forward_MEM;
                        Reg_B_ID <= data_forward_EXE;
                    end
              
              end
              
        end
        2'b10: begin 
              //��EXE_forwardΪ0ʱ, �����ֵδ����
              if (rs == wt_addr_EXE) begin
                    Reg_A_ID <= data_forward_EXE;
                    Reg_B_ID <= Reg_B;
              end else begin
                    Reg_A_ID <= Reg_A;
                    Reg_B_ID <= data_forward_EXE;
              end
        end
        2'b01: begin 
              if (rs  == wt_addr_MEM) begin
                    Reg_A_ID <= data_forward_MEM;
                    Reg_B_ID <= Reg_B;
              end else begin
                    Reg_A_ID <= Reg_A;
                    Reg_B_ID <= data_forward_MEM;
              end
        end
        2'b00: begin 
              Reg_A_ID <= Reg_A;
              Reg_B_ID <= Reg_B;
        end
    endcase
end


//-------------------ID/EXE--------------------
//TODO: ȷ������������EXE�εĿ����ź�
reg [31:0] Reg_A_EXE, Reg_B_EXE, Imm32_EXE, inst_EXE;
reg [3:0] ALU_op_EXE;
reg [31:0] PC_EXE;
reg [1:0] MemtoReg_EXE;
reg [4:0] shamt_EXE;
reg mem_w_EXE;
reg ALUsrc_B_EXE;
reg cond_EXE;
always@(posedge clk) begin
    Reg_A_EXE <= Reg_A_ID;
    Reg_B_EXE <= Reg_B_ID;
    Imm32_EXE <= Imm32_ID;
    PC_EXE <= PC_ID;
    wt_addr_EXE <= wt_addr_ID;
    inst_EXE <= inst_ID;
    shamt_EXE <= shamt_ID;
    //������stall��, ID����EXE�δ���Nop
    //control_stallʱ, IF��ID����nop, ��˲���Ҫ���κ��޸�
    //��עdata_stall����
    ALU_op_EXE <= ALU_op_ID;
    RegWrite_EXE <= RegWrite_ID & ~data_stall;
    MemtoReg_EXE <= MemtoReg_ID;
    BranchFlag_EXE <= BranchFlag_ID;
    mem_w_EXE <= mem_w_ID & ~data_stall;
    EXE_forward <= EXE_forward_ID;
    ALUsrc_B_EXE <= ALUsrc_B_ID; 
    cond_EXE <= cond_ID;
end
//--------------------EXE--------------------
//ALU
wire [31:0] ALUout_EXE;
ALU alu (
    .A(Reg_A_EXE),
    .B(ALUsrc_B_EXE?Imm32_EXE:Reg_B_EXE), //ALUsrc_B = 0, use reg_B(R-Type, e.g.); ALUsrc_B = 1, use Imm
    .shamt(shamt_EXE),
    .ALU_operation(ALU_op_EXE),
    .res(ALUout_EXE)
    );
 
//��֧/��ת��ַ����
//jϵ��ָ����ID�μ������EXE��
wire [31:0] branch_PC_EXE, jr_PC_EXE;
wire b_flag;
assign jr_PC_EXE = Reg_A_EXE; //jr ϵ��ָ���Mem[rs]��ת
Cond c(
    .A(Reg_A_EXE),
    .B(Reg_B_EXE),
    .cond(cond_EXE),
    .res(b_flag)
);
assign branch_PC_EXE = PC_EXE + (b_flag?{Imm32_EXE[29:0], 2'b0}:32'h0); //������������ת, ����PC+4
always@(*) begin
    case(BranchFlag_EXE)
        2'b00: begin  PC_in <= PC_out + 32'h4; end                              //�޷�֧, ����ʱ��
        2'b01: begin  PC_in <= branch_PC_EXE;  end                              //��ָ֧��
        2'b10: begin  PC_in <= {PC_EXE[31:28], inst_EXE[25:0], 2'b0};       end //j
        2'b11: begin  PC_in <= Reg_A_EXE;      end                              //jr
    endcase
end


//-------------------------EXE/MEM---------------------------
reg mem_w_MEM;
reg [31:0] data_out_MEM, ALUout_MEM, Imm32_MEM, PC_MEM, inst_MEM;
reg [1:0] MemtoReg_MEM;
always@(posedge clk) begin
    ALUout_MEM <= ALUout_EXE;
    inst_MEM <= inst_EXE;
    data_out_MEM <= Reg_B_EXE; //���ҽ���SWָ��,����Դ��rt, �����RegB
    Imm32_MEM <= Imm32_EXE;
    PC_MEM <= PC_EXE;
    wt_addr_MEM <= wt_addr_EXE;
    RegWrite_MEM <= RegWrite_EXE;
    MemtoReg_MEM <= MemtoReg_EXE;
    mem_w_MEM <= mem_w_EXE;
end

always@(*)begin
	case(MemtoReg_EXE)
		2'b00: data_forward_EXE <= 32'h0;                    //undefined
		2'b01: data_forward_EXE <= ALUout_EXE;               //ALU��ָ��, Ҳ��EXE��forwarding
		2'b10: data_forward_EXE <= {Imm32_EXE[15:0], 16'b0};  //Lui, ��EXE����forwarding
		2'b11: data_forward_EXE <= PC_EXE;                    //����, ��EXE��forwarding
	endcase
end

//--------------------------MEM-------------------------------
//��������: д���
assign mem_w = mem_w_MEM;
assign data_out = data_out_MEM;
assign addr_out = ALUout_MEM; //д���ַ��ALU����RegA + Imm32(with sign extend)

//--------------------------MEM/WB----------------------------
reg [31:0] Imm32_WB, ALUout_WB, data_in_WB, PC_WB, inst_WB;
reg [1:0] MemtoReg_WB;
always@(posedge clk) begin
    Imm32_WB <= Imm32_MEM;
    ALUout_WB <= ALUout_MEM;
    data_in_WB <= data_in; //��������: �����
    PC_WB <= PC_MEM;
    inst_WB <= inst_MEM;
    
    wt_addr_WB <= wt_addr_MEM;
    RegWrite_WB <= RegWrite_MEM;
    MemtoReg_WB <= MemtoReg_MEM;
end

always@(negedge clk)begin
	case(MemtoReg_MEM)
		2'b00: data_forward_MEM <= data_in;                    
		2'b01: data_forward_MEM <= ALUout_MEM;             
		2'b10: data_forward_MEM <= {Imm32_MEM[15:0], 16'b0};
		2'b11: data_forward_MEM <= PC_MEM;                  
	endcase
end

//--------------------------WB--------------------------------
// ȷ��д��Ĵ����������
always@(*)begin
	case(MemtoReg_WB)
	    2'b00:wt_data_WB <= data_in_WB;               //LW
		2'b01:wt_data_WB <= ALUout_WB;                //ALU��ָ��
		2'b10:wt_data_WB <= {Imm32_WB[15:0], 16'b0};  //Lui
		2'b11:wt_data_WB <= PC_WB;                    //����
	endcase
end
endmodule
