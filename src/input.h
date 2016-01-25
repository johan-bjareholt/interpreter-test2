#ifndef INPUT_H
#define INPUT_H

void input_set_interactive();
bool input_set_file(const char* filename);

char* input_getline();
void input_consume_line();
int input_line_nr();

void input_close();

#endif
