#include "stack_processor.h"
#include <functional>

void stack_processor::load_program(std::istream& stream)
{
	char ch;
	while (stream.get(ch) && ch!='\n') {
		this->program.add_back(ch);
	}

	program_size = this->program.size();
}

void stack_processor::next_instruction()
{
	loop_begin: //label added for conditional jump, if conditional jump occurs it should go to the beginning
	while (this->instruction_ptr < this->program.size()){

		char instruction = this->program[this->instruction_ptr];

		switch (instruction)
		{
			case (int)PROGRAM_TOKENS::PUT_EMPTY_LIST_ON_STACK:
				put_empty_list_on_stack();
				break;
			case (int)PROGRAM_TOKENS::COPY_LIST_ON_TOP:
				copy_top();
				break;
			case (int)PROGRAM_TOKENS::REMOVE_LIST_FROM_TOP:
				remove_list_from_top();
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
			case (int)PROGRAM_TOKENS::PLUS:
				plus();
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
			case (int)PROGRAM_TOKENS::POP_NUMBER_PUT_AS_CHARACTER:
				pop_number_put_as_character();
				break;
			case (int)PROGRAM_TOKENS::POP_NUMBER_PUT_FIRST_ASCII:
				pop_number_put_first_ascii();
				break;
			case (int)PROGRAM_TOKENS::DETACH_FIRST_PUT_TOP:
				detach_first_put_top();
				break;
			case (int)PROGRAM_TOKENS::POP_APPEND_TOP:
				pop_append_top();
				break;

			case (int)PROGRAM_TOKENS::LESS_THAN:
				less_than();
				break;
			case (int)PROGRAM_TOKENS::EQUALS:
				equals();
				break;
			case (int)PROGRAM_TOKENS::NEGATION:
				negation();
				break;

			case (int)PROGRAM_TOKENS::PUT_INSTRUCTION_NUMBER:
				put_instruction_number();
				break;
			case (int)PROGRAM_TOKENS::CONDITIONAL_JUMP: {
				if (conditional_jump())
					goto loop_begin;
				else
					break; //if there was no jump, increase instruction ptr
			}
			default: { //DEFAULT CASE: INPUT TOKEN, convert to digit if is digit, otherwise save as character
					if (is_digit(instruction))
						append_to_top(as_digit(instruction));
					else
						append_to_top(instruction);
					break;
			}
		}

		this->instruction_ptr++;//increment instruction pointer
	}
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

bool stack_processor::is_logical_zero(const dynamic_array<int>& list)
{
	if (list.is_empty() ||  //is empty
		(list.size() == 1 && list.back() == 0)) //has only '0' in it
		return true;
	return false;
}

int stack_processor::list_to_int(const dynamic_array<int>& list)
{
	//TODO: use write recursive version of this
	int power = 0;
	int size = list.size() - 1;
	int minus = (list.front()=='-' ? -1 : 1);
	int last_index = (minus == -1 ? 1 : 0); //if minus is present, then on the last index will be minus sign, which we will carry later
	int out = 0;

	std::function<void(int)> build_number_recursive = [&](int x) {
		if (x < last_index)
			return;
		out += list[x] * pow(10, power++);
		build_number_recursive(--x);
	};

	build_number_recursive(size);

	return out * minus;
}

dynamic_array<int> stack_processor::number_to_list(int number)
{
	//TODO: write recursive version of this
	dynamic_array<int> digits(STACK_PROCESSOR_RESERVE_LIST_SIZE);
	bool is_minus = false;

	//EDGE CASE: number is zero
	if (number == 0) {
		digits.add_back(0);
		return digits;
	}

	if (number < 0) {
		is_minus = true;
		number *= -1; //otherwise loop won't work
	}

	std::function<void(void)> extract_digits_recursive = [&]() {
		if (number <= 0)
			return;
		digits.add_back(number % 10);
		number /= 10;
		extract_digits_recursive();
	};

	extract_digits_recursive();

	if (is_minus)
		digits.add_back((char)PROGRAM_TOKENS::MINUS);

	digits.reverse(); //order is reversed, reverse it

	return digits;
}

bool stack_processor::list_number_equal(const dynamic_array<int>& n1, const dynamic_array<int>& n2)
{
	//TODO: write recursive version of this

	int n1_iter = 0;
	int n2_iter = 0;
	bool n1_minus = n1.front() == '-';
	bool n2_minus = n2.front() == '-';

	//move iterators to left if they have minus signs
	if (n1_minus)	n1_iter++;
	if (n2_minus)	n2_iter++;

	//remove leading zeroes
	n1_iter = skip_leading_zeroes_recursive(n1, n1_iter);
	n2_iter = skip_leading_zeroes_recursive(n2, n2_iter);

	if (n1.size() - n1_iter != n2.size() - n2_iter) //they do not have the same number of significant digits
		return false;

	if (n1_iter - n1.size() == 0 && n2_iter - n2.size() == 0)//they are both zeroes
		return true;

	// Compare sign
	if (n1_minus != n2_minus)
		return false;

	std::function<bool(void)> digits_equal_recursive = [&]() -> bool {
		if (n1_iter >= n1.size() || n2_iter >= n2.size())
			return true; //digits are equal

		if (n1[n1_iter] != n2[n2_iter]) //digits do not match
			return false;

		n1_iter++;
		n2_iter++;
		return digits_equal_recursive();
	};

	return digits_equal_recursive();
}

bool stack_processor::list_less_than(const dynamic_array<int>& n1, const dynamic_array<int>& n2)
{
	int n1_iter = 0;
	int n2_iter = 0;

	//check signs
	bool n1_minus = n1.front() == '-';
	bool n2_minus = n2.front() == '-';

	if (n1_minus) n1_iter++;
	if (n2_minus) n2_iter++;

	//remove leading zeroes
	n1_iter = skip_leading_zeroes_recursive(n1, n1_iter);
	n2_iter = skip_leading_zeroes_recursive(n2, n2_iter);

	if (n1_iter - n1.size() == 0 && n2_iter - n2.size() == 0) //they are both zeroes
		return false;

	if (n1_minus && !n2_minus) return true; // if n1 is negative, then it must be smaller
	if (!n1_minus && n2_minus) return false; // If n1 is positive and n2 is negative, n1 is not less

	//they have same signs, compare length
	if (n1.size() - n1_iter < n2.size() - n2_iter)
		return !n1_minus; // If n1 is positive and has fewer digits, it is less

	if (n1.size() - n1_iter > n2.size() - n2_iter)
		return n1_minus; // If n1 is negative and has more digits, it is less

	//same length, compare digit by digit
	return compare_digits_recursive(n1, n2, n1_iter, n2_iter, n1_minus);
}

int stack_processor::skip_leading_zeroes_recursive(const dynamic_array<int>& n1, int index)
{
	if (index >= n1.size()) return index;           
	if (n1[index] != 0) return index;

	return skip_leading_zeroes_recursive(n1, index + 1);
}

bool stack_processor::compare_digits_recursive(const dynamic_array<int>& n1, const dynamic_array<int>& n2, int n1_iter, int n2_iter, bool is_n1_minus)
{
	//numbers are equal
	if (n1_iter >= n1.size() || n2_iter >= n2.size())
		return false;

	if (n1[n1_iter] < n2[n2_iter])
		return !is_n1_minus;

	if (n1[n1_iter] > n2[n2_iter])
		return is_n1_minus;

	//Digits are equal, keep going
	return compare_digits_recursive(n1, n2, n1_iter + 1, n2_iter + 1, is_n1_minus);
}

void stack_processor::append_to_top(char character)
{
	this->memory.top().add_back(character);
}

void stack_processor::append_minus()
{
	//CASE: list is empty -> just add minus at the back
	if (this->memory.top().is_empty()) {
		this->memory.top().add_front((char)PROGRAM_TOKENS::MINUS);
		return;
	}

	if (this->memory.top().front() == (char)PROGRAM_TOKENS::MINUS)
		this->memory.top().remove_front();
	else
		this->memory.top().add_front((char)PROGRAM_TOKENS::MINUS);
}

void stack_processor::plus()
{
	int n1 = list_to_int(this->memory.pop_top());
	int n2 = list_to_int(this->memory.pop_top());

	dynamic_array<int> out = number_to_list(n1 + n2);
	this->memory.add_top(out);
}

void stack_processor::absolute_value()
{
	//EDGE CASE: list is empty, do nothing
	if (this->memory.top().is_empty())
		return;

	if (this->memory.top().front() == (int)PROGRAM_TOKENS::MINUS)
		this->memory.top().remove(0);
}

void stack_processor::put_empty_list_on_stack()
{
	this->memory.add_top(dynamic_array<int>(STACK_PROCESSOR_RESERVE_LIST_SIZE));
}

void stack_processor::remove_list_from_top()
{
	this->memory.pop_top();
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

void stack_processor::pop_number_put_as_character()
{
	int number = list_to_int(this->memory.pop_top());
	put_empty_list_on_stack();
	this->memory.top().add_back((char)number);
}

void stack_processor::pop_number_put_first_ascii()
{
	int first = this->memory.top().back(); //remember, this could be character, so number can have more than 1 digit
	this->memory.pop_top();
	this->memory.add_top(number_to_list(first));
}

void stack_processor::pop_append_top()
{
	dynamic_array<int> list = this->memory.pop_top();
	this->memory.top().append_front(list);
}

void stack_processor::detach_first_put_top()
{
	int first_char = this->memory.top().back();
	this->memory.top().remove_back();

	put_empty_list_on_stack();
	this->memory.top().add_back(first_char);
}

void stack_processor::less_than()
{

	dynamic_array<int> n1 = this->memory.pop_top();
	dynamic_array<int> n2 = this->memory.pop_top();

	put_empty_list_on_stack();

	if (list_less_than(n2,n1))
		this->memory.top().add_back(1);
	else
		this->memory.top().add_back(0);
}

void stack_processor::equals()
{
	dynamic_array<int> n1 = this->memory.pop_top();
	dynamic_array<int> n2 = this->memory.pop_top();
	put_empty_list_on_stack();

	if (list_number_equal(n1,n2))
		this->memory.top().add_back(1);
	else
		this->memory.top().add_back(0);
}

void stack_processor::negation()
{
	if (is_logical_zero(this->memory.top())) //has only '0' in it
	{
		this->memory.pop_top();
		put_empty_list_on_stack();
		this->memory.top().add_back(1);
	}
	else
	{
		this->memory.pop_top();
		put_empty_list_on_stack();
		this->memory.top().add_back(0);
	}
}

void stack_processor::put_instruction_number()
{
	dynamic_array<int> intstruction_number = number_to_list(this->instruction_ptr);
	this->memory.add_top(intstruction_number);
}

bool stack_processor::conditional_jump()
{
	dynamic_array<int> top_list = this->memory.pop_top();
	bool condition = !is_logical_zero(this->memory.pop_top()); //if is zero, then condition is false, otherwise true

	if (!condition)
	{
		this->memory.add_top(top_list);
		return false;
	}
	else
	{
		this->instruction_ptr = list_to_int(top_list);
		return true;
	}
}

void stack_processor::read_character()
{
	char ch = this->input_stream.get();

	if (is_digit(ch))
		this->memory.top().add_back(as_digit(ch));
	else
		this->memory.top().add_back(ch);
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

	output_stream << character;
	this->memory.pop_top();
}

void stack_processor::print_stack_recursive(std::ostream& stream, int current_idx)
{
	if (current_idx >= this->memory.size())
		return;

	stream << this->memory.size() - current_idx - 1  << ": "; //this is just to match output requirements

	std::function<void(int)> recursive_print = [&](int x) {
		if (x < 0) return;

		int value = this->memory[current_idx][x];

		if (!is_character(value))
			stream << value;
		else
			stream << (char)value;

		recursive_print(x - 1);
	};

	recursive_print(this->memory[current_idx].size() - 1);

	stream << "\n";
	print_stack_recursive(stream, ++current_idx);
}
