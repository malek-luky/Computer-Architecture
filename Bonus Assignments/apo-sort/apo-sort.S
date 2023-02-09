// apo-sort.S file template, rename and implement the algorithm
// Test algorithm in qtmips_gui program
// Select the CPU core configuration with delay-slot
// This setups requires (for simplicity) one NOP instruction after
// each branch and jump instruction (more during lecture about pipelining)
// The code will be compiled and tested by external mips-elf-gcc
// compiler by teachers, you can try make in addition, but testing
// by internal assembler should be enough

// copy directory with the project to your repository to
// the directory work/apo-sort
// critical is location of the file work/apo-sort/apo-sort.S
// and cache parameters work/apo-sort/d-cache.par
// which is checked by the scripts

// The file d-cache.par specifies D cache parameters in the form
//   <policy>,<#sets>,<#words in block>,<#ways>,<write method>
// The example is
//   lru,1,1,1,wb
// The cache size is limited to 16 words maximum.

// Directives to make interesting windows visible
#pragma qtmips show registers
#pragma qtmips show memory

.set noreorder
.set noat

.globl    array_size
.globl    array_start

.text
.globl _start
.ent _start

//REGISTRY
// $1 vicemene zarazka, ktera urcuje konec cyklu, na zacatku v optimalization tam nactu koncovou adresu pole, a od ni pak odecitam 4 kazdy cyklus
// $2 adresa zacatku pole
// $3 prvni cislo
// $4 druhe cislo
// $5 obahuje nulu nebo jedna - provnavani registru
// $6 pocet prvku v poli, nasledne prepsan na koncovy registr, kde se zastavi jeden cyklus

_start:	la   $2, array_start //nactu adresu zacatku pole
	la   $6, array_size //nactu adresu kde je ulozen max pocet prvku
	lw   $6, 0($6) //nactu hodnotu ulozenou na predchozi adrese v registru $6
	addi $1, $2, -4 //nakopieuji adresu registru zacatku rady do noveho, musim odecist -4, protoze neresim posledni, ale predposledni pozici

optimalization: 	//zde vynasobim pocet prvku ctyrikrat a prictu to k $2, nebudu to muset nasledne delat a v kazdem cyklu usetrim jeden prikaz
		add $6, $6, $6 //pocet prvku *2
		add $6, $6, $6 //pocet prvku *4
		add $1, $1, $6 //nahrazuje mi to nop, toto chci vykonat - posunu se o cislo dal - zde to vyziji pro oba cykly zaroven	
		//optimalizace, kdyz switchujeme, staci nacist jen jedno, ovsem potrebuji to nacist hned na zacatku, protoze to dale nacitam az na konci cyklu
		lw $3, 0($2) //nactu prvni cislo
		lw $4, 4($2) //nactu druhe slovo
		
		
main_cycle:	slt $5,$3,$4 //kdyz $3<$4, do $5 to ulozi jednicku, jinak nulu
		beq $5, $0, switch //kdyz $3<$4, switch, jinak jedu dal
		addi $2, $2, 4 //nahrazuje mi to nop, toto chci vykonat - posunu se o cislo dal - zde to vyziji pro oba cykly zaroven	
		lw $3, 0($2) //nejdu do switch, musim nacist obe slova, zde nactu prvni cislo
		lw $4, 4($2) //nactu druhe slovo	
		j main_cycle //skocim zpet na zacatek	
		beq $2, $1, new_cycle //jakmile si jsou registry $2 a $1 rovny, dosli jsme na konec rady	

switch:		sw $3, 0($2) //prepise v pameti mensi na vetsi
	 	sw $4, -4($2) //prepise v pameti vetsi na mensi
		lw $4, 4($2) //nactu druhe slovo
		j main_cycle //skocim zpet na zacatek
		beq $2, $1, new_cycle //jakmile si jsou registry $2 a $1 rovny, dosli jsme na konec rady	

new_cycle:	addi $1,$1, -4 //nakonci kazdeho cyklu odectu od posledniho prvku 4, vim, ze posledni prvek nemusim resit
		la $2, array_start //resetuju hodnotu zacatku pole
		beq $2, $1, end_loop//kdyz je registr $2 roven $1, je setrideno
		//nope neni nutny, stejne uz skaceme pryc, je nam jedno kdyz se prepise neco v registru
		lw $3, 0($2) //nactu prvni cislo
		lw $4, 4($2) //nactu druhe slovo
		j main_cycle //skocim na zacatek pole	

//Final infinite loop
end_loop:
	cache 9, 0($0)  // flush cache memory
	break           // stop the simulator
	j end_loop
	nop

.end _start

.data
// .align    2 // not supported by QtMips yet

array_size:
.word	6
array_start:
.word	6,5,4,3,2,1//,9, 2, 10, 6, 11, 1, 6, 9, 12, 12,15,19,15,19,9,2,5,94,9,5,5,9,4,9,5,9,4,2,9,54,12,9,45,19,49,6,4,94,61,6,98,9,6,66,59,16,65,65,65,65,9,49,54,945,9,19,19,4,94,4,4,65,46,4,4,6,56,56,5,5,56,5,65,5 

// Specify location to show in memory window
#pragma qtmips focus memory array_size