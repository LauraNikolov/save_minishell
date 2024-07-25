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
            if(!curr->var_value)
                return(-1);
            break ;
        }
        curr = curr->next;
    }
    if(curr->var_value)
        return (ft_atoi(curr->var_value));
    return(0);
}
