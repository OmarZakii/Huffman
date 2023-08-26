#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
struct tree {
	int freq;
	char charcter;
	tree* left, * right, * parent;
	tree() {
		freq = 1;
		charcter = NULL;
		left = NULL;
		right = NULL;
		parent = NULL;
	}

};
struct Node {
	Node* next;
	tree* tree_node;
	Node() {
		next = NULL;
		tree_node = new tree;
	}
};
struct info {
	info* follow;
	char binary[5];
	char letter;
	info() {
		follow = NULL;
		binary[0] = '\0';
		letter = ' ';
	}
};

Node* find(Node* list, char item)
{
	if (list == NULL)
		return NULL;
	else
	{
		Node* it = list;
		while (it != NULL)
		{
			if (it->tree_node->charcter == item)
				return it;
			else
				it = it->next;
		}
	}
	return NULL;
}
void swap_nodes(Node* node1, Node* node2) {
	if (node1->tree_node->freq == node2->tree_node->freq)
		return;

	tree* temp = node1->tree_node;
	node1->tree_node = node2->tree_node;
	node2->tree_node = temp;
}
void sort(Node* list) {
	Node* i, * j;
	for (i = list; i != NULL; i = i->next) {
		for (j = list; j != NULL; j = j->next) {
			if (i->tree_node->freq < j->tree_node->freq)
				swap_nodes(i, j);
		}

	}
}//credit for Karim kohel 
Node* insert(Node* list, char item) {
	if (list == NULL) {
		list = new Node;
		list->tree_node->charcter = item;
		list->tree_node->freq = 1;
		list->next = NULL;

	}
	else {
		Node* var = find(list, item);
		if (var != NULL)
			var->tree_node->freq = var->tree_node->freq + 1;
		else {
			Node* temp = new Node;
			temp->tree_node->charcter = item;
			temp->tree_node->freq = 1;
			temp->next = list;
			list = temp;

		}
	}
	sort(list);
	return list;
}
Node* remove(Node* list, tree** tree_node) {

	if (list == NULL)
		return NULL;
	if (list->next == NULL) {
		*tree_node = list->tree_node;
		free(list);
		list = NULL;
	}
	else {
		Node* temp = list->next;
		*tree_node = list->tree_node;
		free(list);
		list = temp;

	}

	return list;

}
struct Queue {
	Node* base;
	Queue()
	{
		base = NULL;
	}
};
Queue* enqueue(Queue* list, char item) {
	if (list == NULL)
		list = new Queue;
	list->base = insert(list->base, item);
	return list;

}
tree* dequeue(Queue* list, tree* node1) {
	if (list->base->tree_node == NULL)
		return NULL;
	list->base = remove(list->base, &node1);//recomennded by a friend to use a double pointer
	return node1;
}
Queue* insert_tree(Queue* list, tree* node_1, tree* node_2) {
	if (list->base == NULL && node_1->freq <= 1)
	{
		list->base = new Node;
		list->base->tree_node->charcter = '#';
		list->base->tree_node->parent = NULL;
		if (node_1->freq > node_2->freq) {
			list->base->tree_node->right = node_1;
			list->base->tree_node->left = node_2;
		}
		else {
			list->base->tree_node->left = node_1;
			list->base->tree_node->right = node_2;
		}

		return list;
	}
	tree* parent_node = new tree;
	Node* tmp_node = new Node;
	Node* temp = list->base;
	parent_node->parent = NULL;
	parent_node->charcter = '#';
	parent_node->freq = node_1->freq + node_2->freq;
	if (node_1->freq > node_2->freq) {
		parent_node->right = node_1;
		parent_node->left = node_2;
		parent_node->left->parent = parent_node;
		parent_node->right->parent = parent_node;
	}
	else {
		parent_node->right = node_2;
		parent_node->left = node_1;
		parent_node->left->parent = parent_node;
		parent_node->right->parent = parent_node;
	}
	tmp_node->tree_node = parent_node;
	tmp_node->tree_node->parent = parent_node->parent;
	tmp_node->next = temp;
	list->base = tmp_node;
	return list;
}
Queue* freq_table(Queue* list) {
	tree* leaf_1 = NULL;
	tree* leaf_2 = NULL;
	Node* node;
	Queue* temp = list;
	while (list->base->next != NULL) {

		leaf_1 = dequeue(list, leaf_1);
		leaf_2 = dequeue(list, leaf_2);
		list = insert_tree(list, leaf_1, leaf_2);
		sort(list->base);
	}
	return list;

}

