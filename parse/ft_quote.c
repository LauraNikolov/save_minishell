#include "../minishell.h"

int	ft_quote_len(char *s, int len)
{
	int		i;
	int		var_size;
	int		quote_flag;
	int		quote_len;
	char	c;

	quote_flag = -1;
	var_size = 0;
	i = -1;
	quote_len = 0;
	c = '\0';
	while (s[++i] && i < len)
	{
		if ((quote_flag == -1 && s[i] == '\"') || (quote_flag == -1
				&& s[i] == '\'') || (s[i] == c))
		{
			quote_flag *= -1;
			c = s[i];
			quote_len++;
		}
	}
	if (quote_len % 2 != 0)
		return (-1);
	return (i + var_size - quote_len);
}
