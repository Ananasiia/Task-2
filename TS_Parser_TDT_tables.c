#include <stdio.h>                                    
#include <string.h>                                   
#include <stdlib.h> 
                                  
#define PACK_SIZE 753577508 


unsigned char *find_head(unsigned char *stream)  /* Returns pointer to head of some elementary stream. */
{
	unsigned char i = 0;
	unsigned char *ptr = NULL;

	if(NULL == stream)
	{
		printf("NULL pointer at fing_head()..\n");
		return NULL;
	}
	while(NULL == ptr)
	{
		if(stream[i] == 0x47)
		{
			ptr = &stream[i] ;
			printf("head is -> %x \n", *ptr);
		}
		else
		{
			i++;
		}
	}
	return ptr;	
			
}

unsigned short find_pid(unsigned char *start_of_es) 
{
	unsigned short pid_byte1 = 0;
	unsigned short pid_byte2 = 0;
	unsigned short whole_pid_field = 0;
        if(NULL == start_of_es)
        {
                printf("NULL pointer at find_pid().\n");
                return -1;
        }
	else
	{
		pid_byte1 = *(++start_of_es);
		pid_byte1 = (pid_byte1 << 8);
		pid_byte2 = *(++start_of_es);
		whole_pid_field = (pid_byte1 + pid_byte2);
		return whole_pid_field;
	}		
	
}

unsigned short determine_pid(unsigned short whole_pid_field) 
{
	unsigned short pid = 0;
	pid = (whole_pid_field << 3);
	pid = (pid >> 3);
	return pid;			
}

unsigned char payload_indicator(unsigned char *start_of_es)
{
	unsigned char i = 0;
	if(NULL == start_of_es)
	{
		printf("NULL pointer at payload_indicator().\n");
		return -1;		
	} 
	else
	{
		i = *start_of_es;
		i = (i << 1);
		i = (i >> 7);
		return i;	
	}
}

void print_es(unsigned char *head)
{
	unsigned char *ptr_head = NULL;
	int i = 0;
	int j = 188;
	if(NULL != head)
	{
		ptr_head = head;
		printf("\nThere is the content of elementary stream:\n");
		for(i; i < j; i++)
		{
			if(0 == (i % 20))
			{
				printf("\n");
			}
			printf("%x", *ptr_head);
			ptr_head++;
		}
		printf("\n\n\n");
	}
}

unsigned char continuity_index(unsigned char *head)
{
        unsigned char *ptr = NULL;
	unsigned char index = 0; 

	if(NULL == head)
	{
		printf("NULL pointer at continuity_index().\n");
		return -1;
	}
	else
	{
		ptr = (head + 3);
		index = (*ptr) << 4;
		index = index >> 4;
		return index;	  
	}
}

unsigned char *get_ptr_selection_payload(unsigned char *head) /* Returns poinret to beginning of payload selection field. */
{
	unsigned char *ptr = 0;
	if(NULL == head)
	{
		printf("NULL pointer at get_ptr_selection_payload().\n");
		return NULL;	
	}
	else
	{
		ptr = (head + 5);
		return ptr;	
	}
}

unsigned char selection_syntax_indicator(unsigned char *ptr_selection_payload)
{
	unsigned char ssi = 0;
	if(NULL == ptr_selection_payload)
	{
		printf("NULL pointer at selection_syntax_indicator().\n");
		return -1;
	}
	else
	{
		ssi = *ptr_selection_payload;
		ssi = (ssi >> 7);
		return ssi;	
	}
}

unsigned short selection_lenght(unsigned char *ptr_selection_payload)
{
	unsigned char *ptr = 0; 
	unsigned short sl = 0;
	unsigned short sl2 = 0;
	if(NULL == ptr_selection_payload)
	{
		printf("NULL pointer at selection_lenght().\n");
		return -1;
	}
	else
	{
		ptr = (ptr_selection_payload + 1);
		sl = *ptr;
		ptr++;
		sl2 = *ptr;
		sl = (sl << 8);
		sl2 = (sl + sl2);
		return sl2;	
	}
}

unsigned short mjd(unsigned char *ptr_selection_payload)
{
	unsigned short mjd = 0;
	unsigned short mjd2 = 0;
	unsigned char *ptr = NULL;
	if(NULL == ptr_selection_payload)
	{
		printf("NULL pointer at mjd().\n");
		return -1;
	}
	else
	{
		ptr = (ptr_selection_payload + 3);
		mjd = *ptr;
		ptr++;
                mjd2 = *ptr;
		mjd = (mjd << 8);
		mjd2 = (mjd | mjd2);
		return mjd2;  	
	}
}

void  year_month_day(unsigned short mjd)
{
	int year = 0;
	int month = 0;
	int day = 0;
	int k = 0;
	year = (int)( (mjd - 15078.2) / 365.25);
	month = (int)( (mjd - 14956.1 - (int)(year * 365.25)) / 30.6001);
	day = ( mjd - 14956 - (int)(year * 365.25) - (int)(month * 30.6001) );
	if(14 == month || 15 == month)
	{
		k = 1;	
	} 
	year = year + k + 1900;
	month = (month - 1 - (k * 12));
	
	printf("|DATE: %d / %d / %d|    ", year, month, day); 	
}

