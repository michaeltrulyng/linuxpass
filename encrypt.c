/*
* encrypt.c
*	Program that uses DES-encryption to encrypt a string
*
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define ARRAY_SIZE		1024
#define DES_SALT_SIZE	2
#define TOTAL_LETTERS	26
#define TOTAL_CASES		2

char new_pw[ARRAY_SIZE];
char encrypt[ARRAY_SIZE];

char *crypt(const char*, const char*);
void *generate_salt(char*);
char getval(int, int);
time_t time(time_t *);
void process_file(char*, char*, char*, char*, char*);
ssize_t getline(char**, size_t*, FILE*);

char getval(int letter, int uppercase){
	char retval = ' ';
	switch (uppercase){
		case 0: // lowercase
			switch(letter){
				case 0:
					retval = 'a';
					break;
				case 1:
					retval = 'b';
					break;
				case 2:
					retval = 'c';
					break;
				case 3:
					retval = 'd';
					break;
				case 4:
					retval = 'e';
					break;
				case 5:
					retval = 'f';
					break;
				case 6:
					retval = 'g';
					break;
				case 7:
					retval = 'h';
					break;
				case 8:
					retval = 'i';
					break;
				case 9:
					retval = 'j';
					break;
				case 10:
					retval = 'k';
					break;
				case 11:
					retval = 'l';
					break;
				case 12:
					retval = 'm';
					break;
				case 13:
					retval = 'n';
					break;
				case 14:
					retval = 'o';
					break;
				case 15:
					retval = 'p';
					break;
				case 16:
					retval = 'q';
					break;
				case 17:
					retval = 'r';
					break;
				case 18:
					retval = 's';
					break;
				case 19:
					retval = 't';
					break;
				case 20:
					retval = 'u';
					break;
				case 21:
					retval = 'v';
					break;
				case 22:
					retval = 'w';
					break;
				case 23:
					retval = 'x';
					break;
				case 24:
					retval = 'y';
					break;
				case 25:
					retval = 'z';
					break;
			}
			break;
		case 1: // uppercase
			switch(letter){
				case 0:
					retval = 'A';
					break;
				case 1:
					retval = 'B';
					break;
				case 2:
					retval = 'C';
					break;
				case 3:
					retval = 'D';
					break;
				case 4:
					retval = 'E';
					break;
				case 5:
					retval = 'F';
					break;
				case 6:
					retval = 'G';
					break;
				case 7:
					retval = 'H';
					break;
				case 8:
					retval = 'I';
					break;
				case 9:
					retval = 'J';
					break;
				case 10:
					retval = 'K';
					break;
				case 11:
					retval = 'L';
					break;
				case 12:
					retval = 'M';
					break;
				case 13:
					retval = 'N';
					break;
				case 14:
					retval = 'O';
					break;
				case 15:
					retval = 'P';
					break;
				case 16:
					retval = 'Q';
					break;
				case 17:
					retval = 'R';
					break;
				case 18:
					retval = 'S';
					break;
				case 19:
					retval = 'T';
					break;
				case 20:
					retval = 'U';
					break;
				case 21:
					retval = 'V';
					break;
				case 22:
					retval = 'W';
					break;
				case 23:
					retval = 'X';
					break;
				case 24:
					retval = 'Y';
					break;
				case 25:
					retval = 'Z';
					break;
			}
			break;
	}
	return retval;
};

void *generate_salt(char* salt){
	int letter, uppercase, i;
	memset(salt, DES_SALT_SIZE + 1, sizeof(char));

	for (i = 0; i < DES_SALT_SIZE; i++){
		letter = rand() % TOTAL_LETTERS;
		uppercase = rand() % TOTAL_CASES;
		salt[i] = getval(letter, uppercase);
	}
	salt[DES_SALT_SIZE + 1] = '\0';
	return salt;
};

void process_file(char *in_filename, char* out_filename, char* position_ptr, char *delimiter, char* salt){
	char *infilename = in_filename, *outfilename = out_filename, *line;
	char *token, conversion_str[ARRAY_SIZE];
	FILE *infile, *outfile;
	size_t len = 0, written;
	ssize_t read;
	int error = 0, field_counter;
	unsigned long position = strtol(position_ptr, (char**)NULL, 10);

	if ((infile = fopen(infilename, "rb")) == NULL){
		perror("ERROR");
		error = 1;
	}

	if ((outfile = fopen(outfilename, "wb")) == NULL){
		perror("ERROR");
		error = 1;
	}
	if (!error){
		while((read = getline(&line, &len, infile)) != -1){
			field_counter = 0;
//			printf("Position: %lu; field_counter: %d\n",position,field_counter);
			token = strtok(line, delimiter);
			memset(conversion_str,0,ARRAY_SIZE*sizeof(char));
			if (token[strlen(token)-1] == '\n'){
				strncpy(conversion_str,token,(strlen(token) - 1)*sizeof(char));
				conversion_str[strlen(conversion_str)-1] = '\0';
			}else{
				strncpy(conversion_str,token,strlen(token)*sizeof(char));
			}

			if (position == field_counter){
				generate_salt(salt);
				strcpy(encrypt, (char*)crypt((char*)conversion_str, salt));
				if ((written = fwrite(encrypt, sizeof(char), strlen(encrypt), outfile)) != strlen(encrypt))
					printf("Error: only %d of %d bytes written.\n", written, strlen(encrypt));
			}else if ((written = fwrite(conversion_str, sizeof(char), strlen(conversion_str), outfile)) != strlen(conversion_str))
				printf("Error: only %d of %d bytes written.\n", written, strlen(conversion_str));

			while((token = strtok(NULL, delimiter)) != NULL){
				field_counter++;

				memset(conversion_str,0,ARRAY_SIZE*sizeof(char));
				if (token[strlen(token)-1] == '\n'){
					strncpy(conversion_str,token,(strlen(token) - 1)*sizeof(char));
					conversion_str[strlen(conversion_str)-1] = '\0';
				}else{
					strncpy(conversion_str,token,strlen(token)*sizeof(char));
				}

				if ((written = fwrite(delimiter, sizeof(char), strlen(delimiter), outfile)) != strlen(delimiter))
					printf("Error: only %d of %d bytes written.\n", written, strlen(delimiter));			
				if (position == field_counter){
					generate_salt(salt);
					strcpy(encrypt, (char*)crypt((char*)conversion_str, salt));
//					printf("token: %s; conversion_str: %s\n",token,conversion_str);
					if ((written = fwrite(encrypt, sizeof(char), strlen(encrypt), outfile)) != strlen(encrypt))
						printf("Error: only %d of %d bytes written.\n", written, strlen(encrypt));
				}else if ((written = fwrite(conversion_str, sizeof(char), strlen(conversion_str), outfile)) != strlen(conversion_str))
					printf("Error: only %d of %d bytes written.\n", written, strlen(conversion_str));
			}
			if ((written = fwrite("\n", sizeof(char), strlen("\n"), outfile)) != strlen("\n"))
				printf("Error: only %d of %d bytes written.\n", written, strlen("\n"));
		}
	}
	fclose(infile);
	fclose(outfile);
};

int main(int argc, char ** argv){
	/* For brevity, the following syntax is assumed:
	*	./encrypt <input filename> <output filename> <encrypt column> <optional delimiter>
	*/
	if (argc < 5){
		printf("Parameters missing. Aborting program.\n");
		printf("Syntax: <input filename> <output filename> <encrypt column> <delimiter>\n");
		return -1;
	}else{
		char *salt = malloc(sizeof(char) * (DES_SALT_SIZE + 1));
		srand((unsigned)time(NULL));
		process_file(argv[1],argv[2],argv[3],argv[4],salt);
		free(salt);
	}
	
//	generate_salt(salt);
//	strcpy(encrypt_pw, (char*)crypt((char*)new_pw, salt));
//	printf("new_pw: %s\n",new_pw);
//	printf("encrypted_pw: %s\n",encrypt_pw);

	return 0 ;
} ;
