/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CONFIG_INTERFACE_H    /* Guard against multiple inclusion */
#define _CONFIG_INTERFACE_H
typedef int8_t(*execute_command)(int8_t *);
int8_t parse_cmd(int8_t *cmdline);
int8_t get_time_cmd(int8_t *);
int8_t set_time_cmd(int8_t *);
int8_t whoareyou_cmd(int8_t *);

execute_command search_cmd(int8_t *, uint8_t);

typedef struct {
    int8_t *cmd_name;
    int8_t(*cmd_function)(int8_t *);
} command_t;

#define COMMAND(NAME) { #NAME , NAME  ## _cmd }

command_t commands[] = {
    COMMAND(get_time),
    COMMAND(set_time),
    COMMAND(whoareyou), {
        NULL, NULL
    }
};
#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
