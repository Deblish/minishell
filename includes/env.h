#ifndef ENV_H
# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	int				is_env;
	struct s_env	*next;
}			t_env;

t_env	*fetch_envp(char **envp);
char	*get_env(char *key);
void	envadd(t_env **lst, t_env *new);
int		add_to_env_list(char *key, char *value, int is_env);
int		is_key_in_env(char *key);

#endif