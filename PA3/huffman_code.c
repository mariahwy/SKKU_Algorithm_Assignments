#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_CHAR 128
#define MAX_SIZE 10000000
#define swap(x, y) {node* tmp = *(x); *(x) = *(y); *(y) = tmp;}

/* 
    basic data structure
*/

typedef struct node{
    char symbol;
    int weight;
    struct node* left;
    struct node* right;
} node;

// implementation of min heap
typedef struct MinHeap{
    int size;
    node* heap_arr[MAX_CHAR];
} MinHeap;

void init_heap(MinHeap *heap){
    heap -> size = 0;
}

int get_parent(int i) {return (i-1)/2;}

int get_left_child(int i) {return 2*i + 1;}

int get_right_child(int i) {return 2*i + 2;}

void push_down(MinHeap* heap, int i){
    int min = i;
    int left = get_left_child(i);
    int right = get_right_child(i);

    if (left < heap->size && heap->heap_arr[left]->weight < heap->heap_arr[min]->weight){
        min = left;
    }
    if (right < heap->size && heap->heap_arr[right]->weight < heap->heap_arr[min]->weight){
        min = right;
    }
    if (min != i){
        swap(&heap->heap_arr[i], &heap->heap_arr[min]);
        push_down(heap, min);
    }
}

void push_up(MinHeap* heap, int i){
    while (i!=0 && heap->heap_arr[get_parent(i)]->weight > heap->heap_arr[i]->weight){
        swap(&heap->heap_arr[i], &heap->heap_arr[get_parent(i)]);
        i = get_parent(i);
    }
}

void insert_heap(MinHeap* heap, node* value){
    if (heap->size >= MAX_CHAR){
        return;
    }

    int i = heap->size++;
    heap->heap_arr[i] = value;

    push_up(heap, i);
}

node* delete_heap(MinHeap* heap){
    if (heap->size <= 0){
        return NULL;
    }

    node* min = heap->heap_arr[0];
    
    heap->heap_arr[0] = heap->heap_arr[heap->size - 1];
    heap->size--;
    push_down(heap, 0);

    return min;
}

node* make_node(char symbol, int weight){
    node* new_node = (node*)malloc(sizeof(node));
    new_node->symbol = symbol;
    new_node->weight = weight;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}


/*
    compress
*/

// 1. calculate frequencies of symbols in file
void calculate_frequencies(MinHeap* heap, char text[]){
    int freq[MAX_CHAR] = {0};
    int i;
    unsigned char c;

    // calculate frequency
    for (i=0; text[i] != '\0'; i++){
        c = text[i];
        freq[c]++;
    }

    // initialize node
    for (i=0; i < MAX_CHAR; i++){
        if (freq[i] > 0){
            node* new_node = make_node((char)i, freq[i]);
            insert_heap(heap, new_node);
        }
    }
}

node* merge_node(node* left, node* right){
    node* new_node = make_node('\0', left->weight + right->weight); 
    new_node->left = left;
    new_node->right = right;

    return new_node;  // 새로운 노드를 반환
}

// 2. create a binary huffman tree
node* create_huffman_tree(MinHeap* heap, char text[]){
    calculate_frequencies(heap, text);

    while (heap->size > 1){
        node* left = delete_heap(heap);
        node* right = delete_heap(heap);

        node* merged_node = merge_node(left, right);
        insert_heap(heap, merged_node);
    }

    return delete_heap(heap);
}

void encode(node* root, char huffman_code[MAX_CHAR], int level, char code_table[MAX_CHAR][MAX_CHAR]) {
    if (root == NULL) return; 

    if (root->left == NULL && root->right == NULL) {
        huffman_code[level] = '\0';
        strcpy(code_table[(unsigned char)root->symbol], huffman_code);
        return;
    }

    huffman_code[level] = '1';
    encode(root->left, huffman_code, level+1, code_table);

    huffman_code[level] = '0';
    encode(root->right, huffman_code, level+1, code_table);
}

void make_header(node* root, FILE* output1) {
    if (root == NULL) {
        return;
    }

    if (root->left == NULL && root->right == NULL) {
        fprintf(output1, "%c", root->symbol);
    } else {
        fprintf(output1, "(");
        make_header(root->left, output1);
        fprintf(output1, ",");
        make_header(root->right, output1);
        fprintf(output1, ")");
    }
}

// 3. use the binary tree to encode a compressed file
void compress(node* root, char huffman_code[MAX_CHAR], char code_table[MAX_CHAR][MAX_CHAR], FILE* output1, char text[]){
    int i;
    unsigned char c;

    make_header(root, output1);
    fprintf(output1, "\n");

    encode(root, huffman_code, 0, code_table);
    
    for (i=0; text[i] != '\0'; i++){
        c = text[i];
        fprintf(output1, "%s", code_table[c]);
    }
    fprintf(output1, "\n");
}

/*
    uncompress
*/

node* reconstruct_tree(FILE* input2){
    unsigned char c = fgetc(input2);

    if (c == '\n') {
        return NULL;
    }

    if (c == '(') {
        node* left = reconstruct_tree(input2);
        fgetc(input2);
        node* right = reconstruct_tree(input2);
        fgetc(input2);
        
        node* new_node = make_node('\0', 0);
        new_node->left = left;
        new_node->right = right;

        return new_node;
    }

    if (c != ')' || c != ',') {
        node* new_node = make_node(c, 0);

        return new_node;
    }
}

void fprint_ascii(char code_table[MAX_CHAR][MAX_CHAR], FILE* output2){
    int i;
    for (i = 0; i < MAX_CHAR; i++) {
        if (code_table[i][0] != '\0') {
            fprintf(output2, "%c:%s\n", i, code_table[i]);
        }
    }
}

// decoding
void decode(node* root, FILE* input2, FILE* output2){
    node* current = root;
    unsigned char c;

    fgetc(input2); // \n

    while ((c = fgetc(input2)) != '\n'){
        if (c == '1'){
            current = current->left;
        }
        else if (c == '0'){
            current = current->right;
        }

        if (current->left == NULL && current->right == NULL){
            fprintf(output2, "%c", current->symbol);
            current = root;
        }
    }
}

void uncompress(FILE* input2, FILE* output2, char code_table[MAX_CHAR][MAX_CHAR]){
    node* reconstructed_tree = reconstruct_tree(input2);
    fprint_ascii(code_table, output2);
    decode(reconstructed_tree, input2, output2);
}

int main(){
    MinHeap heap;
    init_heap(&heap);

    char huffman_code[MAX_CHAR];
    char code_table[MAX_CHAR][MAX_CHAR] = {0};

    FILE* input1 = fopen("hw3_input.txt", "r");

    char* text = (char*)malloc(MAX_SIZE + 1);

    size_t bytesRead = fread(text, sizeof(char), MAX_SIZE, input1);
    text[bytesRead] = '\0';

    if (bytesRead > 0 && text[bytesRead - 1] == '\n') {
        text[bytesRead - 1] = '\0';
    }

    // compress
    FILE* output1 = fopen("hw3_output1.txt", "w");
    node* huffman_tree = create_huffman_tree(&heap, text);

    compress(huffman_tree, huffman_code, code_table, output1, text);

    fclose(input1);
    fclose(output1);

    // uncompress
    FILE* input2 = fopen("hw3_output1.txt", "r");
    FILE* output2 = fopen("hw3_output2.txt", "w");

    uncompress(input2, output2, code_table);

    fclose(input2);
    fclose(output2);

    free(text);

    return 0;
}
