#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h> 

/* logs new entry in change-log */
void append_change_log(const char* file_name, const char* operation, int lines) {
    /* get time struct*/
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    FILE* cl = fopen("change-log.txt", "a");
    if (cl == NULL) {
        perror("File failed to open");
        return;
    }

    /* [dd-mm-yy hh:mm:ss] <operation>: <filename> (<lines>) */
    fprintf(cl, "[%d-%02d-%02d %02d:%02d:%02d] %s: %s (%d)\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, operation, file_name, lines);
    
    fclose(cl);
}

/* gets number of lines in file */
int get_lines(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        perror("File failed to open");
        return -1;
    }

    /* get file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    /* allocate memory for file contents */
    char* file_text = (char*)malloc((size + 1) * sizeof(char));
    if (file_text == NULL) {
        perror("Memory allocation failed");
        return -1;
    }

    long read = fread(file_text, sizeof(char), size, file);
    if (read != size) {
        perror("File could not be read");
        return -1;
    }

    /* ensure null terminator */
    file_text[size] = '\0';

    fclose(file);

    /* count lines */
    int i = 0;
    int l = 0;
    while (file_text[i] != '\0') {
        
        if (file_text[i] == '\n') {
            l++;
        }

        i++;
    }

    /* actual num lines in file (first line ommitted)
       maintains empty file lines = 0 */

    if (i != 0) {
        l++;
    }    

    return l;
}

/* creates a file. if file exists contents are overridden */
void create_file(const char *file_name) {
    FILE* create_result = fopen(file_name, "w");
    if (create_result == NULL) {
        perror("Failed to create file");
        return;
    }

    fclose(create_result);

    append_change_log(file_name, "CREATE", get_lines(file_name));
}

/* deletes a file */
void delete_file(const char *file_name) {
    const int delete_result = remove(file_name);
    if (delete_result != 0) {
        perror("Failed to delete file");
        return;
    }

    append_change_log(file_name, "DELETE", 0);
}

/* copies file contents from src to dest files*/
void copy_file(const char *src_file_name, const char *dest_file_name) {
    FILE *src = fopen(src_file_name, "r");
    FILE *dest = fopen(dest_file_name, "w");
    
    if (src == NULL) {
        perror("Failed to open source file");
        return;
    }
    if (dest == NULL) {
        perror("Failed to open destination file");
        return;
    }

    /* allocate memory for src contents */
    fseek(src, 0, SEEK_END);
    const long size = ftell(src);
    char* buffer = (char*)malloc((size + 1) * sizeof(char));
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return;
    }
    rewind(src);

    /* read file */
    long read = fread(buffer, 1, size, src);
    if (read != size) {
        perror("File could not be read");
        return;
    }
    
    fclose(src);

    /* write dest file */
    fputs(buffer, dest);

    fclose(dest);

    free(buffer);

    append_change_log(src_file_name, "COPY-FROM", get_lines(src_file_name));
    append_change_log(dest_file_name, "COPY-TO", get_lines(dest_file_name));
}

/* prints a line number to the screen. ensures three digits at all times */
void print_linenum(int i) {
    printf("%d%d%d | ", (i % 1000)/100,(i % 100)/10, i % 10);
}

/* prints file contents to the console */
void print_file(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        perror("File failed to open");
        return;
    }

    /* allocate memory for file contents */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char* file_text = (char*)malloc((size + 1) * sizeof(char));
    
    /* read file to buffer */
    long read = fread(file_text, sizeof(char), size, file);
    if (read != size) {
        perror("File could not be read");
        return;
    }

    /* ensures null termination */
    file_text[size] = '\0';

    fclose(file);
    
    /* header */
    printf("%s\n----------\n", file_name);

    /* prints file contents */
    int i = 0;
    int l = 0;
    char c = '\n'; /* ensures line num 0 is printed */
    while (file_text[i] != '\0') {
        if (c == '\n') {
            print_linenum(l);
            l++;
        }

        c = file_text[i];
        printf("%c", c);
        i++;
    }
    printf("\n");

    free(file_text);
}

/* appends a line of text to a file */
void append_line(const char* file_name, const char* line) {
    FILE* file = fopen(file_name, "a");
    if (file == NULL) {
        perror("File failed to open");
        return;
    }

    fputs(line, file);
    
    fclose(file);

    append_change_log(file_name, "APPEND LINE", get_lines(file_name));
}

/* deletes a given line from a file */
void delete_line(const char* file_name, int line) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        perror("File failed to open");
        return;
    }

    /* get file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* file_text = (char*)malloc((size + 1) * sizeof(char));
    if (file_text == NULL) {
        perror("Memory allocation failed.");
        return;
    }

    /* read text to buffer */
    long read = fread(file_text, sizeof(char), size, file);
    if (read != size) {
        perror("File could not be read.");
        return;
    }

    /* ensure null termination */
    file_text[size] = '\0';

    fclose(file);

    /* allocate memory for buffer to store new contents */
    char* buffer = (char*)malloc((size + 1) * sizeof(char));
    if (buffer == NULL) {
        perror("Memory allocation failed");
        return;
    }

    /* copy file contents without new line */
    int l = 0; /* line number 0 indexed */
    int i1 = 0; /* position in file text */
    int i2 = 0; /* position in buffer */
    while (file_text[i1] != '\0') {
        if (l != line) {
            /* only copy if not the deleted line */
            buffer[i2] = file_text[i1];
            i2++;
        }
        if (file_text[i1] == '\n') {
            l++;
        }

        i1++;
    }

    /* ensure null termination */
    buffer[i2] = '\0';

    /* write file with deleted line */
    file = fopen(file_name, "w");
    fputs(buffer, file);
    fclose(file);


    free(buffer);
    free(file_text);

    append_change_log(file_name, "DELETE LINE", get_lines(file_name));
}

