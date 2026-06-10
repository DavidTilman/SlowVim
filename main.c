#include <stdio.h>
#include "fileops.h"
#include <string.h>

int main(void) {

    char* defualt_file_name = NULL; /* user can set a defualt */

    /* broken on quit */
    while (1) {
        /* get command */
        char command[100];
        printf("cmd > ");
        scanf("%s", command);
        
        /* check command, get args, execute command */

        /* help */
        if (strcmp(command, "help") == 0 || strcmp(command, "h") == 0) {
            printf("NOTE: setfile command sets the default filename (fnm) for continuous editing\n");
            printf("command (cmd)│ alt │ arguments (entered later)        │ description\n");
            printf("─────────────┼─────┼──────────────────────────────────┼───────────────────────────────────────────────────\n");
            printf("help         │ h   │                                  │ shows this message\n");
            printf("setfile      │ set │ <fnm │ file name to set>         │ sets the defualt filename\n");
            printf("unsetfile    │ uns │                                  │ removes the defult file name\n");
            printf("create       │ crt │ <fnm │ file name to create>      │ creates a file\n");
            printf("create       │ crt │ <fnm │ file name to create>      │ creates a file\n");
            printf("deletefile   │ dfl │ <fnm │ file name to deleted>     │ deletes a file\n");
            printf("renamefile   │ rnm │ <fnm │ file name to renamed>     │\n");
            printf("             │     │ <new │ new file name>            │ renames a file from the old name to the new name\n");
            printf("copy         │ cpy │ <src │ source file name>         │\n");
            printf("             │     │ <dst │ destination file name>    │ copies contents form src file to dest file\n");
            printf("show         │ shw │ <fnm │ file to be shown>         │ shows file contents\n");
            printf("append       │ app │ <fnm │ file to append to>        │\n");
            printf("             │     │ <lne │ text to append>           │ appends a line to the end of a file\n");
            printf("deleteline   │ dln │ <fnm │ file to delete from>      │\n");
            printf("             │     │ <nbr │ line number to delete>    │ deletes a specific line from a file\n");
            printf("insertline   │ iln │ <fnm │ file to insert into>      │\n");
            printf("             │     │ <lne │ text to insert>           │\n");
            printf("             │     │ <nbr │ line number to insert to> │ inserts a line into a file\n");
            printf("showline     │ sln │ <fnm │ file to show from>        │\n");
            printf("             │     │ <nbr │ line number to show>      │ shows a specific line in a file\n");
            printf("listdir      │ dir │                                  │ shows the working directory (text files available)\n");
            printf("quit         │ q   │                                  │ quits the application\n");
        }  
        /* setfile */
        else if (strcmp(command, "setfile") == 0 || strcmp(command, "set") == 0) {
            printf("fnm > ");
            char file_name[100];
            scanf("%s", file_name);
            defualt_file_name = file_name;
        }  
        /* unsetfile */
        else if (strcmp(command, "unsetfile") == 0 || strcmp(command, "uns") == 0) {
            defualt_file_name = NULL;
        }
        /* create */
        else if (strcmp(command, "create") == 0 || strcmp(command, "crt") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);

                int lenfn = strlen(file_name);
                int lenext = strlen(".txt");

                if (lenfn >= lenext && strcmp(file_name + lenfn - lenext, ".txt") == 0) {
                    create_file(file_name);
                }
                else {
                    printf("Failed to create file: File extension invalid.");
                    continue;
                }
            }
            else {
                int lenfn = strlen(defualt_file_name);
                int lenext = strlen(".txt");

                if (lenfn >= lenext && strcmp(defualt_file_name + lenfn - lenext, ".txt") == 0) {
                    create_file(defualt_file_name);
                }
                else {
                    printf("Failed to create file: File extension invalid.");
                    continue;
                }
            }
        } 
        /* deletefile */
        else if (strcmp(command, "deletefile") == 0 || strcmp(command, "dfl") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);

                int lenfn = strlen(file_name);
                int lenext = strlen(".txt");

                if (lenfn >= lenext && strcmp(file_name + lenfn - lenext, ".txt") == 0) {
                    create_file(file_name);
                }
                else {
                    printf("Failed to create file: File extension invalid.");
                    continue;
                }
            }
            else {
                int lenfn = strlen(defualt_file_name);
                int lenext = strlen(".txt");

                if (lenfn >= lenext && strcmp(defualt_file_name + lenfn - lenext, ".txt") == 0) {
                    delete_file(defualt_file_name);
                }
                else {
                    printf("Failed to delete file: File extension invalid.");
                    continue;
                }
            }
        } 
        /* renamefile */
        else if (strcmp(command, "renamefile") == 0 || strcmp(command, "rnm") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);

                char new_file_name[100];
                printf("new > ");
                scanf("%s", new_file_name);

                rename_file(file_name, new_file_name);
            }
            else {
                char new_file_name[100];
                printf("new > ");
                scanf("%s", new_file_name);

                rename_file(defualt_file_name, new_file_name);

                defualt_file_name = new_file_name;
            }
        } 
        /* copy */
        else if (strcmp(command, "copy") == 0 || strcmp(command, "cpy") == 0) {
            printf("src > ");
            char src_file_name[100];
            scanf("%s", src_file_name);

            int lensrc = strlen(src_file_name);
            
            printf("dst > ");
            char dest_file_name[100];
            scanf("%s", dest_file_name);

            int lendest = strlen(dest_file_name);

            int lenext = strlen(".txt");

            if (!(lensrc >= lenext && strcmp(src_file_name + lensrc - lenext, ".txt") == 0)) {
                printf("Failed to copy file: Source file extenstion invalid\n");
                continue;
            }

            if (!(lendest >= lenext && strcmp(dest_file_name + lendest - lenext, ".txt") == 0)) {
                printf("Failed to copy file: Destination file extenstion invalid\n");   
                continue;
            }

            copy_file(src_file_name, dest_file_name);
        } 
        /* show */
        else if (strcmp(command, "show") == 0 || strcmp(command, "shw") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);
                print_file(file_name);
            }
            else {
                print_file(defualt_file_name);
            }
        } else if (strcmp(command, "append") == 0 || strcmp(command, "app") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);

                char line[100];
                printf("lne > ");
                char temp;
                scanf("%c",&temp);
                fgets(line, 100, stdin);

                append_line(file_name, line);
            }
            else {
                printf("lne > ");
                char line[100];
                char temp;
                scanf("%c",&temp);
                fgets(line, 100, stdin);

                append_line(defualt_file_name, line);
            } 
        } 
        /* deleteline */
        else if (strcmp(command, "deleteline") == 0 || strcmp(command, "dln") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);
                printf("nbr > ");
                int line;
                scanf("%d", &line);

                delete_line(file_name, line);
            }
            else {
                printf("nbr > ");
                int line;
                scanf("%d", &line);

                delete_line(defualt_file_name, line);
            } 
        } 
        /* insertline */
        else if (strcmp(command, "insertline") == 0 || strcmp(command, "iln") == 0) {
            if (defualt_file_name == NULL) {
                char file_name[100];
                printf("fnm > ");
                scanf("%s", file_name);

                printf("lne > ");
                char line[100];
                char temp;
                scanf("%c",&temp);
                fgets(line, 100, stdin);

                printf("nbr > ");
                int num;
                scanf("%d", &num);

                insert_line(file_name, line, num);
            }
            else {
                printf("lne > ");
                char line[100];
                char temp;
                scanf("%c",&temp);
                fgets(line, 100, stdin);
                
                printf("nbr > ");
                int num;
                scanf("%d", &num);

                insert_line(defualt_file_name, line, num);
            }
        } 
        /* showline */
        else if (strcmp(command, "showline") == 0 || strcmp(command, "sln") == 0) {
            char file_name[100];
            if (defualt_file_name == NULL) {
                printf("fnm > ");
                scanf("%s", file_name);
                printf("nbr > ");
                int num;
                scanf("%d", &num);
                show_line(file_name, num);
            }
            else {
                printf("nbr > ");
                int num;
                scanf("%d", &num);

                show_line(defualt_file_name, num);
            }
        } 
        /* showlines */
        else if (strcmp(command, "showlines") == 0 || strcmp(command, "sls") == 0) {
            char file_name[100];
            if (defualt_file_name == NULL) {
                printf("fnm > ");
                scanf("%s", file_name);
                show_linenum(file_name);
            }
            else {
                show_linenum(file_name);
            } 
        }
        /* listdir */
        else if (strcmp(command, "listdir") == 0 || strcmp(command, "dir") == 0) {
            list_dir();
        }
        /* quit */
        else if (strcmp(command, "quit") == 0 || strcmp(command, "q") == 0) {
            return 0;
        }

    }
}
