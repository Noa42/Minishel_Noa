
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_data
{
    char    **env;
	char	*input;
    char    **array_input;
    char    **array_var;
}			t_data;

// SIGNALS
void		signals(void);
void		handle_ctrl_c(int signal);

// PARSING
void parsing(t_data *data);

// UTILS
void		print_array(char **array);
void		free_array(char **array);
char **copy_alloc_array(char **array);
int array_len(char **array);

// EXECUTION
void execution (t_data *data);
void ft_pwd(void);
void ft_echo(char **array_input);
void builtins(t_data *data);
void ft_cd(t_data *data);
void ft_env(t_data *data);
void ft_export(t_data *data);
void insert_var(char **array, char *var_name, char *var_value);


// PATH
char		*get_path_line(char **env);
char		*get_path(char *command, char **env);

#endif