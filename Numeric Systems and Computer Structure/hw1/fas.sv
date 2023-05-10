// Full Adder/Subtractor template
module fas (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic a_ns,        // A_nS (add/not subtract) control
    output logic s,          // Output S
    output logic cout        // Carry out
);

// Put your code here
	logic g0_out, g1_out, g2_out, g3_out, g5_out;
	
	NAND2 #(.Tpdhl(1),.Tpdlh(1)) g2_NAND (.A(b), .B(cin), .Z(g2_out));
	NAND2 #(.Tpdhl(1),.Tpdlh(1)) g3_NAND (.A(g0_out), .B(g1_out), .Z(g3_out));
	NAND2 #(.Tpdhl(1),.Tpdlh(1)) g4_NAND (.A(g2_out), .B(g3_out), .Z(cout));

	XNOR2 #(.Tpdhl(4),.Tpdlh(4)) g0_XNOR (.A(a_ns), .B(a), .Z(g0_out));
	XNOR2 #(.Tpdhl(4),.Tpdlh(4)) g5_XNOR (.A(a), .B(b), .Z(g5_out));
	XNOR2 #(.Tpdhl(4),.Tpdlh(4)) g6_XNOR (.A(g5_out), .B(cin), .Z(s));
	
	OR2 #(.Tpdhl(6),.Tpdlh(6)) g1_OR (.A(b), .B(cin), .Z(g1_out));
	
// End of your code

endmodule
