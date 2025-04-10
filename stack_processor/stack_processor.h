#include "../data_structures/dynamic_stack.h"
#include "../data_structures/dynamic_array.h"
#include "tokens.h"

#include <iostream> //input, output streams

constexpr int STACK_PROCESSOR_RESERVE_PROGRAM_SIZE = 10000; //reserve initial memory for program
constexpr int STACK_PROCESSOR_RESERVE_LIST_SIZE = 10;       //initial reserved size of list for each list
constexpr int STACK_PROCESSOR_RESERVE_STACK_SIZE = 10;      //initial reserved size of stack for program


class stack_processor 
{
private:
	dynamic_stack<dynamic_array<int>> memory;
	long instruction_ptr = 0;
	long program_size = 0;
	dynamic_array<char> program;

	std::istream& input_stream = std::cin;
	std::ostream& output_stream = std::cout;

//HELPERS:
	void print_stack_recursive(std::ostream& stream, int current_idx);
	void next_instruction();

	bool is_digit(const char& character);
	bool is_character(const int& number);
	int as_digit(const char& digit_character); //this function does not check if character is really digit
	char digit_to_character(const int& digit); //converts digit back into its character form
	bool is_logical_zero(const dynamic_array<int>& list);

	int list_to_int(const dynamic_array<int>& list); //converts data stored in list to number
	dynamic_array<int> number_to_list(int number);

public:
	stack_processor(): memory(dynamic_stack<dynamic_array<int>>(STACK_PROCESSOR_RESERVE_STACK_SIZE)),
					   program(dynamic_array<char>(STACK_PROCESSOR_RESERVE_PROGRAM_SIZE)),
					   instruction_ptr(0){}


	void load_program(std::istream& stream); //load program from stream (can be file stream, standard input stream etc...)

	void append_to_top(char character); //TOKEN -> INPUT


	void append_minus(); //TOKEN -> MINUS
	void plus();
	void absolute_value(); //TOKEN -> ABS

	void put_empty_list_on_stack(); //TOKEN -> PUT_EMPTY_LIST_ON_STACK
	void swap_top(); //TOKEN -> SWAP_TOP
	void copy_index(); //TOKEN -> SWAP_INDEX
	void copy_top(); //TOKEN -> COPY_LIST_ON_TOP

	void pop_number_put_as_character();//TOKEN -> POP_NUMBER_PUT_AS_CHARACTER
	void pop_number_put_first_ascii();//TOKEN -> POP_NUMBER_PUT_FIRST_ASCII
	void pop_append_top(); //TOKEN -> POP_APPEND_TOP
	void detach_first_put_top(); //TOKEN -> DETACH_FIRST_PUT_TOP

	void less_than(); //TOKEN -> LESS_THAN
	void equals(); //TOKEN -> EQUALS
	void negation(); //TOKEN -> NEGATION
	void put_instruction_number(); //TOKEN -> PUT_INSTRUCTION_NUMBER
	void conditional_jump(); //TOKEN -> JUMP

	void read_character(); //reads character from given input (can be standard input) and puts it onto the top of the stack
	void print_stack(); //print current stack state (can be file stream, standard output stream  etc...)
	void print_first_char_and_pop(); //TOKEN -> WRITE_FIRST_CHAR_AND_POP

	void start_program();
};