#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_ORDER 100
#define ORDER_NUM 17
#define ORDER_LEN 20
#define ADRS_END 65535
#define MAX_HASH 20
#define MAXLEN 100
#define MAX_REF_NUM 10
#define FFFFFF 16777215
#define DEVICE_STATEMENT 1 //device ready
#define DEVICE_INPUT 0


int help();
int dir();
int history();
int put_history(char*);
int dump();
int dump_1(char*);
int dump_2(char*, char*);
int reset();
int round_down(int);
int edit(int, int);
int fill(char*, char*, char*);
int get_txt();
int hashCode(char*);
int PrintAllHashData();
int opcode(char*);
int type(char*);
int assemble(char*);
int put_LOCTAB(int);
int put_ADRSTAB(char*, int);
int check_operation(char*);
int is_format(char*);
int is_directive(char*);
int make_lst(char*);
int print_LOCTAB();
int symbol();
int is_reg(char*);
int is_no_obj(char*);
int is_variable(char*);
int get_opcode(char*);
int get_symbol_adrs(char*);
int get_pc();
char* get_objcode(char*,char*,char*);
void put_OBTAB(char*);
int print_OBTAB();
void ReverseStr(char*);
void make_obj(char*);
int put_sorted_symbol(char*, int);

//project3
int progaddr(char*);
char* split(char*,int,int);
int loader(char*, char*, char*);
int str_to_int(char*, int);
void put_ESTAB(char*, char*, int, int);
void clear_ESTAB();
int search_ESTAB(char*, char*);
int hex_to_ascii(int);
char* get_from_mem(int m_adrs, int m_len);
char* put_zero_str(char*,int);
int two_complement(int);
char* delete_white_space(char*);
int run();
int print_bp();
int bp(char*);
int clear_bp();
int get_operation(int);
int excute_operation();
void insert_mem(int,int,int);
char* get_xbpe(char*);

int order_flag;
int exit_flag;
int first_order;
int his_num;
int dump_pc;
int mem[65536] = {0};
int history_operand1;
int history_operand2;
int first_loc=0;
int LOC_START;
int post_loc;
int start_flag;
int first_ob=1;
int BASE;
char obj_code[255];
char PROGRAM_NAME[50];
int START_PROGRAM=0;
int END_PROGRAM=0;
char ERROR[20];

//project3
int program_adrs=0;
int total_len;
int PROGRAM_END;
int A=0;
int X=0;
int L=0;
int PC=0;
int B=0;
int S=0;
int T=0;
int SW=0;
char now_seeing_operation[20];
char now_seeing_operand[20];
int ni=0;
int format=0;
int STEP=0;

typedef struct order_history
{
   char data[MAX_ORDER];
   int num;
   char operand1[MAX_ORDER];
   char operand2[MAX_ORDER];
   struct order_history *link;
}Node, *pNode;
pNode head, p, tail;

Node *talloc(void)
{
   return (Node *)malloc(sizeof(Node));
}

typedef struct HashTable
{
   int key;
   char operation_name[20];
   char opcode[10];
	char format[10];
   struct HashTable* Next;
}HashTable;

struct HashTable* HT[20];

int AddHashData(int, HashTable*);

typedef struct SYMTAB
{
	int key;
	char symbol[30];
	int adrs;
	struct SYMTAB* Next;
}SYMTAB;

struct SYMTAB* ST[20];

int put_SYMTAB(char*, int);

typedef struct sorted_symbol
{
	char symbol[30];
	int adrs;
	struct sorted_symbol *link;
}sorted_symbol, *pSorted_sym;
pSorted_sym ss_head, ss_p, ss_tail;

sorted_symbol *sym_talloc(void)
{
	return (sorted_symbol *)malloc(sizeof(sorted_symbol));
}

char order_list[ORDER_NUM][ORDER_LEN] = {"h","help", "dir", "d", "quit", "q", "history", "hi", "dump",
   "du", "edit", "e","fill","f", "opcode", "opcodelist","reset"};

typedef struct LOCTAB
{
	char name[30];
//	char operation[30];
//	char operand1[30];
//	char operand2[30];
	int LOC;
	struct LOCTAB *Next;
}LOCTAB, *pLOCTAB;
pLOCTAB loc_head, loc_p, loc_tail, pass2_loc_p;

LOCTAB *loc_talloc(void)
{
	return (LOCTAB*)malloc(sizeof(LOCTAB));
}

typedef struct OBTAB
{
	char obj[100];
	struct OBTAB *Next;
}OBTAB, *pOBTAB;
pOBTAB ob_head, ob_p, ob_tail;

OBTAB* ob_talloc(void)
{
	return (OBTAB*)malloc(sizeof(OBTAB));
}

//project3
typedef struct ESTAB
{
	char cs_name[20];
	char sym_name[20];
	int adrs;
	int len;
	struct ESTAB *Next;
}ESTAB, *pESTAB;
pESTAB es_head, es_p, es_tail;

ESTAB* es_talloc(void)
{
	return (ESTAB*)malloc(sizeof(ESTAB));
}

typedef struct BP
{
	int data;
	struct BP *Next;
}BP, *pBP;
pBP front, rare;

BP* bp_talloc(void)
{
	return (BP*)malloc(sizeof(BP));
}

BP* POP();

int changing_prompt(){

   /*changing prompt*/
   printf("%c[1;33m",27);
   printf("sicsim> ");
   printf("%c[0m",27);

   return 0;}

char* itoa(int num, char* str, int radix)
{
	char* p = str;

	while(num) {
		if(radix<=10)
			*p++ = (num % radix) + '0';
		else {
			int t = num % radix;
			if ( t <= 9 )
				*p++ = t + '0';
			else
				*p++ = t - 10 + 'A';
		}
		num /= radix;
	}
	*p = '\0';
	if(radix==16) ReverseStr(str);
	return str;
}

void ReverseStr(char* s)
{
	int size = strlen(s);
	char temp[2];
	for(int i=0;i<size/2;i++) {
		temp[0] = s[i];
		s[i] = s[(size-1)-i];
		s[(size-1)-i]=temp[0];
	}
	return;
}