/* inserts a line at a given number */
void insert_line(const char* file_name, const char* line, int num) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        perror("File failed to open");
        return;
    }

    /* get file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    /* allocate memory */
    char* file_text = (char*)malloc((size + 1) * sizeof(char));
    if (file_text == NULL) {
        perror("Memory allocation failed");
        return;
    }
 
    long read = fread(file_text, sizeof(char), size, file);
    if (read != size) {
        perror("File could not be read");
        return;
    }

    /* ensure null termination */
    file_text[size] = '\0';
    
    fclose(file);

    /* allocate memory for new buffer */
    char* buffer = (char*)malloc((size + 1) * sizeof(char) + strlen(line + 2 /* experimental value, unclear why required */) * sizeof(char));

    /* write the buffer with line inserted */
    int l = 0; /* line number 0 indexed */
    int i = 0; /* position in file text */
    int j = 0; /* position in buffer */
    while (file_text[i] != '\0') {
        if (l == num) {

            /* insert the line into buffer */
            int k = 0; /* index in line */
            while (line[k] != '\0') {
                buffer[j] = line[k];
                j++;
                k++;
            }
            l++; 
            i--; /* skip back to \n char */
        }
        else {
            buffer[j] = file_text[i];
            j++;
        }

        if (file_text[i] == '\n') {
            l++;
        }
        i++;
    }

    /* ensure null termination */
    buffer[j] = '\0';

    /* write file */
    file = fopen(file_name, "w");
    fputs(buffer, file);
    fclose(file);

    free(buffer);
    free(file_text);

    append_change_log(file_name, "INSERT LINE", get_lines(file_name));
}

/* shows a specific line in a file */
void show_line(const char* file_name, int num) {
    FILE* file = fopen(file_name, "r"); 
    if (file == NULL) {
        perror("File failed to open");
        return;
    }

    /* get file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    /* allocate memory for contents */
    char* file_text = (char*)malloc((size + 1) * sizeof(char));
    if (file_text == NULL) {
        perror("Memory allocation failed");
        return;
    }

    /* read file */
    long read = fread(file_text, sizeof(char), size, file);
    if (read != size) {
        perror("File could not be read");
        return;
    }

    /* ensure null termination */
    file_text[size] = '\0';

    fclose(file);
    
    /* header */
    printf("%s\n----------\n", file_name);
    print_linenum(num);

    /* print line */
    int i = 0;
    int l = 0;
    char c;
    while (file_text[i] != '\0') {
        c = file_text[i];

        if (l == num) {
            printf("%c", c);
        }
        
        if (c == '\n') {
            l++;
        }
        i++;
    }
    printf("\n");

    free(file_text);
}

/* lists text files in working directory */
void list_dir() {
    struct dirent *de; /* memory for entries*/
    DIR *dr = opendir("."); /* read directory */
    if (dr == NULL) 
    { 
        perror("Could not open current directory" ); 
        return; 
    } 
  
    while (1) {
        de = readdir(dr);
        if (de == NULL) { break; } /* no more entries */
        
        /* get file name */
        const char* name = de->d_name;

        /* check file extension */
        int lenname = strlen(name);
        int lenext = strlen(".txt");
        if (!(lenname >= lenext && strcmp(name + lenname - lenext, ".txt") == 0)) { continue; } /* do not print non .txt files */
        
        printf("%s  ", name);
    }

    printf("\n");
  
    closedir(dr);      
}

/* show the number of lines in a file */
void show_linenum(const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        perror("File failed to open");
        return;
    }

    /* read file */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    /* allocate memory for contents */
    char* file_text = (char*)malloc((size + 1) * sizeof(char));
    if (file_text == NULL) {
        perror("Memory allocation failed");
        return;
    }

    /* read file */
    long read = fread(file_text, sizeof(char), size, file);
    if (read != size) {
        perror("File could not be read");
        return;
    }

    /* ensure null termination */
    file_text[size] = '\0';

    fclose(file);

    /* count line number */
    int i = 0, l = 0;
    while(file_text[i] != '\0') {
        if (file_text[i] == '\n') {
            l++;
        }
        i++;
    }

    /* if file is not empty (\n not counted for first line)*/
    if (i != 0) {
        l++;
    }

    /* <filename>: <num lines>*/
    printf("%s: %d\n", file_name, l);
}

/* rename a file */
void rename_file(const char* old, const char* new) {
    rename(old, new);
    append_change_log(old, "RENAME-FROM", get_lines(new));
    append_change_log(new, "RENAME-TO", get_lines(new));
}