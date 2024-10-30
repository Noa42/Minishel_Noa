
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_data	t_data;
typedef struct s_cmd	t_cmd;
typedef struct s_pipe	t_pipe;
typedef struct s_redir	t_redir;

// typedef enum e_token_type
// {
// 	CMD,
// 	PIPE,
// 	REDIR
// }t_token_type;

typedef struct s_cmd
{
	char				**array_cmds;
	// flag args?
	int					fd_in;
	int					fd_out;
	t_data				*data;
}						t_cmd;

typedef struct s_pipe
{
	int					fd_in;
    int					fd_out;
	t_data				*data;
}						t_pipe;

typedef struct s_redir
{
	int					type;
	char				*in_name;
	char				*out_name;
	int					fd_in;
	int					fd_out;
	t_data				*data;
}						t_redir;

typedef struct s_token
{
	int					index;
	char*		token_type;
	t_cmd				*cmd;
	t_pipe				*pipe;
	t_redir				*redir;
	struct s_token		*next;
}						t_token;

typedef struct s_data
{
	char				**env;
	char				*input;
	char				**array_input;
	char				**array_var;
	int					exit_status;
	t_token				*token_list;//Borrar si al final usamos arrays xD
    t_token             **token_array;
}						t_data;
//PRUEBAS
void					listas(t_data *data);
void					arrays(t_data *data);

// SIGNALS
void					signals(void);
void					handle_ctrl_c(int signal);

// PARSING
void					parsing(t_data *data);

// UTILS
void					print_array(char **array);
void					free_array(char **array);
char					**copy_alloc_array(char **array);
int						array_len(char **array);
void					sort_strings(char **array, int size);
void					swap(char **a, char **b);
int						ft_strcmp(const char *s1, const char *s2);
char					**realloc_elonged_array(char **src_array);

// EXECUTION
void					execution(t_data *data);
void					builtins(t_data *data);

// SRC //BUILTINS
// FT_ECHO
int						has_variable(char *input);
char					*get_exp_var(char *s, int i);
void					print_expanded(char *input, char **env);
void					ft_echo(t_data *data);
// FT_PWD
void					ft_pwd(void);
// FT_CD
void					ft_cd_home(t_data *data, char *oldpwd, char *pwd);
void					ft_cd(t_data *data);
// FT_ENV
void					ft_env(t_data *data);
// FT_EXPORT
void					print_export(char **array_var);
void					ft_export(t_data *data);
// FT_UNSET
void					ft_unset(t_data *data);
void					delete_var(char **array, char *var_name);
// BUILDINS_UTILS
char					*ft_var_name(char *name_value);
char					*ft_var_value(char *name_value);
char					*ft_getenv(char *var_name, char **env);
char					**insert_var(char **array, char *var_name,
							char *var_value);

// PATH
char					*get_path_line(char **env);
char					*get_path(char *command, char **env);

#endif