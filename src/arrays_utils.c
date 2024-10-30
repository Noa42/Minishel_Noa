#include "../include/minishell.h"

int tokens_len(t_token **token_array)
{
	int i;

	i = 0;
	while (token_array[i] != NULL)
		i++;
	return (i);
}

t_token	*new_cmd(char **array_cmds, int fd_in, int fd_out, t_data *data)
{
	t_token	*new_token;

	// Alocamos memoria para el token
	new_token = malloc(sizeof(t_token));
	// Alocamos memoria para el cmd*
	new_token->cmd = malloc(sizeof(t_cmd));
	// El resto de punteros los inicializamos a NULL
	new_token->pipe = NULL;
	new_token->redir = NULL;
	//Le asignamos el índice
	new_token->index = tokens_len(data->token_array);
	//Le ponemos el tipo de token
	new_token->token_type = "CMD";
	// Llenamos el cmd
	new_token->cmd->array_cmds = array_cmds;
	new_token->cmd->fd_in = fd_in;
	new_token->cmd->fd_out = fd_out;
	new_token->cmd->data = data;
	new_token->next = NULL; // quitar esto si lo hacemos con arrays
	// Devolvemos el token
	return (new_token);
}
t_token* new_pipe(int fd_in, int fd_out, t_data *data)
{
	t_token	*new_token;

	// Alocamos memoria para el token
	new_token = malloc(sizeof(t_token));
	// Alocamos memoria para el pipe*
	new_token->pipe = malloc(sizeof(t_pipe));
	// El resto de punteros los inicializamos a NULL
	new_token->cmd = NULL;
	new_token->redir = NULL;
	//Le asignamos el índice
	new_token->index = tokens_len(data->token_array);
	//Le ponemos el tipo de token
	new_token->token_type = "PIPE";
	// Llenamos el pipe
	new_token->pipe->fd_in = fd_in;
	new_token->pipe->fd_out = fd_out;
	new_token->pipe->data = data;
	new_token->next = NULL; // quitar esto si lo hacemos con arrays
	// Devolvemos el token
	return (new_token);
}

t_token *new_redir(int type, char *in_name, char *out_name, int fd_in, int fd_out, t_data *data)
{
	t_token	*new_token;

	// Alocamos memoria para el token
	new_token = malloc(sizeof(t_token));
	// Alocamos memoria para el redir*
	new_token->redir = malloc(sizeof(t_redir));
	// El resto de punteros los inicializamos a NULL
	new_token->cmd = NULL;
	new_token->pipe = NULL;
	//Le asignamos el índice
	new_token->index = tokens_len(data->token_array);
	//Le ponemos el tipo de token
	new_token->token_type = "REDIR";
	// Llenamos el redir
	new_token->redir->type = type;
	new_token->redir->in_name = in_name;
	new_token->redir->out_name = out_name;
	new_token->redir->fd_in = fd_in;
	new_token->redir->fd_out = fd_out;
	new_token->redir->data = data;
	new_token->next = NULL; // quitar esto si lo hacemos con arrays
	// Devolvemos el token
	return (new_token);
}

void print_cmd(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->array_cmds[i] != NULL)
	{
		printf("-array_cmds: %i: %s\n", i, cmd->array_cmds[i]);
		i++;
	}
	printf("-fd_in: %i\n", cmd->fd_in);
	printf("-fd_out: %i\n", cmd->fd_out);
}
void print_pipe(t_pipe *pipe)
{
	printf("-fd_in: %i\n", pipe->fd_in);
	printf("-fd_out: %i\n", pipe->fd_out);
}
void print_redir(t_redir *redir)
{
	printf("-type: %i\n", redir->type);
	printf("-in_name: %s\n", redir->in_name);
	printf("-out_name: %s\n", redir->out_name);
	printf("-fd_in: %i\n", redir->fd_in);
	printf("-fd_out: %i\n", redir->fd_out);
}

void	print_tokens(t_token **token_array)
{
	int	i;

    printf("++++ARRAY DE TOKENS++++\n");
	i = 0;
	if (token_array == NULL)
	{
		printf("Token Array is empty\n");
		return ;
	}
	while (token_array[i] != NULL)
	{
		printf("\n+Token [%i]: %s\n", token_array[i]->index, token_array[i]->token_type);
		if(ft_strcmp(token_array[i]->token_type, "CMD") == 0)
			print_cmd(token_array[i]->cmd);
		if(ft_strcmp(token_array[i]->token_type, "PIPE") == 0)
			print_pipe(token_array[i]->pipe);
		if(ft_strcmp(token_array[i]->token_type, "REDIR") == 0)
			print_redir(token_array[i]->redir);
		i++;
		printf("\n");
	}
}

void free_cmd(t_cmd *cmd)
{
	//Si alocamos memoria para alguno de los campos este es el lugar para liberarla. Puede ser pruto de un split o un strjoin
	free(cmd);
}
void free_pipe(t_pipe *pipe)
{
	//Si alocamos memoria para alguno de los campos este es el lugar para liberarla. Puede ser pruto de un split o un strjoin
	free(pipe);
}
void free_redir(t_redir *redir)
{
	//Si alocamos memoria para alguno de los campos este es el lugar para liberarla. Puede ser pruto de un split o un strjoin
	free(redir);
}
void free_token_array(t_token **token_array)
{
	int i;

	i = 0;
	while (token_array[i] != NULL)
	{
		if(ft_strcmp(token_array[i]->token_type, "CMD") == 0)
			free_cmd(token_array[i]->cmd);
		if(ft_strcmp(token_array[i]->token_type, "PIPE") == 0)
			free_pipe(token_array[i]->pipe);
		if(ft_strcmp(token_array[i]->token_type, "REDIR") == 0)
			free_redir(token_array[i]->redir);
		free(token_array[i]);
		printf("Token %i liberado\n", i);
		i++;
	}
	free(token_array);
	///token_array = NULL. No estoy seguro pero no sé muy bien por qué esto solo funciona si lo hago fuera de la función
}

void	arrays(t_data *data)
{
    printf("\nPRUEBA ARRAYS\n\n");

	char *array_cmds[5];
    array_cmds[0] = "ls";
    array_cmds[1] = "-l";
    array_cmds[2] = "grep";
    array_cmds[3] = "hola";
    array_cmds[4] = NULL;

    char *array_cmds2[5];
    array_cmds2[0] = "ls";
    array_cmds2[1] = "-l";
    array_cmds2[2] = "grep";
    array_cmds2[3] = "adios";
    array_cmds2[4] = NULL;

	data->token_array = malloc(sizeof(t_token) * 5);
	data->token_array[0] = new_cmd(array_cmds, 0, 1, data);
    data->token_array[1] = new_cmd(array_cmds2, 0, 1, data);
	data->token_array[2] = new_pipe(0, 1, data);
	data->token_array[3] = new_redir(1, "in.txt", "out.txt", 0, 1, data);

	data->token_array[4] = NULL;
	print_tokens(data->token_array);
	printf("\n DESPUES DE LIMPIEZA\n");
	free_token_array(data->token_array);
	data->token_array = NULL;
	if(data->token_array == NULL)
		printf("Token array is NULL\n");
	print_tokens(data->token_array);
}
