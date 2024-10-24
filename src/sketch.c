#include "../include/minishell.h"

int	only_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	history(char *input)
{
	if (input != NULL && ft_strlen(input) != 0 && only_spaces(input) == 0)
	{
		add_history(input);
	}
}

void	init_data(t_data *data, char **env)
{
	data->env = copy_alloc_array(env);
	//print_array(data->env);
	data->array_var = copy_alloc_array(env);
	data->input = NULL;
	data->array_input = NULL;
}
void    free_data(t_data *data)
{
    free(data->input);
}

int	main(int argc, char** argv, char **env)
{
	t_data	data;
	argc ++; //esto es para que no me salte un warning de que argc y argv no se usa y el primer argumento siempre tiene que ser de tipo int. Es para coger el env.
	argv[1] = "avoid warning";
	init_data(&data, env);
	signals();
	while (1)
	{
		data.input = readline("MiniShell> ");
		if (data.input == NULL || ft_strncmp(data.input, "exit", ft_strlen(data.input)) == 0)
			break ;
		history(data.input);
		printf("Leído: %s\n", data.input);
		parsing(&data);
		execution(&data);
	}
    free_data(&data);
	printf("Saliendo de MiniShell\n");
	return (0);
}
