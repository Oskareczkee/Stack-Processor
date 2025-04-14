enum class PROGRAM_TOKENS {
	INPUT = 0, //INPUT is everything that is not processor instruction token
	PUT_EMPTY_LIST_ON_STACK = '\'',
	REMOVE_LIST_FROM_TOP = ',',
	COPY_LIST_ON_TOP = ':',
	SWAP_TOP = ';',
	COPY_INDEX = '@',
	READ_CHARACTER = '.',
	MINUS = '-',
	PLUS = '+',
	ABS = '^', //Absolute value
	PRINT_FIRST_CHAR_AND_POP = '>',
	POP_NUMBER_PUT_AS_CHARACTER = ']',
	POP_NUMBER_PUT_FIRST_ASCII = '[',
	POP_APPEND_TOP = '#',
	DETACH_FIRST_PUT_TOP = '$',

	LESS_THAN='<',
	EQUALS='=',
	NEGATION='!',
	PUT_INSTRUCTION_NUMBER='~',
	CONDITIONAL_JUMP='?',

	PRINT_STACK='&'

	//add more here
};