#include <stdio.h>

#include "input\get_database.h"
#include "input\input.h"
#include "assembly\assembly.h"
#include "proc\proc.h"
#include "disassembly\disassembly.h"

void asm_to_dig(char** argv, ErrList *const list);
void proc_dig_code(ErrList *const list);
void disasm(ErrList *const list);

int main(int argc, char** argv)
{
	ErrList list = {};
	error_list_ctor(&list);
	MAIN

	asm_to_dig(argv, &list);
	MAIN

	proc_dig_code(&list);
	MAIN

	disasm(&list);
	MAIN

	error_list_dtor(&list);

	return 0;
}

void asm_to_dig(char** argv, ErrList *const list)
{
	assert(list);

	Input asm_text = {};
    input_ctor(&asm_text, list);
	RETURN_VOID

    fill_input(&asm_text, argv[1], list);
	RETURN_VOID

	Word* words = word_list_ctor(list);
	get_code(&asm_text, words, list);

	LabelParameters* labels = ctor_labels(list);
	FuncParameters* funcs = ctor_funcs(list);

    Stack stk_code = {};
    stk_ctor(&stk_code, list);
	RETURN_VOID

    assembly(words, labels, funcs, &stk_code, list);
	RETURN_VOID

	dtor_funcs(funcs);
	dtor_labels(labels);
	word_list_dtor(words);
    stk_dtor(&stk_code);
	input_dtor(&asm_text);
}

void proc_dig_code(ErrList *const list)
{
	assert(list);

	Input bin_code = {};
    input_ctor(&bin_code, list);
	RETURN_VOID

	char* dig_file_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
	printf("ENTER DIG FILE NAME!\n");
	scanf("%s", dig_file_name);
	fill_input(&bin_code, dig_file_name, list);
	RETURN_VOID

	free(dig_file_name);

	int* code = (int*)calloc(MAX_FILE_SIZE, sizeof(int));
	size_t dig_amt = 0;

	get_bin_code(&bin_code, code, &dig_amt, list);
	Proc proc = {};
	proc_ctor(&proc, list);
	RETURN_VOID

	proc.size = dig_amt;
	for (int i = 0; i < dig_amt; i++)
		proc.code[i] = code[i];

	free(code);
	
	Stack prog = {};
    stk_ctor(&prog, list);
	Stack stk = {};
    stk_ctor(&stk, list);
	RETURN_VOID

	proc_code(&proc, &prog, &stk, list);

	stk_dtor(&stk);
	stk_dtor(&prog);
	proc_dtor(&proc);
	input_dtor(&bin_code);
}

void disasm(ErrList *const list)
{
	assert(list);

	Input bin_code = {};
    input_ctor(&bin_code, list);
	RETURN_VOID

	char* dig_file_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
	printf("ENTER DIG FILE NAME!\n");
	scanf("%s", dig_file_name);
	fill_input(&bin_code, dig_file_name, list);
	RETURN_VOID

	free(dig_file_name);

	size_t dig_amt = 0;
	int* code = (int*)calloc(MAX_FILE_SIZE, sizeof(int));
	get_bin_code(&bin_code, code, &dig_amt, list);

	char* text = (char*)calloc(MAX_FILE_SIZE, sizeof(char));

	disassembly(code, text, dig_amt, list);
	RETURN_VOID

	free(code);
	free(text);

	input_dtor(&bin_code);
}