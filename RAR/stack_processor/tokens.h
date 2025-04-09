enum class PROGRAM_TOKENS {
	INPUT = 0, //INPUT is everything that is not processor instruction token
	PUT_EMPTY_LIST_ON_STACK = '\'',
	COPY_LIST_ON_TOP = ':',
	SWAP_TOP = ';',
	COPY_INDEX = '@',
	READ_CHARACTER = '.',
	MINUS='-',
	ABS = '^', //Absolute value
	PRINT_FIRST_CHAR_AND_POP='>',
	PRINT_STACK='&'

	//add more here
};