void create_file(const char* filename);

void delete_file(const char* filename);

void copy_file(const char* src_file_name, const char* dest_file_name);

void print_file(const char* file_name);

void append_line(const char* file_name, const char* line);

void delete_line(const char* file_name, int line);

void insert_line(const char* file_name, const char* line, int num);

void show_line(const char* file_name, int num);

void show_linenum(const char* file_name);

void rename_file(const char* old, const char* new);

void list_dir();