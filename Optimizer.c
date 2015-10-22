/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Fall 2015                                *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

void markVariable(int reg, int offset, Instruction *ins){
	Instruction *temp = ins;
	while(temp != NULL){
		if((temp->opcode == ADD || temp->opcode == SUB || temp->opcode == MUL || temp->opcode == DIV) && temp->field3 == reg){
			temp->critical = 1;
			markCode(temp->field1, temp);
			markCode(temp->field2, temp);
		} else if(temp->opcode == LOADAI && temp->field3 == reg){
			temp->critical = 1;
			markVariable(temp->field1, temp->field2, temp);
		} else if(temp->opcode == LOADI && temp->field2 == reg){
			temp->critical = 1;
		}
		temp = temp->prev;
	}	
}

void markCode(int reg, Instruction *ins){
	Instruction *temp = ins;
	while(temp != NULL){
		if((temp->opcode == ADD || temp->opcode == SUB || temp->opcode == MUL || temp->opcode == DIV) && temp->field3 == reg){
			temp->critical = 1;
			markCode(temp->field1, temp);
			markCode(temp->field2, temp);
		} else if(temp->opcode == LOADAI && temp->field3 == reg){
			temp->critical = 1;
			//markVariable(temp->field1, temp->field2, temp);
		} else if(temp->opcode == LOADI && temp->field2 == reg){
			temp->critical = 1;
		}
		temp = temp->prev;
	}
}

int main()
{
	Instruction *head;

	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}

	Instruction *temp = head;
	while(temp != NULL){
		temp->critical = 0;
		temp = temp->next;
	}
	temp = head;
	temp->critical = 1;
	while(temp != NULL){
		if(temp->opcode == OUTPUTAI){
			temp->critical = 1;
			Instruction *iterator = temp;
			while(iterator != head){
				if(iterator->opcode == STOREAI && iterator->field3 == temp->field2){
					iterator->critical = 1;
					markCode(iterator->field1, iterator);
				}
				iterator = iterator->prev;	
			}
		}
		temp = temp->next;
	}

	if (head) 
		PrintInstructionList(stdout, head);

	temp = head;	
	while(temp != NULL){
		free(head);
		temp = temp->next;
		head = temp;	
	}
	
	return EXIT_SUCCESS;
}

