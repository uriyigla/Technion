// 64-bit ALU template
module alu64bit (
    input logic [63:0] a,    // Input bit a
    input logic [63:0] b,    // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic [63:0] s,   // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------
logic cout_1 [62:0];
alu1bit alu_1(.a(a[0]), .b(b[0]), .cin(cin), .op(op), .s(s[0]), .cout(cout_1[0]));
alu1bit alu_63(.a(a[63]), .b(b[63]), .cin(cout_1[62]), .op(op), .s(s[63]), .cout(cout));
genvar i;
generate
	for (i = 1; i < 63; i++)
	begin
	   alu1bit alu_current(.a(a[i]), .b(b[i]), .cin(cout_1[i - 1]), .op(op), .s(s[i]), .cout(cout_1[i]));
	end
endgenerate



// End of your code

endmodule
