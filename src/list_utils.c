#include "../include/minishell.h"

int	token_list_size(t_token *head)
{
	int		len;
	t_token	*p;

	if (head == NULL)
		return (0);
	len = 1;
	p = head;
	while (p -> next != NULL)
	{
		p = p -> next;
		len ++;
	}
	return (len);
}

t_token *new_cmd_token(char **array_cmds, int fd_in, int fd_out, t_data *data)
{
	t_token	*token;
	//alocamos el token
	token = malloc(sizeof(t_token));
	//asignamos el index
	token->index = token_list_size(data->token_list);
	//asignamos el tipo
	token->token_type = "CMD";
	//alocamos el cmd
	token->cmd = malloc(sizeof(t_cmd));
	//ponemos los otros punteros en null
	token->pipe = NULL;
	token->redir = NULL;
	//llenamos el cmd
	token->cmd->array_cmds = array_cmds;
	token->cmd->fd_in = fd_in;
	token->cmd->fd_out = fd_out;
	token->cmd->data = data;
	token->next = NULL;
	return (token);
}

t_token *new_pipe_token(int fd_in, int fd_out, t_data *data)
{
	t_token	*token;
	//alocamos el token
	token = malloc(sizeof(t_token));
	//asignamos el index
	token->index = token_list_size(data->token_list);
	//asignamos el tipo
	token->token_type = "PIPE";
	//alocamos el pipe
	token->pipe = malloc(sizeof(t_pipe));
	//ponemos los otros punteros en null
	token->cmd = NULL;
	token->redir = NULL;
	token->pipe->fd_in = fd_in;
	token->pipe->fd_out = fd_out;
	token->pipe->data = data;
	token->next = NULL;
	return (token);
}
t_token *new_redir_token(int type, char *in_name, char *out_name, int fd_in, int fd_out, t_data *data)
{
	t_token	*token;
	//alocamos el token
	token = malloc(sizeof(t_token));
	//asignamos el index
	token->index = token_list_size(data->token_list);
	//asignamos el tipo
	token->token_type = "REDIR";
	//alocamos el redir
	token->redir = malloc(sizeof(t_redir));
	//ponemos los otros punteros en null
	token->cmd = NULL;
	token->pipe = NULL;
	//llenamos el redir
	token->redir->type = type;
	token->redir->in_name = in_name;
	token->redir->out_name = out_name;
	token->redir->fd_in = fd_in;
	token->redir->fd_out = fd_out;
	token->redir->data = data;
	token->next = NULL;
	return (token);
}

t_token	*token_list_last(t_token *head)
{
	t_token	*p;

	p = head;
	if (head == NULL)
		return (NULL);
	while (p -> next)
	{
		p = p -> next;
	}
	return (p);
}
void	token_list_add(t_token **head, t_token *new)
{
	t_token	*last;

	if (*head == NULL)
		*head = new;
	else
	{
		last = token_list_last(*head);
		last -> next = new;
	}
}
int token_type(t_token *token)
{
	if (token->cmd != NULL)
		return (0);
	else if (token->pipe != NULL)
		return (1);
	else if (token->redir != NULL)
		return (2);
	return (-1);
}
void print_cmd_token(t_token *token)
{
	int i;
	i = 0;
	printf("+Token[%i]: %s\n", token->index, token->token_type);
	while (token->cmd->array_cmds[i] != NULL)
	{
		printf("-cmd[%i]: %s\n", i, token->cmd->array_cmds[i]);
		i++;
	}
	printf("-fd_in: %i\n", token->cmd->fd_in);
	printf("-fd_out: %i\n", token->cmd->fd_out);
}
void print_pipe_token(t_token *token)
{
	printf("+Token[%i]: %s\n", token->index, token->token_type);
	printf("-fd_in: %i\n", token->pipe->fd_in);
	printf("-fd_out: %i\n", token->pipe->fd_out);
}
void print_redir_token(t_token *token)
{
	printf("+Token[%i]: %s\n", token->index, token->token_type);
	printf("-type: %i\n", token->redir->type);
	printf("-in_name: %s\n", token->redir->in_name);
	printf("-out_name: %s\n", token->redir->out_name);
	printf("-fd_in: %i\n", token->redir->fd_in);
	printf("-fd_out: %i\n", token->redir->fd_out);
}

void print_token_list(t_token **token_list)
{
    t_token *p;

    p = *token_list;
	if (p == NULL)
	{
		printf("Token List is empty\n");
		return ;
	}
	printf("+++Token List+++\n\n");
    while (p != NULL)
    {
		if(token_type(p) == 0)
			print_cmd_token(p);
		else if(token_type(p) == 1)
			print_pipe_token(p);
		else if(token_type(p) == 2)
			print_redir_token(p);
		printf("\n");
        p = p->next;
    }
	printf("+++End Token List+++\n");
}
void free_cmd_token(t_token *token)
{
	//Si alocamos memoria para alguno de los campos este es el lugar para liberarla. Puede ser pruto de un split o un strjoin
    free(token->cmd);
    token->cmd = NULL; // Evitar acceso después de liberar
}

void free_pipe_token(t_token *token)
{
	//Si alocamos memoria para alguno de los campos este es el lugar para liberarla. Puede ser pruto de un split o un strjoin
    free(token->pipe);
    token->pipe = NULL; // Evitar acceso después de liberar
}

void free_redir_token(t_token *token)
{
	//Si alocamos memoria para alguno de los campos este es el lugar para liberarla. Puede ser pruto de un split o un strjoin
    free(token->redir);
    token->redir = NULL; // Evitar acceso después de liberar
}

void free_token_list(t_token **token_list)
{
    t_token *p;
    t_token *tmp;

    p = *token_list;
    printf("Debug1\n");
    while (p != NULL)
    {
        printf("Debug2\n");
        printf("Token type: %i\n", token_type(p));
        printf("Token index: %i\n", p->index);
        tmp = p;
        if (token_type(tmp) == 0)
            free_cmd_token(tmp);
        else if (token_type(tmp) == 1)
            free_pipe_token(tmp);
        else if (token_type(tmp) == 2)
            free_redir_token(tmp);
        p = p->next;
        free(tmp);
    }
    printf("Debug3\n");
    *token_list = NULL;
}

void listas (t_data *data)
{
	char** array_cmds = malloc(sizeof(char *) * 3);
	array_cmds[0] = "ls";
	array_cmds[1] = "-l";
	array_cmds[2] = NULL;
    token_list_add(&data->token_list, new_cmd_token(array_cmds, 0, 1, data));
    token_list_add(&data->token_list, new_redir_token(1, "in_file", "out_file", 0, 1, data));
    token_list_add(&data->token_list, new_pipe_token(0, 1, data));
    print_token_list(&data->token_list);
	printf("----MODIFICACION----\n");
	data->token_list->cmd->array_cmds[0] = "cat";
	print_token_list(&data->token_list);
	printf("----LIMPIEZA----\n");
	free_token_list(&data->token_list);
	print_token_list(&data->token_list);
}