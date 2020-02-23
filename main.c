//
//  main.c
//  Keychain
//
//  Created by Nick Bonner on 1/18/20.
//  Copyright © 2020 Nick Bonner. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int PWSIZE = 31;
const int NAMESIZE = 31;
const int MAXLINE = 100;
const int START = 31;

void readme() {
    const int READLINE = 200;
    char *line = calloc(READLINE, sizeof(char));
    
    FILE *readme = fopen("Keychain_Readme.txt", "r");
    while (!feof(readme)) {
        fgets(line, MAXLINE, readme);
        printf("%s", line);
        memset(line, 0, sizeof(char));
    }
    
    fclose(readme);
    free(line);
}

int addrandom() {

    char *password = calloc(PWSIZE, sizeof(char));
    char *name = calloc(NAMESIZE, sizeof(char));
    char *line = calloc(MAXLINE, sizeof(char));
    int n;
    
    FILE *source = fopen("Keychain_Source.txt", "a");
    
    /*check file open success*/
    if (source == NULL) {
        printf("Keychain failed to open. Confirm keychain text file is in same folder as this tool.\n\n");
        return 1;
    }
    
    if (password == NULL)
        printf("Failed to allocate for password");
    if (name == NULL)
        printf("Failed to allocate for name");
    if (line == NULL)
        printf("Failed to allocate for line");
    
    /*build name and password group*/
    printf("Enter the name associated with this password: ");
    fflush(stdin);
    fgets(name, NAMESIZE, stdin);
    name[strlen(name) - 1] = '\0';
    
    printf("Enter requested size of password, max is 30: ");
    scanf("%d", &n);
    srand(time(NULL));
    
    for (int i = 0; i < n; i++) {
        char c = (rand() % 95) + 32;
        password[i] = c;
     }
    
    printf("Adding %s with password %s\n", name, password);
   
    int position = 0;
    
    for (int i = 0; name[i] != '\0'; i++) {
            
        if (name[i] + position > 126) {
            line[i] = ((name[i] + position) % 126) + START;
            position++;
        } else {
            line[i] = name[i] + position;
            position++;
        }
            
    }
    
    line[position] = ':';
    position++;
    line[position] = ' ';
    position++;
    
    int pw = 0;
    for (int i = position;; i++) {
        if (password[pw] == '\0')
            break;
        if (password[pw] + position > 126) {
            line[i] = ((password[pw] + position) % 126) + START;
            position++;
            pw++;
          
        } else {
            line[i] = password[pw] + position;
            position++;
            pw++;
        }
    }
    
    fprintf(source, "%s\n", line);
    
    free(line);
    free(password);
    free(name);
    fclose(source);
    
    return 0;
}


int adddefined() {
   
    char *password = calloc(PWSIZE, sizeof(char));
    char *name = calloc(NAMESIZE, sizeof(char));
    char *line = calloc(MAXLINE, sizeof(char));
    int key = 0;
    
    FILE *source = fopen("Keychain_Source.txt", "a");
    
    if (source == NULL) {
        printf("Could not find source file. Make sure file is in same folder group as this tool.\n");
        return 1;
    }
    
    if (line == NULL || name == NULL || password == NULL) {
        printf("Failed to allocate memory for variables\n");
        return 1;
    }
    
    printf("Enter the name for your password: ");
    fflush(stdin);
    fgets(name, NAMESIZE, stdin);
    name[strlen(name) - 1] = '\0';
    
    printf("Enter your password (30 character maximum): ");
    fgets(password, PWSIZE, stdin);
    password[strlen(password) - 1] = '\0';
    
    printf("\nAdding %s with password %s\n", name, password);
    
    for (int i = 0; name[i] != '\0'; i++) {
        if ((name[i] + key) > 126){
            line[i] = ((name[i] + key) % 126) + START;
            key++;
        } else {
            line[i] = name[i] + key;
            key++;
        }
        
    }
    
    line[key] = ':';
    key++;
    line[key] = ' ';
    key++;
    
    for (int i = 0; password[i] != '\0'; i++){
        if ((password[i] + key) > 126){
            line[key] = ((password[i] + key) % 126) + START;
            key++;
        } else {
            line[key] = password[i] + key;
            key++;
        }
    }
  
    fprintf(source, "%s\n", line);
    
    free(name);
    free(password);
    free(line);
    fclose(source);
    
    return 0;
}

