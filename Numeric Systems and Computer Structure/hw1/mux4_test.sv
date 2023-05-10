// 4->1 multiplexer test bench template
module mux4_test;

// Put your code here
// ------------------
logic d0, d1, d2, d3, z;
logic [1:0] sel;
mux4 test(.d0(d0),.d1(d1),.d2(d2),.d3(d3),.sel(sel),.z(z));
initial begin

	d0 = 1'b1;
	
	d1 = 1'b0;
	
	d2 = 1'b0;
	
	d3 = 1'b0;
	
	sel = 2'b00;

	#60
	
	sel=2'b01;

	#60

	sel = 2'b00;
	
	#60;
	
end
// End of your code

endmodule
