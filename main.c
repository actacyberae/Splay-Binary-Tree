#include <stdio.h>
#include <math.h>
#include "./libs/lib.h"
#include "./libs/SBT.h"

int main (int argc, char *argv[]) {
	SBT pSBT = {0, NULL};
	SBTptr pNode;
	Insert_node(&pSBT, 40);
	Insert_node(&pSBT, 30);
	Insert_node(&pSBT, 35);
	Insert_node(&pSBT, 34);
	Insert_node(&pSBT, 32);
	Insert_node(&pSBT, 25);
	Insert_node(&pSBT, 1);
	Insert_node(&pSBT, 27);
	Insert_node(&pSBT, 26);
	Insert_node(&pSBT, 28);
	Insert_node(&pSBT, 60);
	Insert_node(&pSBT, 50);
	Insert_node(&pSBT, 47);
	Insert_node(&pSBT, 55);
	Insert_node(&pSBT, 57);
	Insert_node(&pSBT, 52);
	Insert_node(&pSBT, 54);
	Insert_node(&pSBT, 53);
	Insert_node(&pSBT, 70);
	Insert_node(&pSBT, 65);
	Insert_node(&pSBT, 75);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 65, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 25, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 75, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 34, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 32, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 1, &pNode);
	print_line(STDOUT_FILENO, "DELETE NODE: [%d]\n", pNode->key);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "========\n");
	Get_node(&pSBT, 33, &pNode);
	print_line(STDOUT_FILENO, "========\n");
	Free_node(&pSBT, pNode);
	tree_info(&pSBT);
	
	print_line(STDOUT_FILENO, "Exit\n");
	Free_tree(&pSBT);
	
	return 0;
}
