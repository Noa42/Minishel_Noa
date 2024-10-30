#include "../include/minishell.h"

char *ft_var_name(char *name_value)
{
    int i = 0;
    while (name_value[i] != '=')
        i++;
    return (ft_substr(name_value, 0, i));
}
char *ft_var_value(char *name_value)
{
    int i = 0;
    while (name_value[i] != '=')
        i++;
    return (ft_substr(name_value, i + 1, ft_strlen(name_value) - i));
}

char *ft_getenv(char *var_name, char **env)//obtiene el valor de una variable de entorno
{
    int i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var_name, ft_strlen(var_name)+1) == 0) //el +1 es para que compare el = también y no encuentre otras variables que empiezan por el mismo nimbre
            return (ft_var_value(env[i]));
        i++;
    }
    return (NULL);
}

char **insert_var(char **array, char *var_name, char *var_value)
{
    int i;
    char **new_array;
    char *temp;

    i = 0;
    while(array[i])
    {
        if (ft_strncmp(array[i], var_name, ft_strlen(var_name)) == 0)
        {
            free(array[i]);
            temp = ft_strjoin(var_name, "=");
            array[i] = ft_strjoin(temp, var_value);
            free(temp);
            return (array);
        }
        i++;
    }
    new_array = realloc_elonged_array(array);
    temp = ft_strjoin(var_name, "=");
    new_array[i] = ft_strjoin(temp, var_value);
    new_array[i + 1] = NULL;
    free_array(array);
    free(temp);
    return (new_array);
}



