#include "../include/minishell.h"

void execution (t_data *data)
{
    printf("\nResultado de nuestros builtins\n");
    builtins(data);
    //Esto ejecuta un comando y por tanto acaba el proceso!!!!
    // char *path = get_path(data->array_input[0], data->env);
    // printf("\nResultado con execve:\n");
    // execve(path, data->array_input, data->env);
    //Solo funciona si solo le pasamos un comando. Si le pasamos un comando con argumentos no funciona.
}
//!!!!!!!!!!!!!!!!!!Hay que hacer que devulevan los mismos errores!!!!!!!!!!!!!!!!!!!
void builtins(t_data *data)
{
    if(ft_strncmp(data->array_input[0], "echo", ft_strlen(data->array_input[0])) == 0)
        ft_echo(data->array_input);
    else if(ft_strncmp(data->array_input[0], "pwd", ft_strlen(data->array_input[0])) == 0)
        ft_pwd();
    else if (ft_strncmp(data->array_input[0], "cd", ft_strlen(data->array_input[0])) == 0)
        ft_cd(data);
    else if (ft_strncmp(data->array_input[0], "env", ft_strlen(data->array_input[0])) == 0)
        ft_env(data);
    else if (ft_strncmp(data->array_input[0], "export", ft_strlen(data->array_input[0])) == 0)
        ft_export(data);
    else if (ft_strncmp(data->array_input[0], "x", ft_strlen(data->array_input[0])) == 0)
        chdir("..");//esto si lo hace bien

    
}
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
void print_expanded(char *input)
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
            printf("%s", getenv(var)); //imprime el valor de la variable usando el nombre de la variable
            free (var);
            return;
        }
        else
            printf("%c", input[i]);            
        i++;
    }
}

void ft_echo(char **array_input)
{
    //Esta función está hecha para que reciba un ** en el que [0]echo [1]palabra/-n [2]palabra ... [x]:NULL
    int n_flag;
    int i = 1;
    n_flag = 0; 
    if (ft_strncmp(array_input[1], "-n", 3) == 0)
    {
        n_flag = 1;
        i = 2;
    }
    if(has_variable(array_input[i]) == 0) //si no tiene $
        printf("%s", array_input[i]);//imprimimos la palabra tal cual
    else
    {
        print_expanded(array_input[i]);//imprme la palabra sustituyendo lo que hay después de $ por el valor de la variable 
    }
    if (array_input[i + 1] != NULL) //pone los espacios entre las palabras a menos que sea el final
        printf(" ");
    i++;
    if(n_flag == 0) //Sin la flag no se ha cambiado imprime salto de linea
        printf("\n");
}
void ft_pwd(void)
{
    char *dir;

    dir = malloc(sizeof(char)* 1024);
    getcwd(dir, 1024);
    printf("%s\n", dir);
}
void ft_cd(t_data *data)
{
    char *oldpwd;
    
    oldpwd = malloc(sizeof(char)* 1024);
    getcwd(oldpwd, 1024);
    if (chdir(data->array_input[1]) != 0)
    {
        free(oldpwd);
        printf("Error cambiando de dirección\n");
        return;
    }
    else
    {
        insert_var(data->env, "OLDPWD", oldpwd);
        insert_var(data->array_var, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    printf("El directorio destino es %s\n", data->array_input[1]);
    chdir(data->array_input[1]);
}
void ft_env(t_data *data)
{
    print_array(data->env);
}

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

void insert_var(char **array, char *var_name, char *var_value)
{
    int i;

    i = 0;
    while(array[i])
    {
        if (ft_strncmp(array[i], var_name, ft_strlen(var_name)) == 0)//Si la variable ya existe la cambia
        {
            free(array[i]);
            array[i] = ft_strjoin(var_name, "=");
            array[i] = ft_strjoin(array[i], var_value);
            return;
        }
        i++;
    }
    array[i] = ft_strjoin(var_name, "=");//Si la variable no existe aloca una nueva linea en el array con la variable
    array[i] = ft_strjoin(array[i], var_value);
    array[i + 1] = NULL;
}

void print_export(char **array_var)
{
    int i = 0;
    while (array_var[i])//hay que implementar orden alfabético
    {
        printf("declare -x %s\n", array_var[i]);
        i++;
    }
}

void ft_export(t_data *data)
{
    if(data->array_input[1] == NULL)
    {
        print_export(data->array_var);
        return;
    }
    char *var_name;
    char *var_value;
    var_name = ft_var_name(data->array_input[1]);
    var_value = ft_var_value(data->array_input[1]);
    //if comprobaciones porque hay variables que se incluyen en export y no en env por ejemplo si está vacía $export a="", esto aparece en export pero no en env
    insert_var(data->env, var_name, var_value);
    insert_var(data->array_var, var_name, var_value);
    free(var_name);
    free(var_value);
}