#include "./SBT.h"

/*
* Простая функция распечатки информации об узле pNode: ключе вершины. Ес
* ли узел имеет левого и/или правого потомка, то информация также выводи
* тся и для них. Формат информации следующий:
* [КЛП|КВ|КПП], где
* КЛП - ключ левого потомка
* КПП - ключ правого потомка
* КВ  - ключ вершины
* Если потомок у вершины отсутствует, то вместо информации о потомке, пи
* шется слово NULL.
*/
void node_info(SBTptr pNode) {
	print_line(STDOUT_FILENO, "[");
	if (pNode != NULL) {
		if (pNode->left != NULL) {
			print_line(STDOUT_FILENO, "%d|", (pNode->left)->key);
		} else {
			print_line(STDOUT_FILENO, "NULL|");
		}
		print_line(STDOUT_FILENO, "%d", pNode->key);
		if (pNode->right != NULL) {
			print_line(STDOUT_FILENO, "|%d", (pNode->right)->key);
		} else {
			print_line(STDOUT_FILENO, "|NULL");
		}
	} else {
		print_line(STDOUT_FILENO, "NULL");
	}
	print_line(STDOUT_FILENO, "]\n");
}

/*
* Рекурсивная функция распечатки содержимого поддерева с корнем pRoot в 
* порядке симметричного обхода дерева. Пусть корень поддерева обозначает
* ся буквой A, а его левый и правый потомки B и C соответственно.
*      |
*      A
*     / \
*    /   \
*   B     C
* Печать:
* > B A C
*/
static void info_in_order_traversal(SBTptr pRoot) {
	if (pRoot != NULL) {
		info_in_order_traversal(pRoot->left);
		node_info(pRoot);
		info_in_order_traversal(pRoot->right);
	}
}

/*
* Обобщенная функция печати дерева pSBT. В качестве входящего параметра 
* рекурсивной функции печати выступает корень дерева pSBT. Т. о., дерево
* обходится в порядке симметричного обхода и последовательно распечатыва
* ется вся информация об узлах.
*/
void tree_info(SBT *pSBT) {
	info_in_order_traversal(pSBT->root);
}

/*
* Функция возвращает родителя вершины pNode дерева pSBT. В случае, если 
* берется корень дерева pSBT, то возвращается само корневое значение. Пу
* сть корень поддерева обозначается буквой A, а его левый и правый потом
* ки B и C соответственно. Данные сохраняются в pParent по значению.
*      |
*      A
*     / \
*    /   \
*   B     C
* Тогда вызов функции get_parent_of_node для узла C вернет вершину A.
*/
static int get_parent_of_node(SBT *pSBT, SBTptr pNode, SBTptr *pParent) {
	int pKey;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	pKey = pNode->key;
	*pParent = pSBT->root;
	while (*pParent != NULL) {
		if ((*pParent)->right == pNode || (*pParent)->left == pNode) {
			return TRUE;
		}
		if ((*pParent)->key > pKey) {
			*pParent = (*pParent)->left;
		} else if ((*pParent)->key < pKey) {
			*pParent = (*pParent)->right;
		} else {
			break;
		}
	}
	return FALSE;
}

