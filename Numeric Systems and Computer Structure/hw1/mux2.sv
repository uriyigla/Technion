// 2->1 multiplexer template
module mux2 (
    input logic d0,          // Data input 0
    input logic d1,          // Data input 1
    input logic sel,         // Select input
    output logic z           // Output
);

// Put your code here
// ------------------

logic g1_out; 
logic g2_out; 
logic g3_out; 

NAND2#(.Tpdhl(1),.Tpdlh(1))
	g1(.A(d0),.B(g2_out),.Z(g1_out));

NAND2#(.Tpdhl(1),.Tpdlh(1))
	g2(.A(sel),.B(sel),.Z(g2_out));

NAND2#(.Tpdhl(1),.Tpdlh(1))
	g3(.A(sel),.B(d1),.Z(g3_out));

NAND2#(.Tpdhl(1),.Tpdlh(1))
	g4(.A(g3_out),.B(g1_out),.Z(z));


// End of your code

endmodule
