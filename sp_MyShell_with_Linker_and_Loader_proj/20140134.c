#include "20140134.h"

int main()
{ 
	get_txt(); //get opcode.txt
	reset(); //initialize hashtable

	dump_pc=0;
	exit_flag=0;
	first_order=0;
	his_num=0;
	head=talloc();
	loc_head=loc_talloc();
	ob_head=ob_talloc();
	ss_head=sym_talloc();
	ss_head=NULL;

	//until user inputs "quit"
	while(exit_flag!=1)
	{
		changing_prompt(); //make 'sicsim>'

		char buf[MAX_ORDER];
		char buf_store[MAX_ORDER];

		//get order
		fgets(buf, sizeof(buf), stdin);
		strcpy(buf_store,buf); //buf_store is for different format of input

		//split string for distinguishing operation and operands.
		char operation[20];
		operation[0]='\0';
		char operand1[10];
		operand1[0]='\0';
		char operand2[10];
		operand2[0]='\0';
		char operand3[10];
		operand3[0]='\0';

		int index=0;
		int op_count=0;
		int operation_count=0;
		int operand1_count=0;
		int operand2_count=0;
		int operand3_count=0;
		char ORDER[100];

		strcpy(ORDER,buf);
		char char_buf[5]="";
		int comma_flag=0;
		int comma_flag2=0;
		
		while(ORDER[index]!='\0')
		{
			if(ORDER[index]==' ') while(ORDER[index]!=' ') index++;
			if(op_count==0)
			{
				if(ORDER[index]=='\0');
				else if(ORDER[index]==' ');
				else
				{
					char_buf[0]=ORDER[index];
					strcat(operation, char_buf);
					operation_count=1;
				}
				if((operation_count==1)&&(ORDER[index]==' '))op_count++;
			}
			else if(op_count==1)
			{
				if(ORDER[index]=='\0');
				else if(ORDER[index]==',') comma_flag=1;
				else if(ORDER[index]==' ');
				else
				{
					char_buf[0]=ORDER[index];
					strcat(operand1, char_buf);
					operand1_count=1;
				}
				if((ORDER[index+1]!=' ')&&comma_flag) op_count++;
			}
			else if((op_count==2)||((comma_flag==1)&&(comma_flag2!=1)))
			{
				if(ORDER[index]=='\0');
				else if(ORDER[index]==',') {comma_flag2=1;}
				else if(ORDER[index]==' ');
				else
				{
					char_buf[0]=ORDER[index];
					strcat(operand2, char_buf);
					operand2_count=1;
				}
				if((ORDER[index+1]!=' ')&&comma_flag2) op_count=3;
			}
			else if((op_count==3)||(comma_flag2==1))
			{
				if(ORDER[index]=='\0');
				else if(ORDER[index]==' ');
				else
				{
					char_buf[0]=ORDER[index];
					strcat(operand3, char_buf);
					operand3_count=1;
				}
			}
			index++;
		}
		//spiliting string is finished

		//for each cases, add null character at the end of each string
		if((operation_count==1)&&(operand1_count!=1)&&(operand2_count!=1)&&(operand3_count!=1))
		{
			int operation_last=strlen(operation);
			operation[operation_last-1]='\0';
		}
		else if((operation_count==1)&&(operand1_count==1)&&(operand2_count!=1)&&(operand3_count!=1))
		{
			int operand1_last=strlen(operand1);
			operand1[operand1_last-1]='\0';
		}
		else if((operation_count==1)&&(operand1_count==1)&&(operand2_count==1)&&(operand3_count!=1))
		{
			int operand2_last=strlen(operand2);
			operand2[operand2_last-1]='\0';
		}
		else if((operation_count==1)&&(operand1_count==1)&&(operand2_count==1)&&(operand3_count==1))
		{
			int operand3_last=strlen(operand3);
			operand3[operand3_last-1]='\0';
		}
		//adding null character at the end of each string is finished


		/*compare operation and operands with proper operands and operands
	     if user-inputs are proper, execute the operation with operands
		  		and put that operation and operands in history
		  if not, print ERROR massage.
		  in each cases, have to check numbers of operands
		 	for printing ERROR massage.*/
		if((!strcmp(operation,"help"))||(!strcmp(operation,"h")))
		{
			if((operand1[0]!='\0')||(operand2[0]!='\0'))
			{
				printf("ERROR : Incorrect Operands\n");
			}
			else
			{
				put_history(operation);
				help();
			}
		}
		else if(!strcmp(operation,"dir")||(!strcmp(operation,"d")))
		{
			if((operand1[0]!='\0') || operand2[0]!='\0')
			{
				printf("ERROR : Incorrect Operands\n");
			}
			else
			{
				put_history(operation);
				dir();
			}
		}
		else if((!strcmp(operation,"history"))||(!strcmp(operation,"hi")))
		{
			if((operand1[0]!='\0')||(operand2[0]!='\0'))
			{
				printf("ERROR : Incorrect Operands\n");
			}
			else
			{
				put_history(operation);
				p=head;
				while(p!=NULL)
				{
					printf("%d\t%s\n",p->num, p->data);
					p=p->link;
				}
			}

		}
		else if((!strcmp(operation,"quit"))||(!strcmp(operation,"q")))
		{
			if((operand1[0]!='\0')||(operand2[0]!='\0'))
			{
				printf("ERROR : Incorrect Operands\n");
			}
			else
			{
			exit_flag=0;
			break;
			}
		}
		else if((!strcmp(operation,"dump"))||(!strcmp(operation,"du")))
		{
			if((operand1[0]=='\0')&&(operand2[0]=='\0'))
			{
				if(!dump()) //if there is no operand, execute dump()
				{
					put_history(buf);
				}
			}
			else if((operand1[0]!='\0')&&(operand2[0]=='\0'))
			{
				if(!dump_1(operand1)) //if there is one operand, execute dump1()
				{
					put_history(buf);
				}
			}
			else if((operand1[0]!='\0')&&(operand2[0]!='\0')&&(operand3[0]=='\0'))
			{ //if there is two operands, execute dump2
				if(!dump_2(operand1, operand2))
				{
					put_history(buf);
				}
			}
			else printf("\nERROR : Incorrect Operands\n");
		}
		else if((!strcmp(operation,"edit"))||(!strcmp(operation,"e")))
		{
			if((operand1[0]!='\0')&&(operand2[0]!='\0'))
			{
				int adrs = str_to_int(operand1,16);
				int val = str_to_int(operand2,16);
				if(!edit(adrs, val))
				{
					put_history(buf);
				}
			}
			else printf("\nERROR\n");
		}
		else if(!strcmp(operation,"reset"))
		{
			put_history(operation);
			reset();
		}
		else if((!strcmp(operation,"fill"))||(!strcmp(operation,"f")))
		{
			if((operand1[0]!='\0')&&(operand2[0]!='\0')&&(operand3[0]!='\0'))
			{
				if(!fill(operand1, operand2, operand3))
				{
					put_history(buf);
				}
				else printf("\nERROR : Incorrect Operands\n");
			}
			else printf("\nERROR : Incorrect Operands\n");
		}
		else if((!strcmp(operation,"opcode")))
		{
			if((operand1[0]!='\0')&&(operand2[0]=='\0'))
			{
				if(!opcode(operand1))
				{
					put_history(buf);
				}
			}
			else {printf("\nERROR : Incorrect Operands\n");}
		}
		else if((!strcmp(operation,"opcodelist")))
		{
			if((operand1[0]=='\0'))
			{
				PrintAllHashData();
			}
			else {printf("\nERROR : Incorrect Operands\n");}
		}
		else if((!strcmp(operation,"type")))
		{
			if((operand1[0]!='\0'))
			{
				if(!type(operand1)) //when filename is valid
				{
					put_history(buf); //put it to history
				}
			}
			else {printf("\nERROR : Incorrect Operands\n");}
		}
		else if((!strcmp(operation,"assemble")))
		{
			if(operand1[0]!='\0')
			{
				assemble(operand1);
			}
			else {printf("\nERROR : Incorrect Operands\n");}
		}
		else if((!strcmp(operation,"symbol")))
		{
			if(operand1[0]=='\0')
			{
				symbol();
			}
			else {printf("\nERROR : Incorrect Operands\n");}
		}
		else if((!strcmp(operation,"progaddr")))
		{
			if((operand1[0]!='\0')&&(operand2[0]=='\0'))
			{
				progaddr(operand1);
			}
			else
			{
				printf("\nERROR : Incorrect Operands\n");
			}
		}
		else if((!strcmp(operation,"loader")))
		{
			//loader is different with the other functions.
			//because it's operands are not classify with comma(,)
			//instead of comma, it is classified with whitespace( )
			//so get buf_store and classify buf_store with whitespace.

			char *ptr = strtok(buf_store," ");
			char null_str[2]="*";
			strcpy(operation,ptr);
			ptr=strtok(NULL," ");
			strcpy(operand1,ptr);
			ptr=strtok(NULL," ");

			if(ptr==NULL) 
			{
				strcpy(operand2,null_str);
				strcpy(operand3,null_str);
			}
			else
			{
				strcpy(operand2,ptr);
				ptr=strtok(NULL," ");
				if(ptr!=NULL) strcpy(operand3,ptr);
				else strcpy(operand3,null_str);
			}

			if((operand1[0]!='\0')&&(!strcmp(operand2,"*"))&&(!strcmp(operand3,"*")))
			{
				//1 file input
				loader(operand1,operand2,operand3);
			}
			else if((operand1[0]!='\0')&&(operand2[0]!='\0')&&(strcmp(operand3,"*")))
			{
				//3 files input
				loader(operand1, operand2, operand3);
			}
			else if((operand1[0]!='\0')&&(operand2[0]!='\0')&&(!strcmp(operand3,"*")))
			{
				//2 files input
				loader(operand1,operand2, operand3);

			}
			else
			{
				printf("\nERROR : Incorrect Operands\n");
			}
		}
		else if(!strcmp(operation,"run"))
		{
			if(operand1[0]=='\0') run();
			else printf("ERROR : Incorrect Operands\n");
		}
		else if(!strcmp(operation,"bp"))
		{
			if(operand1[0]=='\0') print_bp();
			else if((operand1[0]!='\0')&&(operand2[0]=='\0'))
			{
				if(!strcmp(operand1,"clear"))
				{
					clear_bp();
				}
				else
				{
					bp(operand1);
				}
			}
			else printf("ERROR : Incorrect Operands\n");
		}
		else
		{
			printf("ERROR : Unvalid Operation\n");
		}
	}

	printf("\n");
	return 0;

}

