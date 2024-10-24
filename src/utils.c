#include "../include/minishell.h"

void print_array(char **array)
{
    int i = 0;
    while (array[i])
    {
        printf("%s\n", array[i]);
        i++;
    }
}
void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
char **copy_alloc_array(char **array)
{
	int i = 0;
	char **new_array;
	while (array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}
int array_len(char **array)
{
	int i = 0;
	while (array[i])
		i++;
	return (i);
}