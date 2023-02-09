// fibo-hazards.S file template, rename and implement the algorithm
// Test algorithm in qtmips_gui program
// Select the CPU core configuration to
//    Pipelined without hazard unit and cache
// This option select version where RAW dependencies which leads
// to hazards are not resolved in hardware, you need to schedule
// instruction execution according to the pipeline structure
// (classical 5-stage MIPS) such way, that no dependency results
// in a hazard


// copy directory with the project to your repository to
// the directory work/fibo-hazards
// critical is location of the file work/fibo-hazards/fibo-hazards.S
// which is checked by the scripts

// The script loads number of the last Fibonacci series element to compute
// into fibo_limit variable and expects computed series in memory starting
// at address fibo_series, the series has to be followed by at least
// one zero element

// When tested by actual qtmips_cli version, the variant without hazard
// unit cannot be selected (this is WIP for the test script), use qtmips_gui
// which is fully configurable

// Directives to make interesting windows visible
#pragma qtmips show registers
#pragma qtmips show memory

.set noreorder
.set noat

.globl    fibo_limit
.globl    fibo_series

.text
.globl _start
.ent _start

_start:
	//la   $a0, fibo_series
	la $a0, fibo_series
	nop
	addi $t3, $a0, 0 //nyni ve vhodny moment si do $t3 ulozim value z $a0
	nop
	nop
	add $a0, $a0, $t3 //nyni to opet prictu ve spravny moment z $t3 na registr $a0
	
	la $a1, fibo_limit
	nop
	addi $t3, $a1, 0 //nyni ve vhodny moment si do $t3 ulozim value z $a1
	nop
	nop
	add $a1, $a1, $t3 //nyni to opet prictu ve spravny moment z $t3 na registr $a1
	nop
	nop
	lw   $a1, 0($a1) // number of elements in the array

inicialization:
	sw $0, 0($4) //ulozim do pameti nulu
	lw $t0, 0($4) //do registru $t0 ulozim nulu z pameti
	addi $t1, $t0, 1 //do registru $t1 ulozim jednicku
	nop
	nop
	sw $t1, 4($4) //do pameti ulozim jednicku
	addi $t2, $0, 2 //do registru $t2 ulozim index pozice 2 (az jednicka je zacatek)
	addi $a0, $a0, 8 //k registru $a0 prictu 12, pozice kam ukladat dalsi cislo
	//nyni mohu zacit scitat fibonaciho

cycle:	//provedu dva cykly najednou, at efektivne pracuji s registry, vystaci mi jen dva
	add $t0, $t0, $t1
	nop
	nop
	sw $t0, 0($4)
	beq $t2,$a1, end_loop
	addi $t2, $t2, 1 //prictu index
	addi $a0, $a0, 4 //posunu ukazatel pameti

	add $t1, $t0, $t1
	nop
	nop
	sw $t1, 0($4)
	beq $t2,$a1, end_loop
	addi $t2, $t2, 1 //prictu index
	addi $a0, $a0, 4 //posunu ukazatel pameti
	j cycle
	

//Final infinite loop
end_loop:
	sw $0, 4($4)    // prida nulu na konec memory
	cache 9, 0($0)  // flush cache memory
	break           // stop the simulator
	j end_loop
	nop

.end _start

.data
// .align    2 // not supported by QtMips yet

fibo_limit:
	.word 15
fibo_series:
	.skip 1000*4

// Specify location to show in memory window
#pragma qtmips focus memory fibo_limit