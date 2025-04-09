#include "stack_processor.h"

void stack_processor::load_program(std::istream& stream)
{
	char ch;
	while (stream.get(ch) && ch!='\n') {
		this->program.add_back(ch);
	}
}

void stack_processor::next_instruction()
{
	//TODO: store program size in variable
	if (this->instruction_ptr >= this->program.size())
		return;

	char instruction = this->program[this->instruction_ptr];
	this->instruction_ptr++;

	switch (instruction)
	{
	case (int)PROGRAM_TOKENS::PUT_EMPTY_LIST_ON_STACK:
		put_empty_list_on_stack();
		break;
	case (int)PROGRAM_TOKENS::COPY_LIST_ON_TOP:
		copy_top();
		break;
	case (int)PROGRAM_TOKENS::COPY_INDEX:
		copy_index();
		break;
	case (int)PROGRAM_TOKENS::SWAP_TOP:
		swap_top();
		break;
	case (int)PROGRAM_TOKENS::READ_CHARACTER:
		read_character();
		break;
	case (int)PROGRAM_TOKENS::MINUS:
		append_minus();
		break;
	case (int)PROGRAM_TOKENS::ABS:
		absolute_value();
		break;
	case (int)PROGRAM_TOKENS::PRINT_STACK:
		print_stack();
		break;

	case (int)PROGRAM_TOKENS::PRINT_FIRST_CHAR_AND_POP:
		print_first_char_and_pop();
		break;
	default: //DEFAULT CASE: INPUT TOKEN
		append_to_top(as_digit(instruction));
		break;
	}

	next_instruction();
}

bool stack_processor::is_digit(const char& character)
{
	if (character >= '0' && character <= '9')
		return true;
	return false;
}

bool stack_processor::is_character(const int& number)
{
	return number > 9;
}

int stack_processor::as_digit(const char& digit_character)
{
	return digit_character - '0';
}

char stack_processor::digit_to_character(const int& digit)
{
	return digit + '0';
}

int stack_processor::list_to_int(const dynamic_array<int>& list)
{
	//TODO: use write recursive version of this
	int power = 0;
	int size = list.size() - 1;
	int minus = digit_to_character(list.front())=='-' ? -1 : 1;
	int last_index = minus == -1 ? 1 : 0; //if minus is present, then on the last index will be minus sign, which we will carry later
	int out = 0;

	for (int x = size; x >= last_index; x--)
		out += list[x] * pow(10, power);

	return out * minus;
}

void stack_processor::append_to_top(char character)
{
	this->memory.top().add_back(character);
}

void stack_processor::append_minus()
{
	//CASE: list is empty -> just add minus at the back
	if (this->memory.top().is_empty()) {
		this->memory.top().add_back((char)PROGRAM_TOKENS::MINUS);
		return;
	}

	if (this->memory.top().back() == (char)PROGRAM_TOKENS::MINUS)
		this->memory.top().remove_back();
	else
		this->memory.top().add_back((char)PROGRAM_TOKENS::MINUS);
}

void stack_processor::absolute_value()
{
	if (this->memory.top().front() == (int)PROGRAM_TOKENS::MINUS)
		this->memory.top().remove(0);
}

void stack_processor::put_empty_list_on_stack()
{
	this->memory.add_top(dynamic_array<int>(STACK_PROCESSOR_RESERVE_LIST_SIZE));
}

void stack_processor::swap_top()
{
	//TOOD: consider using pointers, just to swap pointers instead of copying a whole lot of data
	dynamic_array<int> top_copy = this->memory.top();
	this->memory.pop_top();
	dynamic_array<int> under_top_copy = this->memory.top();
	this->memory.pop_top();


	this->memory.add_top(top_copy);
	this->memory.add_top(under_top_copy);
}

void stack_processor::copy_index()
{
	int index = list_to_int(this->memory.pop_top()); //get number from the top

	if (index < 0 || index > memory.size() - 1)
		throw std::out_of_range("stack_processor::swap_index() -> memory index is out of range");

	int top = this->memory.size() - 1;
	dynamic_array<int> list = this->memory[top - index];
	this->memory.add_top(list);
}

void stack_processor::copy_top()
{
	dynamic_array<int> top = this->memory.top();
	this->memory.add_top(top);
}

void stack_processor::read_character()
{
	char ch = this->input_stream.get();
	if (is_character(ch))
		this->memory.top().add_back(ch);
	else
		this->memory.top().add_back(as_digit(ch));
}


void stack_processor::start_program()
{
	if (this->program.is_empty())
		throw std::runtime_error("Program has not been loaded");

	instruction_ptr = 0; //just in case
	next_instruction();
}

void stack_processor::print_stack()
{
	int starting_idx = 0;
	print_stack_recursive(this->output_stream, starting_idx);
}

void stack_processor::print_first_char_and_pop()
{
	int first = this->memory.top().back();
	char character = is_character(first) ? first : digit_to_character(first);

	output_stream << character<<"\n";
	this->memory.pop_top();
}

void stack_processor::print_stack_recursive(std::ostream& stream, int current_idx)
{
	if (current_idx >= this->memory.size())
		return;

	stream << this->memory.size() - current_idx - 1  << ": "; //this is just to match output requirements

	//TODO: write recursive version of this
	for (int x = this->memory[current_idx].size() - 1; x >= 0; x--) {
		int value = this->memory[current_idx][x];

		if (!is_character(value))
			stream << this->memory[current_idx][x];
		else
			stream << (char)this->memory[current_idx][x];
	}

	stream << "\n";
	print_stack_recursive(stream, ++current_idx);
}
