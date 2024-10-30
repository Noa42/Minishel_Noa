#include "../include/minishell.h"

int has_variable(char *input)
{
    int i = 0;
    while (input[i] != '\0')
    {
        if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
            return (1);
        i++;
    }
    return (0);
}
char *get_exp_var(char *s, int i)
{
    unsigned int start;
    char *var;

    start = i;
    while(s[i] != ' ' && s[i] != '\0')
        i++;
    var = ft_substr(s, start, i-start);
    return (var);
}
void print_expanded(char *input, char **env)
{
    int i;
    char *var;

    i = 0;
    while (input[i] != '\0')
    {
        if (input[i] == '$')
        {
            i++;
            var = get_exp_var(input, i);//aloca una subcadena desde despues de $ hasta el siquiente espacio o \0 por lo que devuelve el nombre de la variable a imprimir
            printf("%s", ft_getenv(var, env));
            free (var);
            return;
        }
        else
            printf("%c", input[i]);            
        i++;
    }
}

void ft_echo(t_data *data)
{
    //Esta función está hecha para que reciba un ** en el que [0]echo [1]palabra/-n [2]palabra ... [x]:NULL
    int n_flag;
    int i = 1;
    n_flag = 0;
    if (ft_strcmp(data->array_input[1], "-n") == 0)
    {
        n_flag = 1;
        i = 2;
    }
    while(data->array_input[i] != NULL)
    {
        if(ft_strcmp(data->array_input[i], "$?") == 0)
            printf("Exit status: %i", data->exit_status);
        else
        if(has_variable(data->array_input[i]) == 0) //si no tiene $
            printf("%s", data->array_input[i]);//imprimimos la palabra tal cual
        else
            print_expanded(data ->array_input[i], data->env);//imprme la palabra sustituyendo lo que hay después de $ por el valor de la variable 
        if (data->array_input[i + 1] != NULL) //pone los espacios entre las palabras a menos que sea el final
            printf(" ");
        i++;
    }
    if(n_flag == 0) //Sin la flag no se ha cambiado imprime salto de linea
        printf("\n");
}
