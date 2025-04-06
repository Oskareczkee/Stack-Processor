//App Entry point
#include "Stack Processor.h"

using namespace std;

int main()
{
	stack_processor processor;
	processor.load_program(std::cin);

	processor.start_program();
}


/*
* 	dynamic_stack<int> stack;
	dynamic_array<int> array;
	dynamic_stack<dynamic_array<int>> stack_array;

	cout << "Adding items to the stack and array...\n";
	stack.add_top(1);
	stack.add_top(2);
	stack.add_top(3);
	stack.add_top(4);

	array.add_back(0);
	array.add_back(1);
	array.add_back(2);
	array.add_back(3);
	array.add_back(4);

	stack_array.add_top(array);
	
	int idx = 0;
	cout << "Popping items from the stack...\n";

	while (!stack.is_empty())
		cout << idx++ << ". " << stack.pop_top() << "\n";

	cout << "Testing stack array... \n";
	idx = 0;
	while (!array.is_empty()) {
		int item = array[0];
		array.remove(0);
		cout << idx++ << ". " << item << "\n";
	}

	return 0;
*/
