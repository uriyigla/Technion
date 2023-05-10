// 1-bit ALU template
module alu1bit (
    input logic a,           // Input bit a
    input logic b,           // Input bit b
    input logic cin,         // Carry in
    input logic [1:0] op,    // Operation
    output logic s,          // Output S
    output logic cout        // Carry out
);

// Put your code here
// ------------------

logic g1_out; 
logic g2_out;
logic g3_out;
logic g4_out;
logic g5_out;
logic fas_out;


fas fas_uut(.a(a), .b(b), .cin(cin), .a_ns(g1_out), .s(fas_out), .cout(cout));

mux4 mux4_inst(.d0(g5_out), .d1(g3_out), .d2(fas_out), .d3(fas_out), .sel(op), .z(s));

NAND2 #(.Tpdlh(1),.Tpdhl(1)) g5(.A(g4_out), .B(g4_out), .Z(g5_out));
NAND2 #(.Tpdlh(1),.Tpdhl(1)) g3(.A(g2_out), .B(g2_out), .Z(g3_out));
NAND2 #(.Tpdlh(1),.Tpdhl(1)) g1(.A(op[0]), .B(op[1]), .Z(g1_out));

XNOR2 #(.Tpdlh(4),.Tpdhl(4)) g2(.A(a), .B(b), .Z(g2_out));

OR2 #(.Tpdlh(6),.Tpdhl(6)) g4(.A(a), .B(b), .Z(g4_out));

// End of your code

endmodule
