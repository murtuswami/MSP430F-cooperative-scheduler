#ifndef COMMAND_H
#define COMMAND_H

typedef void (*command_handler_t)(const char *args);

typedef struct {
    const char *name;
    command_handler_t handler;
} command_entry_t;

void parse_command(const char*); 
void led_command(const char*);
#endif