int retrieve() {
   
    char *password = calloc(PWSIZE, sizeof(char));
    char *name = calloc(NAMESIZE, sizeof(char));
    char *line = calloc(MAXLINE, sizeof(char));
    char *check = calloc(NAMESIZE, sizeof(char));
    int key = 0;
    
    FILE *source = fopen("Keychain_Source.txt", "r+");
    if (source == NULL) {
        printf("Could not find source file. Make sure it is in same folder group as this tool\n");
        return 1;
    }
    if (line == NULL || name == NULL || check == NULL || password == NULL) {
        printf("Failed to allocate memory for variables\n");
        return 1;
    }
    
    printf("Enter name of requested password: ");
    fflush(stdin);
    fgets(name, NAMESIZE, stdin);
    name[strlen(name) - 1] = '\0';
    
    while (!feof(source)) {
         if (fgets(line, MAXLINE, source) == NULL) {
                   break;
         };
        
        for (int i = 0; line[i] != ':' && line[i] != '\n' && line[i] != '\r'; i++) {
            if ((line[i] - key) <= START) {
                check[i] = 127 - (line[i] - key);
                key++;
            } else {
                check[i] = line[i] - key;
                key++;
            }
        }
        
        if (strncmp(name, check, strlen(name)) == 0) {
            printf("Match found!\n");
            key += 2;
            for (int i = 0; line[key] != '\0'; i++) {
                if ((line[key] - key) <= START) {
                    password[i] = 127 - ((START + 1) - (line[key] - key));
                    key++;
                } else {
                    password[i] = line[key] - key;
                    key++;
                }
            }
            password[strlen(password) - 1] = '\0'; /*cheat with a null terminator, not sure where extra char is coming from*/
            printf("%s password: %s\n", name, password);
            break;
        }
        
        key = 0;
        memset(line, 0, MAXLINE);
        memset(check, 0, NAMESIZE);
        memset(password, 0, PWSIZE);
    }
            
    free(password);
    free(name);
    free(line);
    free(check);
    fclose(source);
    
    return 0;
    
}

int updatedefined() {
    
    char *name = calloc(NAMESIZE, sizeof(char));
    char *check = calloc(NAMESIZE, sizeof(char));
    char *line = calloc(MAXLINE, sizeof(char));
    char *new_line = calloc(MAXLINE, sizeof(char));
    char *password = calloc(PWSIZE, sizeof(char));
    int key = 0;
    
    if (line == NULL || name == NULL || check == NULL || password == NULL) {
        printf("Failed to allocate memory for variables\n");
        return 1;
    }
    
    FILE *source = fopen("Keychain_Source.txt", "r+");
    if (source == NULL) {
        printf("Could not find source file. Make sure it is in same folder group as this tool\n");
        return 1;
    }
    
    FILE *temp = fopen("temp.tmp", "w");
    if (temp == NULL) {
        printf("Failed to copy items.\n");
        return 1;
    }
    
    printf("Enter name of requested password: ");
    fflush(stdin);
    fgets(name, NAMESIZE, stdin);
    
    name[strlen(name) - 1] = '\0';
    
    while (!feof(source)) {
        if (fgets(line, MAXLINE, source) == NULL) {
            break;
        };
    
        for (int i = 0; line[i] != ':' && line[i] != '\n' && line[i] != '\r' && line[i] != '\t'; i++) {
            if ((line[i] - key) <= START) {
                check[i] = 127 - (line[i] - key);
                key++;
            } else {
                check[i] = line[i] - key;
                key++;
            }
        }
        
        
        if (strncmp(name, check, strlen(name)) == 0) {
            key = 0;
            printf("Match found!\n");
            printf("Enter password: ");
            fflush(stdin);
            fgets(password, PWSIZE, stdin);
            
            password[strlen(password) - 1] = '\0';
            
            for (int i = 0; name[i] != '\0'; i++) {
                if ((name[i] + key) > 126) {
                    new_line[i] = ((name[i] + key) % 126) + START;
                    key++;
                } else {
                    new_line[i] = name[i] + key;
                    key++;
                }
            }
            
            new_line[key] = ':';
            key++;
            new_line[key] = ' ';
            key++;
            
            for (int i = 0; i < strlen(password); i++) {
                if (password[i] + key > 126) {
                    new_line[key] = ((password[i] + key) % 126) + START;
                    key++;
                } else {
                    new_line[key] = password[i] + key;
                    key++;
                }
            }
            
            new_line[key] = '\0';
            printf("Updated %s with password %s\n", name, password);
            fprintf(temp, "%s\n", new_line);
            
        } else if (line[0] != '\n') {
            fprintf(temp, "%s", line);
        }
            
        key = 0;
        memset(line, 0, MAXLINE);
        memset(check, 0, NAMESIZE);
        memset(new_line, 0, MAXLINE);
        
    }
    
    fclose(source);
    fclose(temp);
    free(password);
    free(name);
    free(line);
    free(check);
    free(new_line);
    
    remove("Keychain_Source.txt");
    rename("temp.tmp", "Keychain_Source.txt");
    
    return 0;
    
}

