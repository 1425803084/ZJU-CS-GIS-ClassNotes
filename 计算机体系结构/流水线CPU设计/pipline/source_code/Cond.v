`timescale 1ns / 1ps

//���ڷ�ָ֧��Ƚ�
module Cond(
    input [31:0] A,
    input [31:0] B,
    input cond,
    output res
    );

//cond = 0: beq, cond = 1: bne 
assign res = cond ^ (A == B);
endmodule
