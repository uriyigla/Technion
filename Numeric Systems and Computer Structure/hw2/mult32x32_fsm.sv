// 32X32 Multiplier FSM
module mult32x32_fsm (
    output logic valid,           	// Product valid indication
    output logic  [1:0] a_sel,    	// Select one byte from A
    output logic        b_sel,    	// Select one 2-byte word from B
    output logic  [5:0] shift_a_val,// Shift value of 8-bit mult product
	output logic  [5:0] shift_b_val,// Shift value of 8-bit mult product
    output logic upd_prod,        	// Update the product register
    output logic clr_prod,        	// Clear the product register

    input logic start,            	// Start signal
    input logic clk,              	// Clock
    input logic rst               	// Reset
);

// Put your code here
// ------------------
                
// End of your code

endmodule