int updaterandom() {
    
    char *name = calloc(NAMESIZE, sizeof(char));
    char *check = calloc(NAMESIZE, sizeof(char));
    char *line = calloc(MAXLINE, sizeof(char));
    char *password = calloc(PWSIZE, sizeof(char));
    char *new_line = calloc(MAXLINE, sizeof(char));
    int key = 0; /*function with take name, create a new password, and then combine them into a single line, key will track*/
    int usr_size;
    
    if (line == NULL || name == NULL || check == NULL || password == NULL) {
        printf("Failed to allocate memory for variables\n");
        return 1;
    }
    
    FILE *source = fopen("Keychain_Source.txt", "r+");
    if (source == NULL) {
        printf("Could not find source file. Make sure it is in same folder group as this tool\n");
        return 1;
    }
    
    FILE *temp = fopen("temp.tmp", "w");
    if (temp == NULL) {
        printf("Failed to copy items.\n");
        return 1;
    }
    
    printf("Enter name of requested password: ");
    fflush(stdin);
    fgets(name, NAMESIZE, stdin);
    
    name[strlen(name) - 1] = '\0';
    
    while (!feof(source)) {
        
         if (fgets(line, MAXLINE, source) == NULL) {
            break;
         };
        
        for (int i = 0; line[i] != ':' && line[i] != '\n' && line[i] != '\r' && line[i] != '\t'; i++) {
            if ((line[i] - key) <= START) {
                check[i] = 127 - (line[i] - key);
                key++;
            } else {
                check[i] = line[i] - key;
                key++;
            }
        }
        
        if (strncmp(name, check, strlen(name)) == 0) {
            key = 0;
            memset(line, 0, MAXLINE);
            printf("Match found!\n");
            printf("Enter password size (30 characters maximum): ");
            scanf("%d", &usr_size);
            srand(time(NULL));
            
            for (int i = 0; i < usr_size; i++) {
                char c = (rand() % 95) + 32;
                password[i] = c;
            }
            
            for (int i = 0; name[i] != '\0'; i++) {
                if ((name[i] + key) > 126) {
                    new_line[i] = ((name[i] + key) % 126) + START;
                    key++;
                } else {
                    new_line[i] = name[i] + key;
                    key++;
                }
            }
            
            new_line[key] = ':';
            key++;
            new_line[key] = ' ';
            key++;
            
            for (int i = 0; i < usr_size; i++) {
                if (password[i] + key > 126) {
                    new_line[key] = ((password[i] + key) % 126) + START;
                    key++;
                } else {
                    new_line[key] = password[i] + key;
                    key++;
                }
            }
            
            new_line[key] = '\0';
            printf("Updated %s with password %s\n", name, password);
            fprintf(temp, "%s\n", new_line);
            
        } else if (line[0] != ' ' && line[0] != '\n') {
            fprintf(temp, "%s", line);
        }
            
        key = 0;
        memset(line, 0, MAXLINE);
        memset(check, 0, NAMESIZE);
        
    }
    
    fclose(source);
    fclose(temp);
    free(password);
    free(name);
    free(line);
    free(check);
    free(new_line);
    
    remove("Keychain_Source.txt");
    rename("temp.tmp", "Keychain_Source.txt");
    
    return 0;

}
    