static void Get_parent_of_node(SBT *pSBT, SBTptr pNode, SBTptr *pParent) {
	int RETURN;
	
	RETURN = get_parent_of_node(pSBT, pNode, pParent);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("get_parent_of_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("get_parent_of_node: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Функция проверки на то, что узел pNode является левым, либо правым пот
* омком в узла pParent. Если pParent не является родителем pNode, то фун
* кция возвращает отрицательное значение.
*/
static int is_node_a_left_child(SBTptr pNode, SBTptr pParent) {
	if (pNode == NULL || pParent == NULL) {
		return ER_EMPTYNODE;
	}
	if (pParent->left == pNode) {
		return TRUE;
	} else if (pParent->right == pNode) {
		return FALSE;
	} else {
		return ER_UNKNWPRNT;
	} 
}

static int Is_node_a_left_child(SBTptr pNode, SBTptr pParent) {
	int RETURN;
	
	RETURN = is_node_a_left_child(pNode, pParent);
	switch (RETURN) {
		case ER_EMPTYNODE:
			err_msg("is_node_a_left_child: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
		case ER_UNKNWPRNT:
			err_msg("is_node_a_left_child: ER_UNKNWPRNT\n");
			exit(EXIT_FAILURE);
		default:
			return RETURN;
	}
}

/*
* Функция возвращает брата вершины pNode дерева pSBT. В случае, если бер
* ется корень дерева pSBT, то возвращается само корневое значение. Пусть
* корень поддерева обозначается буквой A, а его левый и правый потомки -
* B и C соответственно. Данные сохраняются в pNodeBrother по значению.  
*      |
*      A
*     / \
*    /   \
*   B     C
* Тогда вызов функции get_sibling_node для узла C вернет вершину B.
*/
static int get_sibling_node(SBT *pSBT, SBTptr pNode, SBTptr *pNodeBrother) {
	SBTptr pParent;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	if (pSBT->root != pNode) {
		Get_parent_of_node(pSBT, pNode, &pParent);
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			*pNodeBrother = pParent->right;
		} else {
			*pNodeBrother = pParent->left;
		}
	} else {
		*pNodeBrother = NULL;
	}
	return TRUE;
}

static void Get_sibling_node(SBT *pSBT, SBTptr pNode, SBTptr *pNodeBrother) {
	int RETURN;
	
	RETURN = get_sibling_node(pSBT, pNode, pNodeBrother);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("get_sibling_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("get_sibling_node: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Функция возвращает вершину с минимальным ключом в поддереве с корнем в
* A. Архитектура дерева такова, что минимальная вершина всегда будет леж
* ать слева от корня. Для дерева приведенного ниже, такой вершиной будет
* узел B.
*      |
*      A
*     / \
*    /   \
*   B     C
*/
static int get_minimal_node(SBTptr pRoot, SBTptr *pMinimalNode) {
	if (pRoot == NULL) {
		return ER_EMPTYNODE;
	}
	*pMinimalNode = pRoot;
	while (*pMinimalNode != NULL) {
		if ((*pMinimalNode)->left == NULL) {
			return TRUE;
		}
		*pMinimalNode = (*pMinimalNode)->left;
	}
	return ER_EMPTYNODE;
}

static void Get_minimal_node(SBTptr pRoot, SBTptr *pNodeBrother) {
	int RETURN;
	
	RETURN = get_minimal_node(pRoot, pNodeBrother);
	switch (RETURN) {
		case ER_EMPTYNODE:
			err_msg("get_sibling_node: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* 
*/
static int get_node(SBT *pSBT, int pKey, SBTptr *pNode) {
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	*pNode = pSBT->root;
	while (*pNode != NULL) {
		if ((*pNode)->key > pKey) {
			*pNode = (*pNode)->left;
		} else if ((*pNode)->key < pKey) {
			*pNode = (*pNode)->right;
		} else if ((*pNode)->key == pKey) {
			return TRUE;
		} else {
			break;
		}
	}
	return ER_EMPTYNODE;
}

void Get_node(SBT *pSBT, int pKey, SBTptr *pNode) {
	int RETURN;
	
	RETURN = get_node(pSBT, pKey, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("get_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("get_node: ER_EMPTYNODE\n");
			*pNode == NULL;
	}
}

/*
* Заменяет поддерево с корнем в узле pNode поддеревом с корнем в узле   
* pNewNode. Родителю узла pNode устанавливается новый левый или правый п
* отомок. Если содержимое дерева заменяется полностью, то корень pSBT пр
* инимает новое значение корневого узла pNewNode.                       
*/
static int transplant(SBT *pSBT, SBTptr pNode, SBTptr pNewNode) {
	SBTptr pParent;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	if (pSBT->root != pNode) {
		Get_parent_of_node(pSBT, pNode, &pParent);
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			pParent->left = pNewNode;
		} else {
			pParent->right = pNewNode;
		}
	} else {
		pSBT->root = pNewNode;
	}
	return TRUE;
}

static void Transplant(SBT *pSBT, SBTptr pNode, SBTptr pNewNode) {
	int RETURN;
	
	RETURN = transplant(pSBT, pNode, pNewNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("transplant: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("transplant: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Простая функция проверки на то, является ли вершина внешней по отношен
* ию к родителю. Например, мы имеем поддерево с корнем, обозначенным бук
* вой A, а его левый и правый потомки - B и C соответственно. Каждый пот
* мок имеет дополнительно пару потомков b1, b2 и c1, c2. Тогда внешним у
* злом по отношению к вершине B является узел b1, а внешним узлом по отн
* ошению к вершине C - с2. Такое определение внешних вершин будет полезн
* ым при определении операции вставки нового узла в дерево, когда поведе
* ние алгоритма должно различаться не только для положения ветвей, но и 
* для расположения потомков этих ветвей поддерева.                      
*           |                                                           
*           A                                                           
*          / \                                                          
*         /   \                                                         
*        /     \                                                        
*       /       \                                                       
*      B         C                                                      
*     / \       / \                                                     
*    /   \     /   \                                                    
*   b1   b2   c1   c2                                                   
*/
static int is_node_external(SBT *pSBT, SBTptr pNode) {
	SBTptr pParent;
	SBTptr pGrandParent;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Get_parent_of_node(pSBT, pNode, &pParent);
	Get_parent_of_node(pSBT, pParent, &pGrandParent);
	if (Is_node_a_left_child(pParent, pGrandParent) == TRUE) {
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		if (Is_node_a_left_child(pNode, pParent) == TRUE) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
}

static int Is_node_external(SBT *pSBT, SBTptr pNode) {
	int RETURN;
	
	RETURN = is_node_external(pSBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("is_node_external: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("is_node_external: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
		default:
			return RETURN;
	}
}

/*
* Левый поворот поддерева с корнем в вершине A.                         
*      |         |         |                                            
*      A         |         B                                            
*     / \        |        / \                                           
*    /   \       |       /   \                                          
*   a1    B      |      A     b2                                        
*        / \     |     / \                                              
*       /   \    |    /   \                                             
*      b1   b2   |   a1   b1                                            
*/
static int left_turn(SBT *pSBT, SBTptr *pNode) {
	SBTptr pNewNode;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	pNewNode = (*pNode)->right;
	(*pNode)->right = pNewNode->left;
	pNewNode->left = *pNode;
	if (*pNode != pSBT->root) {
		Transplant(pSBT, *pNode, pNewNode);
	} else {
		pSBT->root = pNewNode;
	}
	return TRUE;
}

static void Left_turn(SBT *pSBT, SBTptr *pNode) {
	int RETURN;
	
	RETURN = left_turn(pSBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("left_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("left_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Правый поворот поддерева с корнем в вершине A.                        
*         |      |      |                                               
*         A      |      B                                               
*        / \     |     / \                                              
*       /   \    |    /   \                                             
*      B    a1   |   b1    A                                            
*     / \        |        / \                                           
*    /   \       |       /   \                                          
*   b1   b2      |      b2   a1                                         
*/
static int right_turn(SBT *pSBT, SBTptr *pNode) {
	SBTptr pNewNode;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	pNewNode = (*pNode)->left;
	(*pNode)->left = pNewNode->right;
	pNewNode->right = *pNode;
	if (*pNode != pSBT->root) {
		Transplant(pSBT, *pNode, pNewNode);
	} else {
		pSBT->root = pNewNode;
	}
	return TRUE;
}

static void Right_turn(SBT *pSBT, SBTptr *pNode) {
	int RETURN;
	
	RETURN = right_turn(pSBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("right_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("right_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Большой левый поворот поддерева с корнем в вершине A осуществляется че
* ерез последовательное применение правого поворота для вершин B и C, а 
* затем левого поворота полученного поддерева относительно вершины A.   
*      |         |      |            |             C                    
*      A         |      A            |            / \                   
*     / \        |     / \           |           /   \                  
*    /   \       |    /   \          |          /     \                 
*   a1    B      |   a1    C         |         /       \                
*        / \     |        / \        |        /         \               
*       /   \    |       /   \       |       A           B              
*      C     b   |      c1    B      |      / \         / \             
*     / \        |           / \     |     /   \       /   \            
*    /   \       |          /   \    |    /     \     /     \           
*   c1   c2      |         c2    b   |   a1     c1   c2      b          
*/
static int left_long_turn(SBT *pSBT, SBTptr *pNode) {
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Right_turn(pSBT, &((*pNode)->right));
	Left_turn(pSBT, pNode);
	return TRUE;
}

static void Left_long_turn(SBT *pSBT, SBTptr *pNode) {
	int RETURN;
	
	RETURN = left_long_turn(pSBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("left_long_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("left_long_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

/*
* Большой правый поворот поддерева с корнем в вершине A осуществляется ч
* ерез последовательное применение левого поворота для вершин B и C, а з
* атем правого поворота полученного поддерева относительно вершины A.   
*         |      |            |      |             C                    
*         A      |            A      |            / \                   
*        / \     |           / \     |           /   \                  
*       /   \    |          /   \    |          /     \                 
*      B    a1   |         C    a1   |         /       \                
*     / \        |        / \        |        /         \               
*    /   \       |       /   \       |       B           A              
*   b     C      |      B    c2      |      / \         / \             
*        / \     |     / \           |     /   \       /   \            
*       /   \    |    /   \          |    /     \     /     \           
*      c1   c2   |   b    c1         |   b      c1   c2     a1          
*/
static int right_long_turn(SBT *pSBT, SBTptr *pNode) {
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (*pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Left_turn(pSBT, &((*pNode)->left));
	Right_turn(pSBT, pNode);
	return TRUE;
}

static void Right_long_turn(SBT *pSBT, SBTptr *pNode) {
	int RETURN;
	
	RETURN = right_long_turn(pSBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("right_long_turn: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("right_long_turn: ER_EMPTYNODE\n");
			exit(EXIT_FAILURE);
	}
}

static void ZigZag_turn(SBT *pSBT, SBTptr *pParent, SBTptr *pGrandParent) {
	if (Is_node_a_left_child(*pParent, *pGrandParent) == TRUE) {
		Right_long_turn(pSBT, pGrandParent);
	} else {
		Left_long_turn(pSBT, pGrandParent);
	}
}

static void ZigZig_turn(SBT *pSBT, SBTptr *pParent, SBTptr *pGrandParent) {
	if (Is_node_a_left_child(*pParent, *pGrandParent) == TRUE) {
		Right_turn(pSBT, pGrandParent);
		Right_turn(pSBT, pParent);
	} else {
		Left_turn(pSBT, pGrandParent);
		Left_turn(pSBT, pParent);
	}
}

static void Zig_turn(SBT *pSBT, SBTptr pNode, SBTptr *pParent) {
	if (Is_node_a_left_child(pNode, *pParent) == TRUE) {
		Right_turn(pSBT, pParent);
	} else {
		Left_turn(pSBT, pParent);
	}
}

/*
* Каждый раз добавляя узел в дерево, требуется проводить балансировку, з
* аключающуюся в перемещении добавленного узла в корень дерева, а размещ
* ение ветвей должно осуществляться за счет операций zig, zig-zag и 
* zig-zig. Эта операция должна предшествовать каждый раз при удалении уз
* ла: удаляемый узел перемещается в корень, а затем производится удалени
* е с правилами, которые определены для BST.
*/
static int tree_balancing(SBT *pSBT, SBTptr pNode, SBTptr pParent) {
	SBTptr pGrandParent;
	
	pGrandParent = NULL;
	while (1) {
		Get_parent_of_node(pSBT, pParent, &pGrandParent);
		if (pGrandParent != pParent) {
			if (is_node_external(pSBT, pNode) == TRUE) {
				ZigZig_turn(pSBT, &pParent, &pGrandParent);
			} else {
				ZigZag_turn(pSBT, &pParent, &pGrandParent);
			}
		} else {
			Zig_turn(pSBT, pNode, &pParent);
		}
		if (pNode == pSBT->root) {
			break;
		}
		Get_parent_of_node(pSBT, pNode, &pParent);
	}
	return TRUE;
}

/*
* 
*/
static void declare_a_new_node(SBT *pSBT, SBTptr *pRoot, SBTptr *pNode, int pKey) {
	*pNode = *pRoot = (SBTptr) Malloc(sizeof(SBTnode));
	(*pNode)->key = pKey;
	(*pNode)->left = (*pNode)->right = NULL;
	pSBT->size++;
}

/*
* 
*/
static void insert_node(SBT *pSBT, SBTptr *pRoot, SBTptr pNode, int pKey) {
	SBTptr pParent;
	
	if (pNode == NULL) {
		declare_a_new_node(pSBT, pRoot, &pNode, pKey);
	} else {
		while (TRUE) {
			if (pKey < pNode->key) {
				if (pNode->left == NULL) {
					pParent = pNode;
					declare_a_new_node(pSBT, &pNode->left, &pNode, pKey);
					break;
				} else {
					pRoot = &pNode->left;
					pNode = pNode->left;
				}
			} else {
				if (pNode->right == NULL) {
					pParent = pNode;
					declare_a_new_node(pSBT, &pNode->right, &pNode, pKey);
					break;
				} else {
					pRoot = &pNode->right;
					pNode = pNode->right;
				}
			}
		}
		tree_balancing(pSBT, pNode, pParent);
	}
}

/*
 * 
 * */
void Insert_node(SBT *pSBT, int pKey) {
	insert_node(pSBT, &pSBT->root, pSBT->root, pKey);
}

/*
* Рекурсивная функция free_tree помещает выделенную память под узел дере
* ва в список повторно используемых областей. Указатель pNode может быть
* равным NULL и проверка, используемая внутри функции указателя, использ
* уется для  последовательное перемещения по поддереву с корнем в pRoot 
* в порядке левого обхода.
*/
static void free_tree(SBTptr pRoot) {
	if (pRoot != NULL) {
		free_tree(pRoot->left);
		free_tree(pRoot->right);
		free(pRoot);
	}
}

/*
* Для очистки всего дерева pSBT используется функция Free_tree, которая 
* задействует рекурсивную функцию free_tree, в качестве параметра которо
* й передает корень дерева pSBT->root.
*/
void Free_tree(SBT *pSBT) {
	free_tree(pSBT->root);
}

/*
* Случай, когда удаляется лист дерева.
*/
static SBTptr case_A(SBT *pSBT, SBTptr *pDeletedNode) {
	SBTptr pBalancingNode;
	
	Get_sibling_node(pSBT, *pDeletedNode, &pBalancingNode);
	Transplant(pSBT, *pDeletedNode, NULL);
	return pBalancingNode;
}

/*
* Случай, когда удаляется вершина дерева с одним потомком.
*/
static SBTptr case_B(SBT *pSBT, SBTptr *pDeletedNode) {
	SBTptr pNewNode;
	SBTptr pBalancingNode;
	
	Get_sibling_node(pSBT, *pDeletedNode, &pBalancingNode);
	if ((*pDeletedNode)->left != NULL) {
		pNewNode = (*pDeletedNode)->left;
	} else {
		pNewNode = (*pDeletedNode)->right;
	}
	Transplant(pSBT, *pDeletedNode, pNewNode);
	return pBalancingNode;
}

/*
* Случай, когда удаляется вершина дерева с двумя потомками.
*/
static SBTptr case_C(SBT *pSBT, SBTptr *pDeletedNode) {
	SBTptr pMinimalNodeFromRightSubtreeOfDeletedNode;
	SBTptr pParentOfMinimalNodeFromRightSubtreeOfDeletedNode;
	SBTptr pBalancingNode;
	
	Get_minimal_node((*pDeletedNode)->right, &pMinimalNodeFromRightSubtreeOfDeletedNode);
	Get_parent_of_node(pSBT, pMinimalNodeFromRightSubtreeOfDeletedNode, &pParentOfMinimalNodeFromRightSubtreeOfDeletedNode);
	Get_sibling_node(pSBT, pMinimalNodeFromRightSubtreeOfDeletedNode, &pBalancingNode);
	
	if (pParentOfMinimalNodeFromRightSubtreeOfDeletedNode != *pDeletedNode) {
		pParentOfMinimalNodeFromRightSubtreeOfDeletedNode->left = pMinimalNodeFromRightSubtreeOfDeletedNode->right;
	}
	if (pMinimalNodeFromRightSubtreeOfDeletedNode != (*pDeletedNode)->right) {
		pMinimalNodeFromRightSubtreeOfDeletedNode->right = (*pDeletedNode)->right;
	}
	pMinimalNodeFromRightSubtreeOfDeletedNode->left = (*pDeletedNode)->left;
	Transplant(pSBT, *pDeletedNode, pMinimalNodeFromRightSubtreeOfDeletedNode);
	return pBalancingNode;
}

static int free_node(SBT *pSBT, SBTptr pNode) {
	SBTptr pBalancingNode;
	
	if (pSBT == NULL) {
		return ER_EMPTYTREE;
	}
	if (pNode == NULL) {
		return ER_EMPTYNODE;
	}
	Get_parent_of_node(pSBT, pNode, &pBalancingNode);
	tree_balancing(pSBT, pNode, pBalancingNode);
	pSBT->size--;
	if (pNode->right == NULL && pNode->left == NULL) {
		pBalancingNode = case_A(pSBT, &pNode);
	} else if (pNode->right == NULL || pNode->left == NULL) {
		pBalancingNode = case_B(pSBT, &pNode);
	} else {
		pBalancingNode = case_C(pSBT, &pNode);
	}
	free(pNode);
	
	return TRUE;
}

void Free_node(SBT *pSBT, SBTptr pNode) {
	int RETURN;
	
	RETURN = free_node(pSBT, pNode);
	switch (RETURN) {
		case ER_EMPTYTREE:
			err_msg("free_node: ER_EMPTYTREE\n");
			exit(EXIT_FAILURE);
		case ER_EMPTYNODE:
			err_msg("free_node: ER_EMPTYNODE\n");
	}
}
