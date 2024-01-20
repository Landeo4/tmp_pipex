/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 18:19:16 by tpotilli          #+#    #+#             */
/*   Updated: 2024/01/20 12:19:08 by tpotilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_pipes *init_struct(char *argv[], int new_argc, char *env[])
{
	t_pipes *pipes;
	int		i;

	i = 0;
	pipes = malloc(sizeof(t_pipes) * 2);
	pipes->argv = malloc(sizeof(char *) * (found_max(argv) + 1));
	pipes->env = malloc(sizeof(char *) * (found_max(env) + 1));
	while (env[i] || argv[i])
	{
		if (env[i])
		{
			pipes->env[i] = malloc(sizeof(char *) * (len_fd_tab(env, i) + 1));
			pipes->env[i] = ft_strlcpy(pipes->env[i], env[i], (ft_strlen(env[i]) + 1));
		}
		if (argv[i])
		{
			pipes->argv[i] = malloc(sizeof(char *) * (len_fd_tab(argv, i) + 1));
			pipes->argv[i] = ft_strlcpy(pipes->argv[i], argv[i], (ft_strlen(argv[i]) + 1));
		}
		i++;
	}
	// pipes->argv[i] = '\0';
	pipes->argc = new_argc;
	return (pipes);
}

//revoir systheme pou rcompter le nombre de pipe
// faire ne sorte que ca s'adapte pour nb de pipes, de fd, pour le moment simple

