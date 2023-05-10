// Full Adder/Subtractor test bench template
module fas_test;

// Put your code here

	logic a_inner;
	logic b_inner;
	logic cin_inner;
	logic a_ns_inner;
	logic s_inner;
	logic cout_inner;
	
	fas test (.a(a_inner), .b(b_inner), .cin(cin_inner), .a_ns(a_ns_inner), .s(s_inner), .cout(cout_inner));
	
	initial begin
	
		a_inner = 1'b0;
		b_inner = 1'b0;
		cin_inner = 1'b1;
		a_ns_inner = 1'b0;
		
		#60
		
		a_inner = 1'b1;
		
		#60
		
		a_inner = 1'b0;
		
		#60;
		
	end
// End of your code

endmodule
