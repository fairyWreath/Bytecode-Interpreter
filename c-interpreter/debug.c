#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name)
{
	printf("== %s ==\n", name);				// print a little header for debugging

	for (int offset = 0; offset < chunk->count;)	// for every existing instruction in the chunk
	{	
		offset = disassembleInstruction(chunk, offset);		// disassemble individually, offset will be controlled from this function
	}
}

int disassembleInstruction(Chunk* chunk, int offset)
{
	printf("%04d ", offset);	// print byte offset of the given instruction, or the index
	/* quick note on C placeholders
	say, we have int a = 2
	if %2d, it will be " 2"
	if %02d, it will be "02'
	*/


	// show source line each instruction was compiled from
	if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])		// show a | for any instruction that comes from the 
																			//same source as its preceding one

	{
		printf("	| ");
	}
	else
	{
		printf("%4d ", chunk->lines[offset]);
	}

	uint8_t instruction = chunk->code[offset];		// takes one byte, or an element, from the container
	switch (instruction)
	{
	case OP_RETURN:
		return simpleInstruction("OP_RETURN", offset);		// dispatch to a utility function to display it
	case OP_CONSTANT:
		return constantInstruction("OP_CONSTANT", chunk, offset);		// pass in chunk to get ValueArray element
	case OP_NEGATE:
		return simpleInstruction("OP_NEGATE", offset);
	default:
		printf("Unknown opcode %d\n", instruction);
		return offset + 1;
	}
}

static int simpleInstruction(const char* name, int offset)
{
	printf("%s\n", name);	// print as a string, or char*
	return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset)
{
	uint8_t constant = chunk->code[offset + 1];		// pullout the constant index from the subsequent byte in the chunk
	printf("%-16s %4d '", name, constant);			// print out name of the opcode, then the constant index
	printValue(chunk->constants.values[constant]);	//	display the value of the constant,  user defined function
	printf("'\n");
	return offset + 2;			//OP_RETURN is a single byte, and the other byte is the operand, hence offsets by 2
}
