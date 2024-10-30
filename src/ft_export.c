#include "../include/minishell.h"

void print_export(char **array_var)
{
    int i;

    i = 0;
    while (array_var[i])
    {
        printf("declare -x %s\n", array_var[i]);
        i++;
    }
}

void ft_export(t_data *data)
{
    char *var_name;
    char *var_value;
    int i;

    if(data->array_input[1] == NULL)
    {
        print_export(data->array_var);
        return;
    }
    i = 1;
    while (data->array_input[i])
    {
        var_name = ft_var_name(data->array_input[i]);
        var_value = ft_var_value(data->array_input[i]);
        //if comprobaciones porque hay variables que se incluyen en export y no en env por ejemplo si está vacía $export a="", esto aparece en export pero no en env
        data->env = insert_var(data->env, var_name, var_value);
        data->array_var = insert_var(data->array_var, var_name, var_value);
        i++;
    }
    free(var_name);
    free(var_value);
}