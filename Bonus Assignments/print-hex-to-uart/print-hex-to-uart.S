// print-hex-to-uart.S file template, rename and implement the algorithm
// The task is to process random value value injected into input_val
// unsigned integer variable and print corresponding hexadecimal value
// terminated by line feed (LF, '\n', 0x0a) character to the serial
// port (UART).

// Test algorithm in qtmips_gui program.
// Every CPU configuration conforming MIPS architecture can be chosen.
// Test script select pipelined configuration with hazard unit but without
// cache.

// Copy directory with the project to your repository to
// the directory work/print-hex-to-uart
// critical is location of the file work/print-hex-to-uart/print-hex-to-uart.S
// which is checked by the scripts

// The script loads number to print into input_val global variable
// and the implemented algorithm converts it to series of 8 hexadecimal
// digits finalized by LF ('\n', 0x0a) character. Lowercase digits a to f
// are expected for nibbles within range 10 to 15.

// When tested by actual qtmips_cli version, capture of serial output
// to the file is not implemented yet.

// Directives to make interesting windows visible
#pragma qtmips show registers
#pragma qtmips show memory


// Serial port/terminal registers
// There is mirror of this region at address 0xffff0000
// to match QtSpim and Mars emulators

.equ SERIAL_PORT_BASE,      0xffffc000 // base address of serial port region

.equ SERP_RX_ST_REG,        0xffffc000 // Receiver status register
.equ SERP_RX_ST_REG_o,          0x0000 // Offset of RX_ST_REG
.equ SERP_RX_ST_REG_READY_m,       0x1 // Data byte is ready to be read
.equ SERP_RX_ST_REG_IE_m,          0x2 // Enable Rx ready interrupt

.equ SERP_RX_DATA_REG,      0xffffc004 // Received data byte in 8 LSB bits
.equ SERP_RX_DATA_REG_o,        0x0004 // Offset of RX_DATA_REG

.equ SERP_TX_ST_REG,        0xffffc008 // Transmitter status register
.equ SERP_TX_ST_REG_o,          0x0008 // Offset of TX_ST_REG
.equ SERP_TX_ST_REG_READY_m,       0x1 // Transmitter can accept next byte
.equ SERP_TX_ST_REG_IE_m,          0x2 // Enable Tx ready interrupt

.equ SERP_TX_DATA_REG,      0xffffc00c // Write word to send 8 LSB bits to terminal
.equ SERP_TX_DATA_REG_o,        0x000c // Offset of TX_DATA_REG

.set noreorder
.set noat

.globl    input_val

.text
.globl _start
.ent _start

// REGISTRY
// a0 - base adresa serioveho portu
// a1 - input_val, to, co vypisuji
// t1 - pomocna pro zvoleni pozadovane ctverice bajtu pomoci and
// t2 - promenna o kolik shiftuju v aktualnim cyklu
// t3 - pouze pomocna promenna pro odecitani (nelze odecitat immediate value)
// t4 - true nebo false, zjistuje zda je to cislo nebo pismeno
// v0 - to co budu tisknotu na terminal v danem cyklu

_start:	la  	$a1, input_val
	lw	$a1, 0($a1)
	la	$a0, SERIAL_PORT_BASE
	addi	$t2, $0, 32
	addi	$t3, $0, 4	

loop:	sub	$t2, $t2, $t3
	addi	$t1, $0, 0xf
	sllv	$t1, $t1, $t2 
	and	$v0, $t1, $a1
	srlv	$v0, $v0, $t2
	slti	$t4, $v0, 10
	
	beq	$t4, $0, char
	nop

number:	addi 	$v0, $v0, 48
	j print
	nop

char:	addi 	$v0, $v0, 87

print:	lw	$t0, SERP_TX_ST_REG_o($a0) // read status of transmitter
	andi	$t0, $t0, SERP_TX_ST_REG_READY_m // mask ready bit
	beq	$t0, $zero, print	// if not ready wait for ready condition
	nop
	sw	$v0,SERP_TX_DATA_REG_o($a0) // write byte to Tx data register
	bne	$t2, $0, loop
	nop

end_loop:	
	addi 	$t6, $0, 0x0a
	sw	$t6,SERP_TX_DATA_REG_o($a0)
	cache 9, 0($0)  // flush cache memory
	break           // stop the simulator
	j end_loop
	nop

.end _start

.data
// .align    2 // not supported by QtMips yet

input_val:
	.word 0x12345678

// Specify location to show in memory window
#pragma qtmips focus memory input_val