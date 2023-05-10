// 64-bit ALU test bench template
module alu64bit_test;

// Put your code here
// ------------------

     logic [63:0] a;  // Input bit a
     logic [63:0] b;    // Input bit b
	 logic [63:0] s;   // Output S
     logic [1:0] op;    // Operaion
     logic cin,cout;   // Carry in/Carry out

	alu64bit alu_test( 
		.a(a),
		.b(b),
		.cin(cin),
		.op(op),
		.s(s),
		.cout(cout)
	);

initial begin

	cin = 1'b0;
	a = {64{1'b1}};
	b = {64{1'b0}};
	op[0] = 1'b0;
	op[1] = 1'b1;
		
	#1500
	
	cin = 1'b1;
	a = {64{1'b1}};
	b = {64{1'b0}};
	op[0] = 1'b0;
	op[1] = 1'b1;

    #1500
	
	cin = 1'b0;
	a = {64{1'b1}};
	b = {64{1'b0}};
	op[0] = 1'b0;
	op[1] = 1'b1;
	
	#1500;
	
end
// End of your code

endmodule