void generate_binary(tree* nav_tree, char* array, int c, info** binary_var) { // credit for afriend outside college  
	if (nav_tree->left != NULL) {
		if (c < strlen(array) - 1)
		{
			int m = strlen(array);
			for (int i = c + 1; i < m; i++)
			{
				array[i] = '\0';
			}
		}
		array[c] = '0';
		generate_binary(nav_tree->left, array, c + 1, binary_var);

	}

	if (nav_tree->right != NULL)
	{
		if (c < strlen(array) - 1)
		{
			int m = strlen(array);
			for (int i = c + 1; i < m; i++)
			{
				array[i] = '\0';
			}
		}
		array[c] = '1';
		generate_binary(nav_tree->right, array, c + 1, binary_var);

	}


	if (nav_tree->left == NULL && nav_tree->right == NULL) {

		if (*binary_var == NULL) {
			info* temp = new info;

			*binary_var = temp;
			for (int i = 0; i < 5; i++)
				(*binary_var)->binary[i] = array[i];
			(*binary_var)->letter = nav_tree->charcter;
			(*binary_var)->follow = NULL;
		}
		else {
			info* temp = new info;

			temp->letter = (*binary_var)->letter;
			temp->follow = (*binary_var)->follow;
			for (int j = 0; j < 5; j++)
				temp->binary[j] = (*binary_var)->binary[j];
			for (int j = 0; j < 9; j++)
				(*binary_var)->binary[j] = array[j];
			(*binary_var)->letter = nav_tree->charcter;
			(*binary_var)->follow = new info;
			(*binary_var)->follow->letter = temp->letter;
			(*binary_var)->follow->follow = temp->follow;
			for (int j = 0; j < 9; j++)
				(*binary_var)->follow->binary[j] = temp->binary[j];


		}
		printf("%c", nav_tree->charcter);
		for (int i = 0; i < 5; i++)
			printf("%c", array[i]);

	}


}
info* find_info(info* list, char item)
{
	if (list == NULL)
		return NULL;
	else
	{
		info* it = list;
		while (it != NULL)
		{
			if (it->letter == item)
				return it;
			else
				it = it->follow;
		}
	}
	return NULL;
}

info* equivalent(info* list, char item, char* arr) {
	info* var = new info;
	var = find_info(list, item);
	if (var->letter == item) {
		for (int i = strlen(var->binary); i >= 0; i--) {
			if (arr[0] == '\0') {
				arr[0] = var->binary[i];
			}
			else {
				for (int j = strlen(arr); j >= 0; j--)
					arr[j + 1] = arr[j];
				arr[0] = var->binary[i];
			}
		}
	}


	return list;
}
void compression(char* arr, char* compressed_text, int* count) {
	while (strlen(arr) % 8 != 0) {
		for (int j = strlen(arr); j >= 0; j--)
			arr[j + 1] = arr[j];
		arr[0] = '0';
		(*count)++;
		int x = 0;
	}
	char array[8] = { 0 };
	int x = strlen(arr);
	int value = 0;
	int var_pow = 0;
	for (int x = strlen(arr); x >= 0; x--) {
		for (int i = 0; i <= 7; i++) {
			if (arr[x] == '\0') {
				x--;
				i--;
			}
			else {
				array[i] = arr[x];
				x--;
			}
		}
		for (int j = 0; j <= 7; j++) {
			if (array[j] == '1')
				var_pow += pow(2, j);
		}
		compressed_text[value] = (char)var_pow;
		value++;
	}

}
void display_ratio(float in_val, float out_val) {
	float ratio;
	ratio = out_val / in_val * 100;
	printf("ratio =%f %", ratio);

}
void decompress(char* arr, tree* tree, char temp[], int count) {
	for (int i = 0; i < count; i++) {
		for (int j = 0; j <= strlen(arr); j++)
			arr[j] = arr[j + 1];
	}
	int x = strlen(arr);
	int y = tree->freq;
	int j = 0;

	for (int i = 0; i < x; i++) {
		if (arr[i] == '0') {
			tree = tree->left;

		}
		else if (arr[i] == '1') {
			tree = tree->right;

		}
		if (tree->right == NULL && tree->left == NULL) {
			for (int j = strlen(arr); j >= 0; j--) {
				temp[j + 1] = temp[j];
			}

			temp[0] = tree->charcter;

			while (tree->parent != NULL) {
				tree = tree->parent;

			};

		}

	}


}
int main() {
	FILE* reading_file;
	char words[1000];
	Queue* text = NULL;
	char str1[1000] = { 0 };
	Queue* table_queue = NULL;
	bool t_f;
	FILE* new_file;
	fopen_s(&reading_file, "FINALPROJECT.txt", "r");
	fgets(words, 1000, reading_file);
	printf("file:%s", words);
	fclose(reading_file);

	for (int i = 0; i < strlen(words); i++)
	{
		text = enqueue(text, words[i]);

	}
	info* binary_var = NULL;
	text = freq_table(text);
	generate_binary(text->base->tree_node, str1, 0, &binary_var);
	char arr[100] = { 0 };
	for (int i = 0; i < strlen(words); i++)
	{

		binary_var = equivalent(binary_var, words[i], arr);
	}
	float i, j;

	char v = { 'a' };
	i = v;
	char ascii_arr[40] = { 0 };

	int count = 0;
	compression(arr, ascii_arr, &count);
	i = (float)strlen(words);
	j = (float)strlen(ascii_arr);
	char decompression_text[50] = { 0 };
	display_ratio(i, j);
	decompress(arr, text->base->tree_node, decompression_text, count);
	FILE* file_2;
	fopen_s(&new_file, "decompresserd file.dat", "w");
	fprintf(new_file, "%s", decompression_text);
	fclose(new_file);
	fopen_s(&file_2, "compresserd file.dat", "w");
	fprintf(file_2, "%s", ascii_arr);
	fclose(file_2);


	_getch();
}