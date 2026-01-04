#ifndef COMMAND_H
#define COMMAND_H
#include <stdint.h>


#define MAX_COMMAND_ARGS 3
#define MAX_SC_LENGTH 10



typedef void (*command_handler_t)(char tokens[][MAX_SC_LENGTH], uint16_t count);

typedef struct {
    const char *name;
    command_handler_t handler;
} command_entry_t;



void parse_command(char tokens[][MAX_SC_LENGTH],uint16_t count); 
uint16_t tokenize(const char* command, char tokens[][MAX_SC_LENGTH]);
void dispatch_command(
    char tokens[][MAX_SC_LENGTH],
    uint16_t count,
    const command_entry_t* table,
    uint8_t table_size
);

#endif