int delete() {
    
    char *name = calloc(NAMESIZE, sizeof(char));
    char *check = calloc(NAMESIZE, sizeof(char));
    char *line = calloc(MAXLINE, sizeof(char));
    int key = 0;
    
    FILE *source = fopen("Keychain_Source.txt", "r");
    FILE *temp = fopen("temp.tmp", "w");
    if (source == NULL || temp == NULL) {
        printf("Failed to open keychain files.");
        return 1;
    }
    
    if (name == NULL || check == NULL || line == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }
    
    printf("Enter name of password for deletion: ");
    fflush(stdin);
    fgets(name, NAMESIZE, stdin);
    
    name[strlen(name) - 1] = '\0';
    
    while (!feof(source)) {
        
        if (fgets(line, MAXLINE, source) == NULL) {
            break;
        };
        
        for (int i = 0; line[i] != ':' && line[i] != '\n' && line[i] != '\r'; i++) {
            if ((line[i] - key) < START) {
                check[i] =  127 - (START - (line[i] - key));
                key++;
            } else {
                check[i] = line[i] - key;
                key++;
            }
        }
        
        check[key] = '\0';
       
        if (strncmp(name, check, strlen(name)) != 0 && line[0] != ' ' && line[0] != '\n' && line[0] != '\t') {
            fprintf(temp, "%s", line);
        }
        
        key = 0;
        memset(line, 0, MAXLINE);
        memset(check, 0, NAMESIZE);
    
        
    }
    
    printf("Deleted %s\n", name);
    
    fclose(source);
    fclose(temp);
    free(name);
    free(check);
    free(line);
    
    remove("Keychain_Source.txt");
    rename("temp.tmp", "Keychain_Source.txt");
    
    return 0;
}

int cleanup() {
    
    char *line = calloc(MAXLINE, sizeof(char));
    char *new_line = calloc(MAXLINE, sizeof(char));
    
    if (line == NULL || new_line == NULL) {
        printf("Failed to allocate memory for variables\n");
        return 1;
    }
    
    FILE *source = fopen("Keychain_Source.txt", "r+");
    FILE *temp = fopen("temp.tmp", "w");
    
    while(!feof(source)) {
        
        if (fgets(line, MAXLINE, source) == NULL) {
            break;
        };
        
        if (line[0] != '\n' && line[0] != ' ' && line[0] != '\r' && line[0] != '\t') {
            for (int i = 0; line[i] != '\0'; i++)
                new_line[i] = line[i];
            fprintf(temp, "%s\n", new_line);
        }
        
        memset(line, 0, MAXLINE);
        memset(new_line, 0, MAXLINE);
    }
    
    printf("Clean up process complete.\n");
    
    free(line);
    free(new_line);
    fclose(source);
    fclose(temp);
    remove("Keychain_Source.txt");
    rename("temp.tmp", "Keychain_Source.txt");
    
    return 0;
    
}


int main(int argc, const char * argv[]) {
    int state;
    int pin = 1570; /*user's pin number*/
    int pin_check = 0;
    int attempts = 2;
    
    while (attempts >= 0) {
        printf("Enter your pin number: ");
        scanf("%d", &pin_check);
        if (pin == pin_check) {
            printf("Logging in\n");
            break;
        } else if (attempts == 0) {
            printf("Program exit. Failed to authenticate.\n");
            return 2;
        } else {
            printf("Incorrect pin number. Try again. %d attempts remaining\n", attempts);
            attempts--;
        }
    }
    
    while (1) {
        printf("\n*********** Welcome to the keychain tool ***********\n\n");
        printf("*************** MAIN MENU SELCTION *****************\n\n");
        printf("Make a selction below:\n");
        printf("'1'  to view readme file.\n");
        printf("'2'  to create a new random password.\n");
        printf("'3'  to create a user defined password.\n");
        printf("'4'  to retrieve a password via name.\n");
        printf("'5'  to update a random password via name.\n");
        printf("'6'  to update a user defined password via name.\n");
        printf("'7'  to delete a password via name.\n");
        printf("'8'  to run keychain maintenance.\n");
        printf("'9'  to exit program\n");
        printf("Input: \n");
        scanf("%d", &state);
    
        if (state == 9)
            break;
    
        if (state == 1) {
            readme();
        } else if (state == 2) {
            addrandom();
        } else if (state == 3) {
            adddefined();
        } else if (state == 4) {
            retrieve();
        } else if (state == 5) {
            updaterandom();
        } else if (state == 6) {
            updatedefined();
        } else if (state == 7) {
            delete();
        } else if (state == 8) {
            cleanup();
        } else {
            printf("Undefined command select numbers 1 through 8");
        }
        
    }
    
    return 0;
}