void print_hex_date_time(unsigned char *ptr_selection_payload)
{
	int i = 4;
	if(NULL != ptr_selection_payload)
	{
		printf("|DATE AND TIME IN HEX: 0x");
		for(i; i <= 8; i++)
		{
			printf("%x", *(ptr_selection_payload + i));	
		}
		printf("|\n");
	}
}

void time(unsigned char *ptr_selection_payload)
{
	unsigned char *ptr = NULL;
	unsigned char number = 0;
        unsigned char number2 = 0;
	int i = 0;
	if(NULL == ptr_selection_payload)
	{
		printf("NULL pointer at time().\n");
		return;
	}
	else
	{
		ptr = (ptr_selection_payload + 6);
		printf("|TIME : ");
		for(i; i < 3; i++)
		{
			if(1 == i || 2 == i)
			{
				printf(":");
			}
			number = *ptr;
			number2 = (number & 0x0f);
                        number = (number >> 4);
				printf("%d%d", number, number2);
			ptr += i;  	
		}
 		printf("|\n");
	}
	return;
}

void print_TDT_list_info(unsigned char *stream)
{
	unsigned char *ptr = NULL;
        unsigned char *ptr2 = NULL;
	unsigned short pid = 0;
	int sto = 0;
	unsigned char indicator = 0;	
        unsigned char cont_ind = 0;
	unsigned long counter = 0;
	unsigned char ssi = 0;
	unsigned short sec_len = 0;
	if(NULL != stream)
	{
		ptr = find_head(stream);
		for(counter; counter < 4008391; counter++, sto++)
		{
			// HEAD
			pid = find_pid(ptr);
			pid = determine_pid(pid);
                        indicator = payload_indicator(ptr);
			cont_ind =  continuity_index(ptr);
			// PAYLOAD
			ptr2 = get_ptr_selection_payload(ptr);
			ssi = selection_syntax_indicator(ptr2);
			sec_len = selection_lenght(ptr2); 
			if(NULL == ptr2 || NULL == ptr)
			{
				printf("NULL pointer at print_pid_list().\n");
				return;
			} 
			else
			{
				if(0x0014 == pid && 0x70 == *ptr2)
				{
					printf("## %u ##\n == HEAD == (%x) \n| PID : %x | Payload Indicator : %d | Continuity index : %d\n ",counter, *ptr, pid, indicator, cont_ind);
					printf(" ==  PAYLOAD ==\n| Table ID : %x | Selection syntax indicator : %d | Secction lenght : 0x%x \n ", *ptr2, ssi, sec_len);
					printf("****TDT**** Time and Date Table\n");
			 
					print_hex_date_time(ptr2);
				 	year_month_day(mjd(ptr2));
					time(ptr2);
					print_es(ptr);
				}
                      		 ptr += 188;
			}
  	        }		
	}
}

int help(char *argv[])
{
	int i = 0;
	char *h[4] = {"-h", "-help", "--h", "--help"};  
	if(NULL != argv)
	{
		for(i; i < 4; i++)
		{
			if(0 == strcmp(argv[1], h[i]))
			{
				printf("===%s===\n\n Application works with files consisted of Tranport Stream.\nCurrent version parses Time and Date Tables.\nFor parsing TS :\n", argv[0]);
				printf(" Indicate the binary file with stream after calling program in command string.\n[EXAMPLE] : ./TS_Parser_TDT_tables tablas_DTH.ts\n");
				printf("Otherwise, put the binary file with stream in the same folder with this application and rename it to 'tablas_DTH.ts'");
				return 1;
			}
		}
		return i;		
	}
	else 
	{
		printf("NULL pointer at help()");
		return -1;
	}
}

int main(int argc, char *argv[])
{         
	FILE *file = NULL;
 	unsigned char *shelf2 = NULL;
	int m = 0;                    
    

	if(1 < argc &&  1 == help(argv))
	{
		goto exit;
	}

/* ALLOCATE BUFF MEMORY */
   	shelf2 = (unsigned char*)malloc(sizeof(unsigned char)*PACK_SIZE);

   	if(NULL == shelf2)
	{
		printf("\nMamory is not aloated.\n");
	}
	    else
		{
			memset(shelf2, '0', (sizeof(unsigned char)*PACK_SIZE));

/* OPEN FILE */	
			if(1 != argc)
			{
				file = fopen(argv[1], "rb");		
			}
			else
			{
				file = fopen("tablas_DTH.ts", "rb");
			}
			if(NULL == file)
			{
				printf("The file is not opened.\n");
			}
			else
			{

/* READ CONTENT FROM FILE TO BUFF */
			        m = fread(shelf2, sizeof(unsigned char), PACK_SIZE, file);	
				printf("There %d bytes were writed to buffer.\n\n", m);
 				print_TDT_list_info(shelf2);               
			}	                            

/* FREE BUFF MAMORY AND CLOSE FILE */
			free(shelf2);
			shelf2 = NULL;
       			fclose(file);
	
		}
	exit:    
	return 0;
}   
