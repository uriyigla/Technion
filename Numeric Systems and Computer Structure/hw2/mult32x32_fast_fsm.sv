// 32X32 Multiplier FSM
module mult32x32_fast_fsm (
    output logic valid,             // Operation valid indication
    output logic  [1:0] a_sel,      // Select one byte from A
    output logic        b_sel,      // Select one 2-byte word from B
    output logic  [5:0] shift_a_val,// Shift value of 8-bit mult product
    output logic  [5:0] shift_b_val,// Shift value of 8-bit mult product
    output logic upd_prod,          // Update the product register
    output logic clr_prod,          // Clear the product register

    input logic start,              // Start signal
    input logic a_msb_is_0,   		// Indicates the MSB byte of a is zero
    input logic clk,                // Clock
    input logic rst                 // Reset
);

// Put your code here
// ------------------

                
// End of your code

endmodule

