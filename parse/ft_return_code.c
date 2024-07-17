#include "../minishell.h"

int ft_return_code(char *code, t_envp **env)
{
    if (!env)
        return (0);
    t_envp *curr;

    curr = *env;
    while(curr)
    {
        if (!ft_strcmp(curr->var_name, "?"))
        { 
            ft_safe_free(&curr->var_value);
            curr->var_value = ft_strdup(code);
            break ;
        }
        curr = curr->next;
    }
    free(code);
    return (ft_atoi(curr->var_value));
}