int help()
{
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [star,end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");

	return 0;
}

int dir()
{
	//open files
	printf("\n");
	 DIR *dp;
    struct dirent *dir;
    struct stat st;
    if((dp=opendir("."))==NULL)
    {
       fprintf(stderr, "directory open error\n");
       exit(-1);
    }

    while((dir=readdir(dp))!=NULL)
    { //read files
       if(dir->d_ino ==0)
       {
          continue;
       }
       if(stat(dir->d_name,&st)==0)
       { //using stat function, check the type of files
          int dir_check = st.st_mode & S_IFMT;

          switch(dir_check)
          {
             case S_IFDIR:
                printf("%s/   ",dir->d_name);
                break;
             case S_IFREG:
					 //usting stat functiong, print '*' if it's executable file
                if(st.st_mode&S_IXUSR) printf("%s*  ",dir->d_name);
                else printf("%s   ",dir->d_name);
                break;
          }

       }

    }
	 closedir(dp);
	 printf("\n\n");
	 return 0;
}

/*history's data structure is linked list. 
  so follow the rule of making linked list*/
int put_history(char *buf)
{
	his_num++;
	if(first_order==0)
	{
		strcpy(head->data,buf);
		head->num=his_num;
		first_order=1;
		tail=head;
	}
	else
	{
		p=talloc();
		strcpy(p->data,buf);
		p->num=his_num;
		tail->link=p;
		tail=p;
	}
	tail->link=NULL;
	return 0;
}

/* in dump func, dump1 func and dump2 func

	1. program check the line number by rounddown func
		rounddown fuc will be make the line number using the rule
		that if there is a input number, find the most close 16 times number
		which is smaller than the input number.

	2. program change the input into proper number,
		becuase our memory is concist of 'Decimal Number'
		for example, if user inputs string "10(hex)"
			step 1 : make "10" into hex number with strtol func
						in here, "10" will be change as a integer variable.
			step 2 : make "integer variable (var = 10)" into decimal number
						with type casting.
						int here, "10" will be change into "16" as a decimal number.
	3. program check the error : 
			end is smaller than start or not
			start or end is out of range or not(0000~FFFF)
			value in the adrs is out of range in ASCII
				if program finds errors, prints ERROR massage.
	*/
int dump()
{
	int i;
	int line_start;
	int line_end;
	int err_flag=0;

	line_start=round_down(dump_pc);
	i=line_start;

	for(int t=0;t<10;t++) //print 10 lines with variable t
	{
		//i=dump_pc;
		//line_start=round_down(i);
		//i=line_start;
		line_end=line_start+16;
		printf("%05X ",line_start);
		for(;i<line_end;i++)
		{
			if(i>=ADRS_END) {printf(" "); err_flag=1;}
			else if(i<dump_pc) printf("   ");
			else printf("%02X ", mem[i]);
		}
		i=line_start;
		printf("; ");
		for(;i<line_end;i++)
		{
			if((i>=ADRS_END)||(mem[i]>126)||(mem[i]<=32)) printf(".");
			else printf("%c", (char)(mem[i])); //ASCII print
		}
		printf("\n");
		line_start=round_down(line_end);
		i=line_start;
	}
	dump_pc=line_end;
	if(err_flag)
	{
		printf("\n ERROR : Adrs is out of range\n");
	}
	return 0;
}

int dump_1(char* start)
{
	int i;
	int err_flag_rang=0;
	int line_end;
	char *ptr;
	long start_sth;
	start_sth = strtol(start, &ptr, 16);
	int start_int = (int)start_sth; //user input start
	int start_adrs = round_down((int)start_sth); //round_down start value
	if(start_int > ADRS_END) {printf ("\nERROR : start adrs is out of range\n"); return 1;}
	else
	{
		for(int t=0;t<10;t++)
		{
			i=start_adrs;
			line_end=start_adrs+16;
			printf("%05X " ,i);
			for(;i<line_end;i++)
			{
				if((i>=ADRS_END)||(i<0)) {printf("   "); err_flag_rang=1;}
				else if(i<start_int) printf("   ");
				else printf("%02X ",mem[i]);
			}
			i=start_adrs;
			line_end=start_adrs+16;
			printf("; ");
			for(;i<line_end;i++)
			{
				if((i>=ADRS_END)||(i<start_int)||(mem[i]>126)||(mem[i]<32)) printf(".");
				else printf("%c", (char)(mem[i]));
			}
			printf("\n");
			start_adrs=i;
		}
		dump_pc = line_end;
		if(err_flag_rang) {printf("\nERROR : end adrs is out of range\n");}
	}
	
	return 0;
}

int dump_2(char* start, char* end)
{
	int i;
	int line_end;
	char *ptr;
	long start_sth;
	long end_sth;
	start_sth = strtol(start, &ptr, 16);
	end_sth = strtol(end, &ptr, 16);
	int start_int = (int)start_sth;
	int end_int = (int)end_sth;
	int start_adrs = round_down((int)start_sth);
	int end_adrs = round_down((int)end_sth);
	dump_pc = end_int+1;
	printf("dump pc is %X\n",dump_pc);
	int err_flag_range=0;
	int line_num=0;

	if((start_int >= ADRS_END)||(start_int > end_int)) {printf("\nERROR : Out of Range\n"); return 1;}
	if((end_int > ADRS_END)) {end_int=ADRS_END; err_flag_range=1;}
	if((end_adrs==0)&&(start_adrs==0)) line_num=1;
	else {line_num = (((end_adrs-start_adrs)/16)+1);}

	for(int t=0;t<line_num;t++)
	{
		i=start_adrs;
		line_end=start_adrs+16;
		printf("%05X ",i);
		for(;i<line_end;i++)
		{
			if((i>=ADRS_END)||(i<start_int)||(i>end_int)) printf("   ");
			else printf("%02X ",mem[i]);
		}
		i=start_adrs;
		line_end=start_adrs+16;
		printf("; ");
		for(;i<line_end;i++)
		{
			if((i>=ADRS_END)||(i<start_int)||(i>end_int)||(mem[i]>126)||(mem[i]<32)) printf(".");
			else printf("%c", (char)(mem[i]));
		}
		printf("\n");
		start_adrs=i;
	}
	if(err_flag_range) {printf("\nERROR : Out of Range\n");}

	return 0;
}

/* In the every function related with memory,
	program change the input into proper number
	followed the fule described in the dump func. */

int edit(int adrs, int val)
{
	if((adrs >= ADRS_END)||(val<0)||(val>256)) 
	{
		printf("\nERROR : Out of Range\n"); 
		return 1;
	}
	else 
	{
		mem[adrs] = val;
	}
	return 0;	
}

int reset()
{
	for(int i=0;i<ADRS_END;i++)
		mem[i]=0;

	return 0;
}

int fill(char* start, char* end, char* val)
{
	char* ptr;
	long start_sth;
	long end_sth;
	long val_sth;
	start_sth=strtol(start, &ptr, 16);
	end_sth=strtol(end, &ptr, 16);
	val_sth=strtol(val, &ptr, 16);

	int start_index = (int)start_sth;
	int end_index = (int)end_sth;
	int val_int = (int)val_sth;

	if((start_index > end_index)||(start_index>ADRS_END)||(end_index)>ADRS_END||(val_int<0)||(val_int>256))
	{
		printf("\nERROR : Out of Range\n");
		return 1;
	}
	else {
		int i = start_index;
		for(;i<=end_sth;i++)
		{
			mem[i]=(int)(val_sth);
		}
	}
	return 0;
}

/* 
	in the get_txt function
		-open the txt file
		-scan line by line for store the operation and opcode
		-after the scanning, make the hashtable key using hashCode function
			because we have to use string(=operation name) as a hash key.
		-store the key, operation name, opcode in the hash table.
		-close the txt file.
*/
int get_txt()
{
	FILE *fp = fopen("opcode.txt", "r");
	if(fp == NULL) {printf("\nREAD FILE ERROR\n"); return 1;}
	else
	{
		char opcode[10];
		char op[20];
		char op_format[10];

		while(!feof(fp))
		{
			fscanf(fp,"%s %s %s", opcode, op, op_format);	
			int KEY = hashCode(op); //Change operation name as a integer
			HashTable* pHash = malloc(sizeof(HashTable));
			pHash->key = KEY; 
			strcpy(pHash->operation_name, op);
			strcpy(pHash->opcode, opcode);
			strcpy(pHash->format, op_format);
			AddHashData(pHash->key, pHash);
		}
		fclose(fp);
	}
	
	return 0;
}


int AddHashData(int key, HashTable* node)
{
	int hash_key = key%MAX_HASH; //Make key as a hash_key by deviding by MAX_HASH value.

	
	if(HT[hash_key] == NULL)
	{ //if HT[hash_key] has no node, put the input-node.
		HT[hash_key] = node;
	}
	else
	{ //if HT[hash_key] has nodes, link the input-node
		node->Next = HT[hash_key];
		HT[hash_key] = node;
	}
	return 0;
}


int opcode(char* mnemonic)
{
	//printf("len is %d\n",(int)strlen(mnemonic));
	char buf_mne[20];
	strcpy(buf_mne, mnemonic);
	int buf_key = hashCode(buf_mne);
	int KEY = buf_key%MAX_HASH;

	if(HT[KEY] == NULL)
	{
		//It means there is no such a mnemonic is hash table
		printf("\nYou put wrong mnemonic\n");
		return 1;
	}

	if(HT[KEY]->key == buf_key)
	{
		printf("opcode is %s\n", HT[KEY]->opcode);
	}
	else
	{
		HashTable* node = HT[KEY];
		while(node->Next)
		{
			node = node->Next;
			if((node->key) == buf_key)
			
			{
				printf("opcode is %s\n", node->opcode);
				return 0;
			}
		}
	}
	return 0;
}

int PrintAllHashData()
{
	for(int i=0;i<MAX_HASH;i++)
	{
		printf("%d :",i);
		if(HT[i]!=NULL)
		{
			HashTable* nodebuf = HT[i];

			printf("[%s,%s,%s]",nodebuf->operation_name,nodebuf->opcode,nodebuf->format);
			
			while(nodebuf->Next)
			{
				printf("->");
				nodebuf=nodebuf->Next;
				printf("[%s,%s,%s]",nodebuf->operation_name,nodebuf->opcode,nodebuf->format);
			}
		}
		else printf("NULL");
		printf("\n");
	}
	return 0;
}

//round_down func is described in the dump fuction
int round_down(int number)
{
	while(number%16)
	{
		number--;
	}
	return number;
}

/*
	hashCode func is for making string into integer number.
	Using the polynomial h(s)=mk-1*ak-1*xk-2*ak-2+...+x0*a0
	where a is prime number.
*/
int hashCode(char* str)
{
	int i;
	int r=0;
	int a=33;

	for(i=0;i<(int)(strlen(str));i++)
	{
		r+=r*a+str[i];
	}
	if(r<0) r=-r;
	return r;
}

int type(char* filename)
{
	FILE *fp = fopen(filename,"r");
	char strTemp[255];
	if(fp==NULL)
	{
		printf("\nERROR:File Open Fail\n");
		return 1;
	}
	else
	{
		while(!feof(fp))
		{
			fgets(strTemp,sizeof(strTemp),fp);
			printf("%s",strTemp);
			strTemp[0]='\0';
		}
		fclose(fp);
	}
	return 0;
}

int assemble(char* filename)
{

	int line_number=0;

	//initiating variables
	first_loc=0;
	LOC_START=0;
	post_loc=0;
	start_flag=0;
	first_ob=1;
	BASE=0;
	obj_code[0]='\0';
	PROGRAM_NAME[0]='\0';
	START_PROGRAM=0;
	END_PROGRAM=0;
	pass2_loc_p = loc_head;

	//make .lst file
	char file_buf[50];
	strcpy(file_buf, filename);
	char* lst_p = strtok(file_buf,".");
	char* lst = ".lst";
	strcat(lst_p,lst);
	FILE *ffp = fopen(lst_p,"w");

	if(ffp==NULL)
	{
		printf("\nERROR: .lst File Make Fail\n");
		return 1;
	}
	for(int pass=1;pass<3;pass++)
	{
		pLOCTAB pass2_now_loc = loc_head; 
		FILE *fp = fopen(filename,"r");
		char strTemp[255];
		if(fp==NULL)	
		{
			printf("\nERROR:File Open Fail\n");
			return 1;
		}
		else
		{
			
			//sometimes this part make some error. in that case, input MAXLEN value more bigger.
			char symbol[MAXLEN];
			char operation[MAXLEN];
			char operand1[MAXLEN];
			char operand2[MAXLEN]={0,};
			/*basic logic is very similar with classification logic in main function
			  get buffer and using strtok, classify operation-operand1-operand2.
			  the difference with the classification logic in main function is
			  there is a symbol classification in here.
			  */
			while(!feof(fp))
			{
				//initiating
				start_flag=0;
				int symbol_adrs=0;
				int index=0;
				int op_count=0;
				int operation_count=0;
				int operand1_count=0;
				int operand2_count=0;
				int comma_flag=0;
				strTemp[0]='\0';
				symbol[0]='\0';
				operation[0]='\0';
				operand1[0]='\0';
				operand2[0]='\0';
				char char_buf[10];
				char_buf[0]='\0';
				fgets(strTemp,sizeof(strTemp),fp);
				char strbuf[255];
				strbuf[0]='\0';
				strcpy(strbuf,strTemp);
				int buflen = strlen(strbuf);
				strbuf[buflen-1]='\0';

				//classification for no symbol line
				if((strTemp[0]== ' ')||(strTemp[0]=='\t'))
				{
					while(strTemp[index]!='\0')
					{
						while((strTemp[index]==' ')||(strTemp[index]=='\t')) 
						{
							index++;
						}
						if(op_count==0)
						{
							if(strTemp[index]=='\0');
							else if((strTemp[index]==' ')||(strTemp[index]=='\t'));
							else
							{
								char_buf[0]=strTemp[index];
								strcat(operation,char_buf);
								operation_count=1;
							}

							if((operation_count==1)&&((strTemp[index+1]==' ')||(strTemp[index+1]=='\t')))
							{
								op_count++;
								int len = strlen(operation);
								operation[len]='\0';
							}
						}
						else if(op_count==1)
						{
							if(strTemp[index]=='\0');
							else if(strTemp[index]==',')comma_flag=1;
							else if((strTemp[index]==' ')||(strTemp[index]=='\t'));
							else
							{
								char_buf[0]=strTemp[index];
								strcat(operand1, char_buf);
								operand1_count=1;
							}
							if(((strTemp[index+1]!=' ')||(strTemp[index+1]!='\t'))&&comma_flag) 
							{
								op_count++;
								int len = strlen(operand1);
								operand1[len] = '\0';
							}
						}
						else if((op_count==2)||(comma_flag==1))
						{
							if(strTemp[index]=='\0');
							else if((strTemp[index]==' ')||(strTemp[index]=='\t'));
							else
							{
								char_buf[0]=strTemp[index];
								strcat(operand2,char_buf);
								operand2_count=1;
							}
						}
						index++;
					}

					//delete the '\n' character at the end of variables
					if((operation_count==1)&&(operand1_count!=1)&&(operand2_count!=1))
					{
						int len = strlen(operation);
						operation[len-1]='\0';
					}
					else if((operation_count==1)&&(operand1_count==1)&&(operand2_count!=1))
					{
						int len = strlen(operand1);
						operand1[len-1]='\0';
					}
					else if((operation_count==1)&&(operand1_count==1)&&(operand2_count==1))
					{
						int len=strlen(operand2);
						operand2[len-1]='\0';
					}

					/*-------------------------
							PASS 1 START
					-------------------------*/
					if(pass==1)
					{
						line_number+=5;
						if(!strcmp(operation,"END")) 
						{
							//if operatin is END, quit.
							break;
						}

						/*
							result value gets the type of operation.
							if it is format 4, check_operation returns 4.
							if it is format 3, returns 3.
							if it is format 2, returns 2.
							if it is format 1, returns 1.
							if it is directive or comment, returns 0.
							if it is variable, returns proper number for adding loctaion counter.

							if result value is 0, put LOCTAB value -1. 
							then when program prints LOCTAB, if LOCTAB value is negative number,
							prints tab('\t').

							in the put_LOCTAB function,
							using input value(result_value), add that to post location counter and
							stores the location counter.
						*/
						int result_value = check_operation(operation);
						if(result_value==0) {put_LOCTAB(-1);} //directives
						else
						{
							put_LOCTAB(result_value);
						}
					}
					else if(pass==2)
					{
						/*
							it is pass 2.

							if operation is BASE, put the adrs(operand1 value) to the
							BASE, the global variable.

							if operation is END, close the files.
							and using the LOCTAB and OBTAB made in the assemble passes,
							make an object file with make_obj function.

							else, fprintf the loctab and object code to ffp(.lst file)
						*/
						if(!strcmp(operation,"BASE"))
						{
							int adrs = get_symbol_adrs(operand1);
							BASE = adrs;
						}
						if(!strcmp(operation,"END")) 
						{
							fclose(fp);
							fclose(ffp);
							make_obj(file_buf);
							return 0;
						}
						else
						{
							//fprintf the line number
							line_number+=5;
							fprintf(ffp,"%d\t",line_number); 

							//get object code and now location counter
							char* now_ob = get_objcode(operation, operand1, operand2);
							int now_loc = pass2_now_loc->LOC;

							//fprintf the location counter and object code.
							
							//if now_loc is negative, don't print the loctaion counter.
							//it means it's directives or comments witch doesn't make LOC.
							if(now_loc<0) fprintf(ffp,"    \t");
							else fprintf(ffp,"%04X\t",now_loc);

							//fprintf the whole line with strbuf
							fprintf(ffp,"%s\t",strbuf);

							//if now_ob is "*", it means that it's comments or directives.
							if(!strcmp(now_ob,"*")) fprintf(ffp,"\n");
							else fprintf(ffp,"%s\n",now_ob);

							//pass2_now_loc is variable for now viewing loctaion counter
							pass2_now_loc = pass2_now_loc->Next;
						}
					}
					
				}
				else if(strTemp[0]=='.')
				{
					//if it is comments
					if(pass==1)
					{
						line_number+=5;
						put_LOCTAB(-1);
					}
					else if(pass==2)
					{ 
						line_number+=5;
						fprintf(ffp,"%d\t",line_number);

						char no_obj[10]="*";
						put_OBTAB(no_obj);

						int now_loc = pass2_now_loc->LOC;
						if(now_loc<0) fprintf(ffp,"    \t");
						else fprintf(ffp,"%04X\t",now_loc);
					
						fprintf(ffp,"%s\n",strbuf);
						pass2_now_loc = pass2_now_loc->Next;
					}
				}
				else
				{
					//if that line has the symbol

					//classification for symbol line
					char* tok1=strtok(strTemp," ");
					int i=0;
					while(tok1!=NULL)
					{
						if(i==0) {strcpy(symbol,tok1);}
						else if(i==1) {strcpy(operation,tok1);}
						else if(i==2) {strcpy(operand1,tok1);}
						else if(i==3) {strcpy(operand2,tok1);}
						tok1 = strtok(NULL," ");
						i++;
					}

					if((operand2[0]=='\0')&&(operand1[0]!='\0'))
					{
						int len = strlen(operand1);
						operand1[len-1]='\0';
					}
					else if((operand1[0]=='\0')&&(operand2[0]=='\0'))
					{
						int len = strlen(operation);
						operation[len-1]='\0';
					}
					else if((operand1[0]!='\0')&&(operand2[0]!='\0'))
					{
						int len = strlen(operand2);
						operand2[len-1]='\0';
					}	

					if(symbol[0]=='\0'); //if there is no symbol, do nothing.
					else
					{
						if(pass==1)
						{
							//check it is first line or not.

							//if it is first line, check it is START or not.
							//if it is START, put the adrs(operand1) to START global variable,
							//and put the program name in PROGRAM NAME.

							//if not, put 0 to START global variable and
							//make the PROGRAM NAME 'NONAME'
							if(first_loc==0)
							{
								if(!strcmp(operation,"START"))
								{
									start_flag=1;

									//make the operand1 string to integer number using strtol.
									char* ptr;
									long val_hex = strtol(operand1,&ptr,10);
									symbol_adrs=put_LOCTAB(val_hex);

									strcpy(PROGRAM_NAME,symbol);
									START_PROGRAM=(int)val_hex;
								}
								else
								{
									char prog_name[30] = "NONAME";
									strcpy(PROGRAM_NAME,prog_name);
									START_PROGRAM=0;
								}
							}
							else
							{
								//if it is not first line
								if((!strcmp(operation,"START"))&&(start_flag=1))
								{
									//not first line but START operation means ERROR.
									printf("%d line : ERROR, too many START\n",line_number);
								}
								line_number+=5;

								int str_size=0; //for making variable loc
								if((!strcmp(operation,"BYTE")))
								{
									//if operation is BYTE
									//check C and X
									//if BYTE operand doesn't start with C' or X', ERROR.

									int operand_size = strlen(operand1);
									if((operand1[0]=='C')&&((int)operand1[1]==39))
									{
										// 3 includes C' '\n
										str_size = operand_size-3;
									}
									else if((operand1[0]=='X')&&((int)operand1[1]==39))
									{
										// 3 includes X' '\n
										str_size = operand_size-3;
										if(!(str_size%2)) str_size += 1;
										str_size = str_size/2;
									}
									else
									{
										printf("line %d: ERROR\n",line_number);
									}
									symbol_adrs=put_LOCTAB(str_size);
								}
								else if(!strcmp(operation,"WORD"))
								{
									symbol_adrs=put_LOCTAB(3);
								}
								else if(!strcmp(operation,"RESW"))
								{
									char* ptr;
									int op_int=(int)strtol(operand1,&ptr,10);
									symbol_adrs=put_LOCTAB(op_int*3);
								}
								else if(!strcmp(operation,"RESB"))
								{
									char* ptr;
									int op_int=(int)strtol(operand1,&ptr,10);
									symbol_adrs=put_LOCTAB(op_int);
								}
								else
								{
									int result=check_operation(operation);
									if(result==0) {symbol_adrs=put_LOCTAB(-1);}
									else symbol_adrs=put_LOCTAB(result);
								}
								put_SYMTAB(symbol,symbol_adrs);
							}
						}//pass1 end
						else if(pass==2)
						{	
							if(!strcmp(operation,"END"))
							{
								fclose(fp);
								fclose(ffp);
								make_obj(file_buf);
								return 0;
							}
							else
							{
								line_number += 5;
								fprintf(ffp,"%d\t",line_number);

								//get object code
								//if now_ob is "ERROR", it means error.
								char* now_ob = get_objcode(operation, operand1, operand2);
								if(!strcmp(now_ob,"ERROR"))
								{
									printf("line %d:ERROR in pass 2\n",line_number);
								}
								int now_loc = pass2_now_loc->LOC;

								if(now_loc<0) fprintf(ffp,"    \t");
								else fprintf(ffp,"%04X\t",now_loc);
								
								fprintf(ffp,"%s\t",strbuf);
								if(!strcmp(now_ob,"*")) fprintf(ffp,"\n");
								else fprintf(ffp,"%s\n",now_ob);

								pass2_now_loc = pass2_now_loc->Next;
							}
						}						
					}
					//symbol check (else) end
				}//symbol operation check (else) end
			}//while(!feof(fp))end
		}//fp is not null if else end
		fclose(fp);
	}//passes end
	fclose(ffp);
	
	return 0;
}

int put_LOCTAB(int loc)
{
	int now_loc=post_loc;
	if(first_loc==0) //if it is first line, check it's START or not
	{
		loc_head->LOC=post_loc;
		if(start_flag==1);
		else 
		{
			post_loc=post_loc+loc;
		}
		first_loc=1;
		loc_tail=loc_head;
	}
	else //it is not first line
	{	
		loc_p=loc_talloc();
		if(loc < 0) {loc_p->LOC=loc;} //directives, comments
		else
		{
			loc_p->LOC=post_loc;
			post_loc=loc+post_loc;
		}
			loc_tail->Next=loc_p;
			loc_tail=loc_p;
	}
	loc_tail->Next=NULL;
	END_PROGRAM = post_loc;
	return now_loc;
}

int put_SYMTAB(char* symbol, int adrs)
{
	//it is for sorted symbol table. sorted symbol is linked list.
	put_sorted_symbol(symbol,adrs);

	int KEY=hashCode(symbol);
	SYMTAB* pSym = malloc(sizeof(SYMTAB));
	pSym->key = KEY;
	strcpy(pSym->symbol,symbol);
	pSym->adrs=adrs;

	int hash_key=KEY%MAX_HASH;
	if(ST[hash_key]==NULL)
	{
		ST[hash_key]=pSym;
	}
	else
	{
		pSym->Next = ST[hash_key];
		ST[hash_key]=pSym;
	}
	return 0;
}


int put_sorted_symbol(char* symbol, int adrs)
{
	//it is for making sorted symbol linked list.

	sorted_symbol* newnode;
	sorted_symbol* cur;
	newnode=sym_talloc();
	newnode->link=NULL;
	strcpy(newnode->symbol,symbol);
	newnode->adrs=adrs;
	cur=ss_head;

	if(ss_head==NULL) 
	{
		ss_head=newnode;
		return 0;
	}
	else if(strcmp((ss_head->symbol),(newnode->symbol))==-1)
	{
		//if ss_head->symbol is smaller than newnode->symbol in ASCII code value, replace.
		newnode->link=ss_head;
		ss_head=newnode;
		return 0;
	}
	else
	{
		while(cur->link!=NULL)
		{
			if(strcmp(((cur->link)->symbol),(newnode->symbol))==1)
			{
				//if cur->link->symbol is bigger than newnode->symbol, replace.
				newnode->link=cur->link;
				cur->link=newnode;
				return 0;
			}
			cur=cur->link;
		}
	}
	cur->link=newnode;
	return 0;
}

int print_LOCTAB()
{
	//in real program, it's not used. it is for test.
	int line_num=0;
	loc_p=loc_head;
	while(loc_p!=NULL)
	{
		line_num+=5;
		printf("%d line:",line_num);
		if(loc_p->LOC < 0) printf("\t\n");
		else printf("%04X\n",loc_p->LOC);
		loc_p=loc_p->Next;
	}
	return 0;
}

int symbol()
{
	sorted_symbol* cur;
	cur=ss_head;
	while(cur->link!=NULL)
	{
		printf("\t%s\t%04X\n",cur->symbol,cur->adrs);
		cur=cur->link;
	} 
	printf("\t%s\t%04X",cur->symbol,cur->adrs);

	/*
	>>it is old version symbol function. it will be leave for testing.<<
	for(int i=0;i<MAX_HASH;i++)
	{
		if(ST[i]!=NULL)
		{
			SYMTAB* nodebuf = ST[i];
			printf("\n\t%s\t%04X",nodebuf->symbol,nodebuf->adrs);
			while(nodebuf->Next)
			{
				nodebuf=nodebuf->Next;
				printf("\n\t%s\t%04X",nodebuf->symbol,nodebuf->adrs);
				//if((nodebuf->Next)||(i<MAX_HASH)) printf("\n");
			}
		}
	}*/
	return 0;
}

int check_operation(char* operation)
{
	if(is_directive(operation))
	{
		//it is assembler directive
		return 0;
	}
	
	if(operation[0] == '+')
	{
		return 4; //format 4
	}
	
	int format = is_format(operation); //for classify formats of operation

	if(format != 0)
	{
		if(format == 1)
		{
			return 1;
		}
		else if(format == 2)
		{
			return 2;
		}
		else
		{
			return 3;
		}
	}
	else
	{
		printf("\n\nERROR in checking format\n\n");
		return -1;
	}
}

int is_directive(char* operation)
{
			
	char directive[2][10] = {"END", "BASE"};
	for(int i=0;i<2;i++)
	{
		if(!strcmp(operation, directive[i]))
		{
			return 1;
		}
	}

	return 0;
}

int is_no_obj(char* op)
{
	char directive[6][20] = {"START", "END", "BASE", "RESW", "RESB", "NOBASE"};
	for(int i=0;i<6;i++)
	{
		if(!strcmp(op, directive[i])) return 1;
	}
	return 0;
}

int is_variable(char* op)
{
	char directive[2][10] = {"BYTE", "WORD"};
	for (int i=0;i<2;i++)
	{
		if(!strcmp(op, directive[i])) return 1;
	}
	return 0;
}

int is_format(char* operation)
{

	int buf_key=hashCode(operation);
	int KEY = buf_key%MAX_HASH;

	if(HT[KEY] == NULL)
	{
		return 0;
	}
	if(HT[KEY]->key == buf_key)
	{
		char* ptr;
		long val_long = strtol(HT[KEY]->format, &ptr, 10);
		return (int)val_long; //if it is 3/4 it gonna be 3
	}
	else
	{
		HashTable* node = HT[KEY];
		while(node->Next)
		{
			node = node->Next;
			if((node->key)==buf_key)
			{
				char* ptr;
				long val_long = strtol(node->format, &ptr, 10);
				return (int)val_long;
			}
		}
	}
	return 0;
}

int get_opcode(char* op)
{
	char op_buf[30];
	strcpy(op_buf,op);
	int buf_key=hashCode(op_buf);
	int KEY = buf_key%MAX_HASH;
	if(HT[KEY]==NULL)
	{
		return 0;
	}
	if(HT[KEY]->key==buf_key)
	{
		char* ptr;
		long val_long = strtol(HT[KEY]->opcode, &ptr, 16);
		return (int)val_long;
	}
	else
	{
		HashTable* node=HT[KEY];
		while(node->Next)
		{
			node=node->Next;
			if((node->key)==buf_key)
			{
				char* ptr;
				long val_long = strtol(node->opcode, &ptr, 16);
				return (int)val_long;
			}
		}
	}
	return 0;
}

int get_symbol_adrs(char* symbol)
{
	int len = strlen(symbol);
	symbol[len]='\0';

	int buf_key=hashCode(symbol);
	int KEY=buf_key%MAX_HASH;
	if(ST[KEY]==NULL) return -1; //Error : No such symbol
	if(ST[KEY]->key==buf_key)
	{
		return ST[KEY]->adrs;
	}
	else
	{
		SYMTAB* node=ST[KEY];
		while(node->Next)
		{
			node=node->Next;
			if((node->key)==buf_key)
			{
				return node->adrs;
			}
		}
	}
	return 0;
}

int get_pc()
{
	if(pass2_loc_p->Next != NULL)
	{
		pass2_loc_p = pass2_loc_p->Next;

		while(pass2_loc_p->LOC <0) //if LOC is negative, it means its LOC is null. (directives)
		{
			pass2_loc_p = pass2_loc_p->Next;
		}
		return pass2_loc_p->LOC;
	}
	else return -1; //ERROR
}

char* get_objcode(char* operation_buf, char* op1_buf, char* op2_buf)
{
	//initiating
	int ni=0;
	int xbpe=0;
	int adrs=0;
	int disp=0;
	char op1[30]="";
	char op2[100]="";
	strcpy(op2, op2_buf);
	char operation[30]="";
	int pc;	
	char init[255]="";
	strcpy(obj_code,init);
	int format4_flag=0;
	
	if((!(is_no_obj(operation_buf)))&&(!(is_variable(operation_buf))))
		{//if not directive so make object code

			pc=get_pc();

			if((is_format(operation_buf)==3)||(operation_buf[0]=='+'))
			{
	
				//make ni code
				if((op1_buf[0]=='#')||(op1_buf[0]=='@'))
				{
					if(op1_buf[0]=='#')
					{
						ni=1;
					}
					else if(op1_buf[0]=='@')
					{
						ni=2;
					}
					
					//delete # @ sign
					int len=strlen(op1_buf);
					char buf[10]="";
					for(int i=1;i<len;i++)
					{
						buf[0]=op1_buf[i];
						strcat(op1,buf);
					}
					//delete # @ sign end
				}
				else
				{
					if(op1_buf[0]=='\0')
					{
						ni=3;
					}
					else
					{
						ni=3;
						strcpy(op1,op1_buf);
					}
					
				}


				//make xbpe code
	
				//check x
				if((op1[0]=='X')||(op2[0]=='X'))
				{
					xbpe+=8;
				}
				
				//check b or p
				if(ni==1)
				{
					if(isdigit(op1[0]))
					{
						char* ptr;
						long val_sth;
						val_sth=strtol(op1,&ptr,10);
						adrs=(int)val_sth;
						disp=adrs;
						//if digit and immediate adrsing(ex. #3),
						//disp=that value(3). it is not displacement. just name of value.
					}
					else
					{
						adrs=get_symbol_adrs(op1);

						//check it is base-relative or pc-relative
						//-2048~2047 is range of pc-relative in decimal number
						if(((adrs-pc>2047)||(adrs-pc)<(-2048)))
						{
							disp=adrs-BASE;
							xbpe+=4;
						}
						else
						{
							disp=adrs-pc;
							xbpe+=2;
						}
						
					}
					//if ni==1 check it is digit or not
					//if digit, put it immediately
					//if not, use that symbol adrs as a adrs
					//else check base-relative or pc-relative
				}
				else
				{
					if(op1_buf[0]=='\0')
					{
						disp=0;
					}
					else
					{
						adrs=get_symbol_adrs(op1);
						if((adrs-pc>2047)||(adrs-pc)<(-800))
						{
							disp = adrs-BASE;
							xbpe+=4;
						}
						else
						{
							disp = adrs-pc;
							xbpe+=2;
						}
					}
				}

				//check e (format 4)
				if(operation_buf[0]=='+')
				{
					format4_flag=1;

					int len=strlen(operation_buf);
					char buf[10]="";
					for(int i=1;i<len;i++)
					{
						buf[0]=operation_buf[i];
						strcat(operation,buf);
					}
					len=strlen(operation);
					operation[len]='\0';
					xbpe=1; //if format 4, no p and b.
					if(ni!=1) disp = get_symbol_adrs(op1);
				}
				else
				{
					strcpy(operation,operation_buf);
				}
				//	format4, xbpe+=1

				//make object code
				char hex_buf[30];
						
				int opcode=get_opcode(operation); //get operation's opcode in decimal
	
				opcode = opcode + ni;
				itoa(opcode, hex_buf, 16);

				//check len and put zero sign
				int hexlen = strlen(hex_buf);
				if(hexlen < 2)
				{
					char buf[1];
					buf[0]='0';
					strcat(obj_code, buf);
					strcat(obj_code, hex_buf);
				}
				else if(hexlen==2)
				{
					strcat(obj_code, hex_buf);
				}

				//initiating
				hex_buf[0] = '\0';

				//if xbpe=0, put zero sign at object code.
				//else, put xbpe string to object code.
				if(xbpe == 0)
				{
					char zero[2]="0";
					strcat(obj_code,zero);
				}
				else
				{
					sprintf(hex_buf,"%X",xbpe);
					strcat(obj_code,hex_buf);
				}

				hex_buf[0]='\0';
				int len_disp;
				
				//if displacement is not negative
				if(disp>=0)
				{
					if(disp==0) 
					{
						char* zero="0";
						strcpy(hex_buf,zero);
						len_disp=strlen(hex_buf);
					}
					else
					{
						itoa(disp, hex_buf, 16);
						len_disp = strlen(hex_buf);
					}
				}
				else
				{	 //if displacement is negative, make it 2's complement
					sprintf(hex_buf, "%X", disp);
					len_disp=strlen(hex_buf);
				}
	
				//if format 4
				if(format4_flag==1)
				{
					//if len is not proper, put it zero sign
					if(len_disp!=5)
					{
						if(len_disp<5)
						{
							for(int i=0;i<(5-len_disp);i++)
							{
								char zero[1]="0";
								strcat(obj_code,zero);
							}
							strcat(obj_code,hex_buf);
						}
						else if(len_disp>5)
						{
							for(int i=(len_disp-5);i<len_disp;i++)
							{
								char buf[1];
								buf[0]=hex_buf[i];
								strcat(obj_code,buf);
							}
						}
					}
					else if(len_disp==5) {strcat(obj_code,hex_buf);} 
				}
				else
				{
					//if it is not format 4
					if(len_disp!=3)
					{
						if(len_disp<3)
						{
							for(int i=0;i<(3-len_disp);i++)
							{
								char zero[2]="0";
								strcat(obj_code,zero);
							}
							strcat(obj_code,hex_buf);
						}
						else if(len_disp>3)
						{
							for(int i=(len_disp-3);i<len_disp;i++)
							{
								char buf[2]="";
								buf[0]=hex_buf[i];
								strcat(obj_code,buf);
							}
						}
					}
					else if(len_disp==3) {strcat(obj_code,hex_buf);}
				}
				put_OBTAB(obj_code);
				first_ob++;
				return obj_code;
				//format 3 : opcode+ni / xbpe / disp-in-12bits(000) => 24bits(3bytes) obj code
				//format 4 : opcode+ni / xbpe / disp-in-20bits(00000) => 32bits(4bytes) obj code
			}
			else if(is_format(operation_buf)==1)
			{
				//if format is 1, just put that opcode to object code

				int opcode = get_opcode(operation_buf);
				char hex_opcode[30];
				itoa(opcode, hex_opcode, 16);
				int len=strlen(hex_opcode);
				if(len<2) 
				{
					char buf[2]="";
					buf[0]='0';
					strcat(obj_code,buf);
				}
				strcat(obj_code,hex_opcode);
				put_OBTAB(obj_code);
				first_ob++;
				return obj_code;
			}
			else if(is_format(operation_buf)==2)
			{
				//if it is format2, check the register number
				//if register is null, put 0
				int reg1;
				int reg2;
				if(op1_buf[0]!='\0')
				{
					reg1 = is_reg(op1_buf);
					if(reg1 != -1)
					{
						reg1 = is_reg(op1_buf);
					}
				}
				else
				{
					reg1 = 0;
				}
	
				if(op2_buf[0]!='\0')
				{
					reg2 = is_reg(op2_buf);
					if(reg2 != -1)
					{
						reg2 = is_reg(op2_buf);
					}
				}
				else reg2=0;
				
				int opcode = get_opcode(operation_buf);
				char hex_opcode[20];
				itoa(opcode, hex_opcode, 16);

				//if it is unproper len, put zero sign or delete first characters
				int len=strlen(hex_opcode);
				if(len<2) 
				{
					char buf[2]="0";
					strcat(obj_code,buf);
				}
				strcat(obj_code,hex_opcode);

				char buf[2]="";
				sprintf(buf,"%d",reg1);
				strcat(obj_code,buf);

				char buf2[2]="";
				sprintf(buf2,"%d",reg2);
				strcat(obj_code,buf2);

				put_OBTAB(obj_code);
				first_ob++;
				return obj_code;
			}
			else
			{
				//error
				strcpy(ERROR,"ERROR");
				return ERROR;
			}
		}
		else if((!(is_no_obj(operation_buf)))&&(is_variable(operation_buf)))
		{
			pc=get_pc();
			//if it is variable directive

			if(!strcmp(operation_buf,"BYTE"))
			{
				if(op1_buf[0] == 'C')
				{
					int i=2;
					int j=0;
					char buf[100]="";
					while((int)(op1_buf[i]) != 39)
					{
						buf[j]=op1_buf[i];
						i++;
						j++;
					}
					int len = strlen(buf);
					for(int i=0;i<len;i++)
					{
						char charbuf[20]="";
						int bufbuf;
						bufbuf = (int)buf[i];
						itoa(bufbuf, charbuf, 16);
						strcat(obj_code,charbuf);
					}
				}
				else if (op1_buf[0] == 'X')
				{
					int i=2;
					int j=0;
					char buf[100]="";
					while((int)(op1_buf[i])!=39)
					{
						buf[j] = op1_buf[i];
						i++;
						j++;
					}
					strcat(obj_code,buf);
				}
			}
			else if(!strcmp(operation_buf,"WORD"))
			{
				strcat(obj_code,op1_buf);
			}
			put_OBTAB(obj_code);
			first_ob++;
			return obj_code;
		}
		else if(is_no_obj(operation_buf))
		{
			if(is_directive(operation_buf));
			else
			{
				get_pc();
			}

			//if it is directive which doesn't make a object code
			char no_obj[10]="*";
			strcpy(obj_code,no_obj);
			put_OBTAB(no_obj);
			first_ob++;
			return obj_code;
		}
		else
		{
			//ERROR
			strcpy(ERROR,"ERROR");
			return ERROR;
		}
		return 0;
}

int is_reg(char* reg)
{
	if(!strcmp(reg,"A")) return 0;
	else if(!strcmp(reg,"X")) return 1;
	else if(!strcmp(reg,"L")) return 2;
	else if(!strcmp(reg,"PC")) return 8;
	else if(!strcmp(reg,"SW")) return 9;
	else if(!strcmp(reg,"B")) return 3;
	else if(!strcmp(reg,"S")) return 4;
	else if(!strcmp(reg,"T")) return 5;
	else if(!strcmp(reg,"F")) return 6;
	else return -1;
}

void put_OBTAB(char* obj)
{
	if(first_ob==1)
	{
		strcpy(ob_head->obj,obj);
		ob_tail=ob_head;
	}
	else
	{
		ob_p=ob_talloc();
		strcpy(ob_p->obj,obj);
		ob_tail->Next=ob_p;
		ob_tail=ob_p;
	}
	ob_tail->Next=NULL;
	return;
}

int print_OBTAB()
{
	//it is for test. real program doesn't use it
	int line_num=0;
	ob_p=ob_head;
	while(ob_p!=NULL)
	{
		line_num+=5;
		printf("%d line: %s\n",line_num, ob_p->obj);
		ob_p=ob_p->Next;		
	}
	return 0;
}

void make_obj(char* filename)
{
	pOBTAB ob_pointer = ob_head;
	pLOCTAB loc_pointer = loc_head;
	
	char obj_buf[255];
	char* obj = ".obj";
	char* file = strtok(filename,".");
	strcat(file,obj);
	FILE* fp = fopen(file,"w");
	if(fp==NULL)
	{
		printf("ERROR in make obj file\n");
		return;
	}
	else
	{
		fprintf(fp,"H");
		fprintf(fp,"%-6s",PROGRAM_NAME);
		fprintf(fp,"%06X",START_PROGRAM);
		fprintf(fp,"%06X",END_PROGRAM);
		fprintf(fp,"\n");
		
		loc_pointer = loc_pointer->Next;
		ob_pointer = ob_pointer->Next;

		while(loc_pointer->Next != NULL)
		{
			int pre_loc = loc_pointer->LOC;
			int now_start_loc = loc_pointer->LOC;

			while(1)
			{
				if((pre_loc-now_start_loc) > 30) 
				{
				//cut the line
					break;
				}
				else
				{
					if((loc_pointer->Next)==NULL)
					{
						pre_loc = (loc_pointer->LOC);
					}
					else
					{
						pre_loc = (loc_pointer->Next)->LOC;
					}

					if((!(strcmp(ob_pointer->obj,"*")))&&(loc_pointer->LOC<0)) //directive
					{
						loc_pointer = loc_pointer->Next;
						ob_pointer = ob_pointer->Next;
					}
					else if((!(strcmp(ob_pointer->obj,"*")))&&(loc_pointer->LOC>=0)) //variable
					{
						loc_pointer = loc_pointer->Next;
						ob_pointer = ob_pointer->Next;
						break;
					}
					else if((loc_pointer->Next)==NULL) 
					{ break;}
					else
					{
						//normal
						strcat(obj_buf,ob_pointer->obj);
						ob_pointer = ob_pointer->Next;
						loc_pointer = loc_pointer->Next;
					}
				}
			}

			int len = (loc_pointer->LOC) - now_start_loc;

			if(obj_buf[0]=='\0');
			else
			{
				fprintf(fp,"T");
				fprintf(fp,"%06X",now_start_loc);
				fprintf(fp,"%02X",len);
				fprintf(fp,"%s\n",obj_buf);
			}
			char init[255] = "";
			strcpy(obj_buf,init);
			if(loc_pointer->LOC==END_PROGRAM) break;
		}
		fprintf(fp,"E");
		fprintf(fp,"%06X\n",END_PROGRAM);
	}
	fclose(fp);
	return;
}

//project3

void put_ESTAB(char* cs_name, char* sym_name, int adrs, int len)
{
	//make new estab node
	pESTAB new_node;
	new_node = es_talloc();
	strcpy(new_node->cs_name,cs_name);
	strcpy(new_node->sym_name,sym_name);
	new_node->adrs = adrs;
	new_node->len = len;
	
	es_tail=es_talloc();
	if(es_head == NULL) es_head=new_node;
	else
	{
		es_tail=es_head;
		while(!(es_tail->Next==NULL))
		{
			es_tail = es_tail->Next;
		}
		es_tail->Next = new_node;
	}
	new_node=NULL; 
	free(new_node);
	return;
}

void print_ESTAB()
{
	printf("-----------ESTAB------------\n");
	printf("CS\tSYM\tadrs\tlen\n");
	printf("----------------------------\n");
	if(es_head == NULL)
	{
		printf("ESTAB is null\n");
		return;
	}
	pESTAB new_es_p = es_talloc();
	new_es_p = es_head->Next;
	while(new_es_p!=NULL)
	{
		if(!strcmp(new_es_p->cs_name,"*")) printf("\t");
		else printf("%s\t",new_es_p->cs_name);

		if(!strcmp(new_es_p->sym_name,"*")) printf("\t");
		else printf("%s\t",new_es_p->sym_name);

		printf("%04X\t",new_es_p->adrs);

		if(new_es_p->len == 0) printf("\n");
		else 
		{
			printf("%04X\n",new_es_p->len);
			PROGRAM_END = new_es_p->adrs + new_es_p->len;
		}
		new_es_p = new_es_p->Next;
	}
	new_es_p = NULL;
	free(new_es_p);
	printf("----------------------------\n");
	printf("\ttotal length\t%04X\n",total_len);

	return;
}

/*
		search_ESTAB function

		-if user wants to find constrol section adrs,
		 put "*" in sym_name operand part
		-else if user wants to fine symbol adrs,
		 put "*" in cs_name operand part
		
		[ERROR list]
		1. User puts no "*" mark in operands
		2. User puts "*" marks in all operands
*/
int search_ESTAB(char* cs_name, char* sym_name)
{
	int result_adrs=0;

	if(es_head == NULL)
	{
		printf("ESTAB is null\n");
		return -1;
	}

	pESTAB new_head = es_talloc();
	new_head = es_head->Next;

	if((!(strcmp(cs_name,"*")))&&(strcmp(sym_name,"*")))
	{
		//search SYMBOL adrs in ESTAB
		while(new_head != NULL)
		{
			if(!strcmp(new_head->sym_name,sym_name))
			{
				result_adrs = new_head->adrs;
				break;
			}
			new_head=new_head->Next;
		}
	}
	else if((!(strcmp(sym_name,"*")))&&(strcmp(cs_name,"*")))
	{
		//search Control Section adrs in ESTAB
		while(new_head != NULL)
		{
			if(!strcmp(new_head->cs_name,cs_name))
			{
				result_adrs = new_head->adrs;
				break;
			}
			new_head = new_head->Next;
		}
	}
	else
	{
		free(new_head);
		printf("ERROR : one of operands has to be '*'\n");
		return -1;
	}

	new_head=NULL;
	free(new_head);
	return result_adrs;
}

void clear_ESTAB()
{
	es_p=es_talloc();
	es_p=es_head->Next;
	while(es_p!=NULL)
	{
		es_p = es_p->Next;
	}
	return;
}

int progaddr(char* adrs)
{
	char* ptr;
	long val_sth;
	val_sth = strtol(adrs, &ptr, 16);
	int val_int = (int)val_sth;

	program_adrs = val_int;
	return 0;
}


//split function is for split the string from start index to end index
char* split(char* string, int start_idx, int end_idx)
{
	char *result = malloc(sizeof(char)*(end_idx-start_idx+1));
	strcpy(result,""); //initialize for deleting garbage values

	for(int i=start_idx;i<=end_idx;i++)
	{
		char buffer[2];
		sprintf(buffer,"%c",string[i]);
		strcat(result,buffer);
	}

	return result;
}

//from string to integer function
int str_to_int(char* string, int radix)
{
	char* ptr;
	long val_sth;
	val_sth = strtol(string, &ptr, radix);
	int val_int = (int)val_sth;

	return val_int;
}

int loader(char* file1, char* file2, char* file3)
{
	if(es_head != NULL) 
	{
		es_head = NULL;
	}

	int file_num=0;//number of file
	es_head = es_talloc();
	
	front = bp_talloc();
	rare = bp_talloc();
	front = NULL;
	rare = NULL;

	//check file input number
	
	if((!strcmp(file2,"*"))&&(!strcmp(file3,"*")))
	{
		int len=strlen(file1);
		file1[len-1]='\0';
		file_num=1;
	}
	else if((!strcmp(file3,"*"))&&(strcmp(file2,"*")))
	{
		int len=strlen(file2);
		file2[len-1]='\0';
		file_num=2;
	}
	else
	{
		int len=strlen(file3);
		file3[len-1]='\0';
		file_num=3;
	}

	//get PROGADDR from operating system
	int PROGADDR = program_adrs;
	int CSADDR = PROGADDR;
	total_len=0;

	for(int i=0;i<file_num;i++)
	{
		FILE* fp;
		if(i==0) 
		{
			fp=fopen(file1,"r");
		}
		else if(i==1) 
		{
			fp=fopen(file2,"r");
		}
		else if(i==2) 
		{
			fp=fopen(file3,"r");
		}
		if(fp==NULL)
		{
			printf("ERROR : File Open Error\n");
			return 0;
		}

		//Header Record
		char* Program_name;
		int local_start_adrs;
		int local_prog_len;
		char Header[255];
		fgets(Header,255,fp);
		if(Header[0] != 'H')
		{
			printf("ERROR: Header Recored has to be started with H\n");
		}
		Program_name = split(Header,1,6);
		Program_name = delete_white_space(Program_name);
		local_start_adrs = str_to_int(split(Header,7,12),16) + PROGADDR + total_len;
		CSADDR = local_start_adrs;
		local_prog_len = str_to_int(split(Header,13,18),16);
		put_ESTAB(Program_name,"*",local_start_adrs,local_prog_len);	
		total_len += local_prog_len;
		
		//Other Records (Define, Refer, Text)
		char record[255];
		fgets(record,255,fp);
		if(record[0] == 'D')
		{
			int i=1;
			while((record[i] != '\0'))
			{
				char* NAME=split(record,i,i+5);
				NAME=delete_white_space(NAME); 
				i+=6;
				int adrs;
				adrs = str_to_int(split(record,i,i+5),16) + CSADDR;
				put_ESTAB("*",NAME,adrs,0);
				i+=6;
				if(record[i]=='\n') break;
			}
		}
		fclose(fp);
	}

	print_ESTAB();
	

	//pass2
	for(int i=0;i<file_num;i++)
	{
		char ref_sym_table[MAX_REF_NUM][20];
		FILE* fp;
		if(i==0) fp=fopen(file1,"r");
		else if(i==1) fp=fopen(file2,"r");
		else if(i==2) fp=fopen(file3,"r");
		
		if(fp==NULL)
		{
			printf("ERROR: File Open ERROR\n");
			return 0;
		}

		//Header Record
		char Header[255];
		fgets(Header,255,fp);
		char* Control_Section = split(Header,1,6);
		Control_Section = delete_white_space(Control_Section);
		int cs_adrs = search_ESTAB(Control_Section,"*");
		strcpy(ref_sym_table[1],Control_Section);

		//Other Records(Define, Refer, Text, Modification)
		char record[255];
		fgets(record,255,fp);
		while(record[0]!='E')
		{
			if(record[0]=='D');
			else if(record[0]=='R')
			{
				int i=1;
				while(record[i] != '\0')
				{
					int ref_num = str_to_int(split(record,i,i+1),16);
					char* ref_sym = split(record,i+2,i+7);
					ref_sym = delete_white_space(ref_sym);
					strcpy(ref_sym_table[ref_num],ref_sym);
					i=i+8;
				}
				
			}
			else if(record[0]=='T')
			{
				int text_adrs = str_to_int(split(record,1,6),16);				
				int text_len = str_to_int(split(record,7,8),16)*2;
				char* text_obj_code = split(record,9,9+text_len);
				int t=0;
				for(int i=0;i<text_len;)
				{
					char* obj_code_buffer = split(text_obj_code,i,i+1);
					int val = str_to_int(obj_code_buffer,16);
					edit(text_adrs+t+cs_adrs,val);
					i+=2;
					t+=1;
				}
			}
			else if(record[0]=='M')
			{
				int m_adrs = str_to_int(split(record,1,6),16) + cs_adrs;
				int m_len = str_to_int(split(record,7,8),16);
				char* sign = split(record,9,9);
				int ref_sym_num = str_to_int(split(record,10,11),16);

				char ref_sym_name[20];
				strcpy(ref_sym_name,ref_sym_table[ref_sym_num]);
				
				int ref_sym_adrs = search_ESTAB("*",ref_sym_name);
				if(ref_sym_adrs==0) ref_sym_adrs=search_ESTAB(ref_sym_name,"*");

				char* original_mem = get_from_mem(m_adrs, m_len);
				int mem_to_int = str_to_int(original_mem,16);
				if(original_mem[0]=='F') mem_to_int = two_complement(mem_to_int);

				if(!strcmp(sign,"+"))
				{
					mem_to_int += ref_sym_adrs;
				}
				else if(!strcmp(sign,"-"))
				{
					mem_to_int -= ref_sym_adrs;
				}
			
				char* result;
				result = malloc(sizeof(char)*m_len);
				sprintf(result,"%X",mem_to_int);
				result = put_zero_str(result, m_len);

				if(m_len%2)
				{
					m_len += 1;
					char* new_result = malloc(sizeof(char)*m_len);
					strcpy(new_result,"");
					char* mem_temp = get_from_mem(m_adrs,2);
					char temp[2]="";
					temp[0]=mem_temp[0];
					strcat(new_result,temp);
					strcat(new_result,result);
					strcpy(result,new_result);
				}

				int t=0;
				for(int i=0;i<m_len;)
				{
					int val = str_to_int(split(result,i,i+1),16);
					edit(m_adrs+t,val);
					t+=1;
					i+=2;
				}
			}
			else if(record[0]=='E')
			{
				break;
			}
			else
			{
				if(record[0]=='.'); //comments or skip part
				else
				{
					printf("ERROR : Record doesn't start with H, D, R, T, M, E or .\n");
				}
			}
		fgets(record,255,fp);
		}//if find END record, break.
		fclose(fp);
	}
	

	//clear the ESTAB
	pESTAB prev=NULL;
	pESTAB seek=es_head->Next;
	while(seek!=NULL)
	{
		prev=seek;
		seek=seek->Next;
		(prev->cs_name)[0]='\0';
		(prev->sym_name)[0]='\0';
		prev->adrs=0;
		prev->len=0;
		prev=NULL;
		clear_ESTAB();
	}
	es_tail=NULL;
	es_p=NULL;
	es_head=NULL;
	seek=NULL;
	free(seek);
	free(es_head);
	free(es_p);
	free(es_tail);

	return 0;
}

int hex_to_ascii(int hex)
{
	if((0<=hex)&&(hex<=9))
	{
		return hex+48;
	}
	else if((10<=hex)&&(hex<=15))
	{
		return hex+55;
	}
	else
	{
		printf("ERROR: input hexadecimal num is inproper.\n");
		return -1;
	}
}

char* get_from_mem(int m_adrs, int m_len)
{
	int original_len = m_len;

	if(m_len%2 != 0)
	{
		m_len += 1;
	}

	char* result = malloc(sizeof(char)*(m_len));
	strcpy(result,"");

	for(int i=m_adrs;i<m_adrs+(m_len/2);i++)
	{
		char mem_buffer[2];
		sprintf(mem_buffer,"%X",mem[i]);
		strcat(result,mem_buffer);
	}

	int dif_of_len = m_len - original_len;
	char* new_result = malloc(sizeof(char)*(original_len));
	strcpy(new_result,"");

	if(dif_of_len == 1)
	{
		for(int i=1;i<m_len;i++)
		{
			char buffer[2]="";
			buffer[0]=result[i];
			strcat(new_result,buffer);
		}
	}
	else
	{
		strcpy(new_result,result);
	}
	return new_result;
}

char* put_zero_str(char* string, int len)
{
	char* result = malloc(sizeof(char)*len);
	strcpy(result,"");
	char temp_str[2]="";

	int string_len = strlen(string);
	int dif_of_lens = len - string_len;
	if(dif_of_lens<0)
	{
		dif_of_lens = -dif_of_lens;
		for(int i=dif_of_lens;i<dif_of_lens+len;i++)
		{
			temp_str[0] = string[i];
			strcat(result,temp_str);
		}
		return result;
	}
	else
	{
		for(int i=0;i<dif_of_lens;i++)
		{
			char zero_str[2] = "0";
			strcat(result,zero_str);
		}
		strcat(result,string);
		return result;
	}

	return result;
}

int two_complement(int minus_hex)
{
	int result = minus_hex - FFFFFF - 1; // FFFFFF in hex is 16777215 in decimal
	return result;
}

char* delete_white_space(char* string)
{
	char* result;
	result=malloc(sizeof(char)*strlen(string));
	strcpy(result,"");

	for(int i=0;i<strlen(string);i++)
	{
		if(!((string[i]>=65)&&(string[i]<=90))&&!((string[i]>=97)&&(string[i]<=122)))
		{
			//if it is not alphabet, pass
			;
		}
		else
		{
			char buffer[1];
			buffer[0] = string[i];
			//choose which is not white space
			strcat(result, buffer);
		}
	}

	return result;
}
int print_bp()
{
	if(front == NULL)
	{
		printf("Break Point Queue is NULL\n");
		return -1;
	}
	
	pBP temp_top;
	temp_top = front;

	printf("breakpoint\n");
	printf("----------\n");
	while(temp_top != NULL)
	{
		printf("%4X\n",temp_top->data);
		temp_top = temp_top->Next;
	}

	return 0;
}

int run()
{
	format = 0;
	L = program_adrs + PROGRAM_END;

	pBP BreakPoint = POP();
	int STOP=0;

	if(BreakPoint == NULL)
	{
		STOP=PROGRAM_END;
	}
	else
	{
		STOP=BreakPoint->data;
	}

	if(PC > STOP) STOP=PROGRAM_END;
	
	while(PC <= STOP)
	{
		int memory = mem[PC];
		/*get operation
		  STEP is let computer know how many memory has to be readed.
		  if STEP is 1(format 2), the computer reads just ONE MORE memory.
		  if STEP is 2(foramt 3), the computer reads TWO MORE memory.
		  if STEP is 3(format 4), the computer reads THREE MORE memory.
		  */

		if(!strcmp(now_seeing_operation,""))
		{
			STEP=2; //default STEP is 2
			//If program has no operation
			ni=get_operation(memory); //if operation is format 2, STEP changes to 1
		}
		else
		{
			//If program has operation
			int mem_buf = str_to_int(get_from_mem(PC,2),16);
			char* buffer = malloc(sizeof(char)*10);
			strcpy(buffer,"");
			sprintf(buffer,"%02X",mem_buf);

			if((buffer[0]=='1')&&(strlen(now_seeing_operand) < 2)&&(STEP!=1)) 
			{
				STEP=3;
				format=4;	
			}//format 4. if STEP==1, it means it is format 2. so do not check.
			switch(STEP)
			{
				case 3 :
					{
						strcat(now_seeing_operand,buffer);
						STEP=2;
					}; break;
				case 2 :
					{
						strcat(now_seeing_operand,buffer);
						STEP=1;
					};break;
				case 1 :
					{
						strcat(now_seeing_operand,buffer);
						excute_operation();			
					}; break;
			}
		}
		PC++;
	}

	printf("A : %06X X : %06X\n",A,X);
	printf("L : %06X PC: %06X\n",L,PC);
	printf("B : %06X S : %06X\n",B,S);
	printf("T : %06X\n",T);

	if(PC >= PROGRAM_END)
	{
		printf("End Program\n");
	}
	else printf("Stop at checkpoint[%04X]\n",STOP);

	return 0;
}

int bp(char* input_adrs)
{
	int adrs = str_to_int(input_adrs,16);
	pBP new_node = bp_talloc();
	new_node->data = adrs;
	new_node->Next = NULL;

	if(front==NULL) front=new_node;
	else rare->Next=new_node;
	rare=new_node;
	printf("[ok] create break point %04X\n",adrs);
	return 0;
}

int clear_bp()
{
	if(front == NULL) 
	{
		printf("[ok] no break points\n");
		return 0;
	}

	pBP new_node = POP();
	while(new_node != NULL)
	{
		free(new_node);
		new_node = POP();
	}
	
	printf("[ok] clear all breakpoints\n");
	return 0;
}

BP* POP()
{
	if(front == NULL) return NULL;
	
	pBP temp_rare = bp_talloc();
	temp_rare = front;
	front=temp_rare->Next;
	
	return temp_rare;
}

int get_operation(int opni)
{
	int temp_opni = opni;
	int temp_ni=0;
	char operation[20]="";
	for(temp_ni=0;temp_ni<4;temp_ni++)
	{
		temp_opni = opni;
		temp_opni = temp_opni-temp_ni;
		strcpy(operation,"");
		sprintf(operation,"%02X",temp_opni);

		if(!strcmp(operation,"00")) 
		{
			strcpy(now_seeing_operation,"LDA");
			break;
		}
		else if(!strcmp(operation,"50"))
		{
			strcpy(now_seeing_operation,"LDCH");
			break;
		}
		else if(!strcmp(operation,"74"))
		{
			strcpy(now_seeing_operation,"LDT");
			break;
		}
		else if(!strcmp(operation,"68"))
		{
			strcpy(now_seeing_operation,"LDB");
			break;
		}
		else if(!strcmp(operation,"14"))
		{
			strcpy(now_seeing_operation,"STL");
			break;
		}
		else if(!strcmp(operation,"4C")) 
		{
			strcpy(now_seeing_operation,"RSUB");
			break;
		}
		else if(!strcmp(operation,"48"))
		{
			strcpy(now_seeing_operation,"JSUB");
			break;
		}
		else if(!strcmp(operation,"28"))
		{
			strcpy(now_seeing_operation,"COMP");
			break;
		}
		else if(!strcmp(operation,"30"))
		{
			strcpy(now_seeing_operation,"JEQ");
			break;
		}
		else if(!strcmp(operation,"3C"))
		{
			strcpy(now_seeing_operation,"J");
			break;
		}
		else if(!strcmp(operation,"0C"))
		{
			strcpy(now_seeing_operation,"STA");
			break;
		}
		else if(!strcmp(operation,"08"))
		{
			strcpy(now_seeing_operation,"LDL");
			break;
		}
		else if(!strcmp(operation,"04"))
		{
			strcpy(now_seeing_operation,"LDX");
			break;
		}
		else if(!strcmp(operation,"E0"))
		{
			strcpy(now_seeing_operation,"TD");
			break;
		}
		else if(!strcmp(operation,"D8"))
		{
			strcpy(now_seeing_operation,"RD");
			break;
		}
		else if(!strcmp(operation,"54"))
		{
			strcpy(now_seeing_operation,"STCH");
			break;
		}
		else if(!strcmp(operation,"A0"))
		{
			strcpy(now_seeing_operation,"COMPR");
			format=2;
			STEP=1;
			break;
		}
		else if(!strcmp(operation,"B8"))
		{
			strcpy(now_seeing_operation,"TIXR");
			format=2;
			STEP=1;
			break;
		}
		else if(!strcmp(operation,"38"))
		{
			strcpy(now_seeing_operation,"JLT");
			break;
		}
		else if(!strcmp(operation,"10"))
		{
			strcpy(now_seeing_operation,"STX");
			break;
		}
		else if(!strcmp(operation,"4C"))
		{
			strcpy(now_seeing_operation,"RSUB");
			break;
		}
		else if(!strcmp(operation,"B4"))
		{
			strcpy(now_seeing_operation,"CLEAR");
			format=2;
			STEP=1;
			break;
		}
		else if(!strcmp(operation,"DC"))
		{
			strcpy(now_seeing_operation,"WD");
			break;
		}
	}

	return temp_ni;
}

int excute_operation()
{
	char* operand;
	char* xbpe;
	char* buf_xbpe;
	if(format==4)
	{
		operand=split(now_seeing_operand,1,5);
		buf_xbpe=split(now_seeing_operand,0,0);
		xbpe=get_xbpe(buf_xbpe);
	}
	else if(format==2)
	{
		operand=now_seeing_operand;
	}
	else
	{
		operand=split(now_seeing_operand,1,3);
		buf_xbpe=split(now_seeing_operand,0,0);
		xbpe=get_xbpe(buf_xbpe);
	}

	if(!strcmp(now_seeing_operation,"STL"))
	{
		if(ni==3)
		{
			if(xbpe[2]=='1')
			{
				int val = str_to_int(operand,16);
				val = val + PC + 1;
				insert_mem(val,L,4);
			}
			else if(xbpe[1]=='1')
			{
				int val = str_to_int(operand,16);
				val = val + B;
				insert_mem(val,L,4);
			}
		}
	}
	else if(!strcmp(now_seeing_operation,"LDB"))
	{
		if(ni==1)
		{
			int val = str_to_int(operand,16);
			val = val + PC + 1;
			B = val;
		}	
	}
	else if(!strcmp(now_seeing_operation,"JSUB"))
	{
		if(format==4)
		{
			int val = str_to_int(operand,16);
			L = PC + 1;
			PC = val-1;
		}
	}
	else if(!strcmp(now_seeing_operation,"CLEAR"))
	{
		if(operand[0]=='1') X=0;
		else if(operand[0]=='0') A=0;
		else if(operand[0]=='4') S=0;
		else if(operand[0]=='5') T=0;
	}
	else if(!strcmp(now_seeing_operation,"LDT"))
	{
		if(format==4)
		{
			if(ni==1)
			{
				int val=str_to_int(operand,16);
				T=val;
			}
		}
		else
		{
			int val = str_to_int(operand,16);
			if(xbpe[1]=='1') val = val + B;
			else if(xbpe[2]=='1') val = val + PC;
			T = str_to_int(get_from_mem(val,6),16);
		}
	}
	else if(!strcmp(now_seeing_operation,"TD"))
	{
		SW=DEVICE_STATEMENT;
	}
	else if(!strcmp(now_seeing_operation,"JEQ"))
	{
		if(SW==0)
		{
			int val = str_to_int(operand,16);
			if(operand[0]=='F') val=val - 4095 - 1; //4095 is FFF
			val = val + PC + 1;
			PC = val-1;
		}
	}
	else if(!strcmp(now_seeing_operation,"RD"))
	{
		A=DEVICE_INPUT;
	}
	else if(!strcmp(now_seeing_operation,"COMPR"))
	{
		//default DEVICE_INPUT is 0
		int val;
		if(operand[0]=='0') val = A;

		if(operand[1]=='4') val = val - S;

		SW=val;
	}
	else if(!strcmp(now_seeing_operation,"STCH"))
	{
		int val=str_to_int(operand,16);
		if(xbpe[1]=='1')
		{
			//BASE RELATIVE
			val = val + B + X;
			insert_mem(val,A,2);	
		}
		else if(xbpe[2]=='1')
		{
			val = val + PC + 1;
			insert_mem(val,A,2);
		}
	}
	else if(!strcmp(now_seeing_operation,"TIXR"))
	{
		int val;
		X += 1;
		if(operand[0]=='5') val = X - T;
		SW = val;
	}
	else if(!strcmp(now_seeing_operation,"JLT"))
	{
		if(SW < 0)
		{
			if(xbpe[2]=='1')
			{
				int val = str_to_int(operand,16);
				if(operand[0]=='F') val= val - 4095 - 1; //4095 is FFF
				val = val + PC + 1;
				PC = val - 1;
			}
			else if(xbpe[1]=='1')
			{
				int val = str_to_int(operand,16);
				if(operand[0]=='F') val = val - 4095 - 1;
				val = val + B;
				PC = val - 1;
			}
		}
	}
	else if(!strcmp(now_seeing_operation,"STX"))
	{
		if(xbpe[1]=='1')
		{
			//Base Relative
			int val = str_to_int(operand,16);
			val = val + B;
			insert_mem(val,X,6);
		}
		else if(xbpe[2]=='1')
		{
			//PC Relative
			int val = str_to_int(operand,16);
			val = val + PC + 1;
			insert_mem(val,X,6);
		}
	}
	else if(!strcmp(now_seeing_operation,"RSUB"))
	{
		PC = L-1;
	}
	else if(!strcmp(now_seeing_operation,"COMP"))
	{
		if(ni == 1)
		{
			int val = A - str_to_int(operand,16);
			SW = val;
		}
		else if(ni == 3)
		{
			int mem_buf = str_to_int(operand,16);
			mem_buf = str_to_int(get_from_mem(mem_buf,3),16);
			int val = A - mem_buf;
			SW = val;
		}
	}
	else if(!strcmp(now_seeing_operation,"J"))
	{
		if(ni==3)
		{
			int val = str_to_int(operand,16);
			if(operand[0]=='F') val = val - 4095 - 1; //4095 is FFF
			if(xbpe[2]=='1') val = val + PC + 1;
			else if(xbpe[1]=='1') val = val + B;
			PC = val - 1;
		}
		else if(ni==2)
		{
			//indirect addressing
			int val = str_to_int(operand,16);
			if(operand[0]=='F') val = val - 4095 - 1;
			if(xbpe[2]=='1') val = val + PC + 1;
			else if(xbpe[1]=='1') val = val + B;
			int adrs = str_to_int(get_from_mem(val,4),16);
			PC = adrs - 1;
		}
	}
	else if(!strcmp(now_seeing_operation,"LDA"))
	{
		int val = str_to_int(operand,16);
		if(ni==3)
		{
			if(xbpe[2]=='1') val = val + PC + 1;
			int result = str_to_int(get_from_mem(val,6),16);
			A = result;
		}
		if(ni==1)
		{
			A = val;
		}
	}
	else if(!strcmp(now_seeing_operation,"STA"))
	{
		int val = str_to_int(operand,16);
		if(ni==3)
		{
			if(xbpe[2]=='1') val = val + PC + 1;
			insert_mem(val,A,6);
		}
	}
	else if(!strcmp(now_seeing_operation,"LDCH"))
	{
		int val = str_to_int(operand,16);
		if(xbpe[0] == '1')
		{
			//Using X register
			if(xbpe[1]=='1')
			{
				//Base Relative
				val = val + B + X;
				val = str_to_int(get_from_mem(val+2,2),16);
				A = val;
			}
		}	
	}
	else if(!strcmp(now_seeing_operation,"WD"))
	{
		;
	}

	//clear global variables
	strcpy(now_seeing_operation,"");
	ni=0;
	format=0;
	strcpy(now_seeing_operand,"");

	return 0;
}

char* get_xbpe(char* buffer)
{
	char* result = malloc(sizeof(char)*4);
	int binary=0;
	int sum=0;
	int i=1;

	int val = str_to_int(buffer,16);
	while(val>0)
	{
		binary=(int)val%2;
		sum+=binary*i;
		val=(int)val/2;
		i*=10;
	}
	sprintf(result,"%04d",sum);
	return result;
}

void insert_mem(int adrs, int value, int len)
{
	char* buffer = malloc(sizeof(char)*len);
	sprintf(buffer,"%X",value);
	buffer = put_zero_str(buffer,len);
	
	int t=0;
	for(int i=0;i<strlen(buffer);)
	{
		int val = str_to_int(split(buffer,i,i+1),16);
		edit(adrs+t,val);
		t+=1;
		i+=2;
	}	

	return;
}
