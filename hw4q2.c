#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N (100)
#define MAX_LEN (255)
#define MAX_SENTENCE_LEN ((MAX_LEN + 1) * N - 1)
#define WORD_SEPERATOR ('_')
#define SUCCESS (0)
#define ERROR (1)
#define CAP_NORMALIZER 'a' - 'A'

int case_insensitive(int ascii);
int letters_comp(char* pletter1, char* pletter2);


bool read_number_of_strings(int * n) {
    if (scanf("%d", n) != 1) {
        return false;
    }

    return ((*n >= 1) && (*n <= N));
}

void free_strings(char * strings[], int n) {
    for (int i = 0; i < n; i++) {
        free(*(strings + i));
    }
}

bool read_strings(char * strings[], int n) {
    /* TODO */

    //memory allocation
    char* temp = malloc(sizeof(char) * MAX_LEN);
    if (temp == NULL) {
        return false;
    }
    // scanning
    for (int i = 0; i < n; i++) {
        // word find
        int k = 0;
        while (k < MAX_LEN) {
            if (scanf("%c", temp + k) < 1) {
                free_strings(strings, i);
                free(temp);
                return false;
            }
            if ((*(temp + k)) == ' ' || (*(temp + k)) == '\n') {
                if (k != 0) {
                    *(temp + k) = '\0';
                    k++;
                }
                break;
            }
            k++;
        }
        if (k == 0) {
            i--;
            continue;
        }
        // updates strings[]
        *(strings + i) = malloc(sizeof(char) * k);
        if (*(strings + i) == NULL) {
            free_strings(strings, i);
            free(temp);
            return false;
        }
        memcpy(*(strings + i), temp, sizeof(char) * k);
    }
    free(temp);
    return true;
}

// returns true if words are sorted
bool are_sorted(char * strings[], int n) {

    if (n == 1)return true;
    for (int i = 0; i < n; i++) {
        int k = 0;
        if (i = n - 1)break;

        while ((*(*(strings + i) + k) != '\0') || 
            *(*(strings + i + 1) + k) != '\0') {
            if (letters_comp((*(strings + i) + k), *(strings + i + 1) + k)
                == 1) {
                return false;
            }
            else if (letters_comp((*(strings + i) + k), *(strings + i + 1) + k)
                == -1) {
                break;
                 }
            k++;
        }
        if ((*(*(strings + i) + k) != '\0')) {
            return false;
        }
    }
    return true;
}

// normalizes caps
int case_insensitive(int ascii) {
    if (ascii >= 'A' && ascii <= 'Z') {
        return ascii + CAP_NORMALIZER;
    }
    else return ascii;
}

//returns 1 for 1 element being bigger else 0.
int letters_comp(char * pletter1, char* pletter2) {
    int ascii1 = case_insensitive(*pletter1);
    int ascii2 = case_insensitive(*pletter2);
    if (ascii1 > ascii2) {
        return 1;
    }
    else if (ascii2 > ascii1) {
        return -1;
         }
    else return 0;
}




bool is_string_in_array(char * strings[], int n, char * string) {
    /*  
    * log n
    */
    int left = 0, right = n - 1, mid, k= 0;
    while (left <= right) {
        mid = (right + left) / 2;
        // printf("%s\n", *(strings + mid)); // debugging
        if (letters_comp((string + k), (*(strings + mid) + k)) == 0) {
            if ( *(string + k + 1) == '\0' &&
                *(*(strings + mid) + k + 1) == '\0') {
                return true;
            }
            k++;
        }
        else if(letters_comp((string + k), (*(strings + mid) + k)) == 1) {
            // string bigger
            left = mid + 1;
            k = 0;
        }
        else {
            right = mid - 1;
            k = 0;
        }
    }
    return false;
}

void delete_words(char * words[], int n, char * sentence) {
    /* TODO */
    int i = 0, wordlen = 0, censored = 0;
    char* word = malloc(sizeof(char) * MAX_LEN + 1);
    if (word == NULL) {
        exit(1);
    }
    while (true) {

        // printf("%d, %c \n", *(sentence + i), *(sentence + i));
        // buggggg

        if (*(sentence + i) == '_' || *(sentence + i) == '\0') {
            *(word + wordlen) = '\0';
            if (!is_string_in_array(words, n, word)) {
                // good word
                // printf("good word: %s\n", word); // debugging

                for (int k = 0; k < wordlen; k++) {
                    *(sentence + censored + k) = *(word + k);
                }

                if(*(sentence + i) == '_'){
                    *(sentence + censored + wordlen) = '_';
                }
                censored += wordlen + 1;
            }

            if (*(sentence + i) == '\0') {
                if (censored > 0) {
                    *(sentence + censored - 1) = '\0';
                }
                else {
                    *(sentence + censored) = '\0';
                }
                
                break;
            }
            
            wordlen = 0;
            i++;
            continue;
        }

        // printf("%d", wordlen);
        *(word + wordlen) = *(sentence + i);
        wordlen++;
        i++;
    }

    free(word);
}
 
int main() {
    int n, i;
    char c;

    printf("Enter number of banned words: ");
    if (!read_number_of_strings(&n)) {
        printf("Invalid number!\n");
        return ERROR;
    }

    char* banned_words[N];
    printf("Enter banned words: ");
    if (!read_strings(banned_words, n)) {
        printf("Can't read words!\n");
        return ERROR;
    }

    // debugging 
    /*for (int i = 0; i < n; i++) {
        int j = 0;
        while ((*(*(banned_words + i) + j)) != '\0') {
            printf("%c", (*(*(banned_words + i) + j)));
            j++;
        }
        printf("\n");
    }*/

    // char string[] = "cat";
    // printf("%d\n", is_string_in_array(banned_words, n, string));


    if (!are_sorted(banned_words, n)) {
        printf("Words are not sorted correctly!\n");
        free_strings(banned_words, n);
        return ERROR;
    }

	/*
    char sentence[MAX_SENTENCE_LEN + 1];
    printf("Enter a sentence:\n");
    if (scanf("%s", sentence) != 1) {
        printf("Invalid sentence!\n");
        free_strings(banned_words, n);
        return ERROR;
    }
	*/
	
	char sentence[MAX_SENTENCE_LEN + 1];
	printf("Enter a sentence:\n");
	for (i=0; i < MAX_SENTENCE_LEN; i++)
	{
	  if(scanf("%c", &c) != 1 || c == '\n')
			break;
	  sentence[i] = c;
	}
	sentence[i] = '\0';

	if(strlen(sentence) == 0)
	{
       printf("Invalid sentence!\n");
	   free_strings(banned_words, n);
	   return ERROR;
	}
	
    delete_words(banned_words, n, sentence);
    printf("Censored sentence:\n%s\n", sentence);
    free_strings(banned_words, n);
    return SUCCESS;
}
