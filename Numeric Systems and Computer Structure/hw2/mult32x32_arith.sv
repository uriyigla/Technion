// 32X32 Multiplier arithmetic unit template
module mult32x32_arith (
    output logic [63:0] product, 	// Miltiplication product

    // Multiplier controls
    input logic [31:0] a,        	// Input a
    input logic [31:0] b,        	// Input b
    input logic [1:0] a_sel,     	// Select one byte from A
    input logic       b_sel,     	// Select one 2-byte word from B
    input logic [5:0] shift_a_val, 	// Shift value of 8-bit mult product
    input logic [5:0] shift_b_val, 	// Shift value of 8-bit mult product
    input logic upd_prod,        	// Update the product register
    input logic clr_prod,        	// Clear the product register
    input logic clk,             	// Clock
    input logic rst              	// Reset
);

// Put your code here
// ------------------


// End of your code

endmodule
