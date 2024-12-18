#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>

# include "libft.h"
# include "gc_alloc.h"
# include "lexer.h"
# include "signals.h"
//# include "parsing.h"
# include "builtins.h"
# include "env.h"
//# include "execution.h"

# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_info
{
	bool 	debug;
}			t_info;

t_info	*get_info(void);
void	set_debug(bool debug);
bool	get_debug(void);

extern t_env *g_env;
extern int	g_exit_status;

#endif
