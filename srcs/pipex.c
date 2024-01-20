/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 13:32:11 by tpotilli          #+#    #+#             */
/*   Updated: 2024/01/20 15:03:18 by tpotilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
**	This function takes as parameter: 
**
**	Char *argv
**	char *envp
**  int fd1
**  int fd4
**
** =====================================================
** =====================================================
**
** pid_t to stock the identifiers of process
** pipefd[2] tab to stock the identifiers of pid(reading or writing)
** status to verify the status at child
**
** pid1 = fork () call fork to creat a copy of the parent
** if fork work
** -> parent get the child identifiers in pid1
** if doesn't work -> return (1) 
**
** if pid is negative return 1, if pid1 == 0 child process
**
** waitpid() wait the end of parent to wait child
*/
// faire plusieurs pipe de maniere intelligente

int	ft_pipex(char *argv[], char *env[], int argc)
{
	pid_t		pid[argc];
	int			**pipefd;
	int			i;
	char		**cmd_argument;
	char		*cmd;
	int			c;
	
	i = ((c = 0));
	pipefd = NULL;
	pipefd = alloc_pipe(i, pipefd);
	if (!pipefd[1] || !pipefd[0])
		return (free(pipefd), -1);
	while (i < argc)
	{
		pid[i] = fork();
		if (pid[i] < 0)
			return (printf("erreur de fork\n"), 1);
		if (pid[i] == 0)
		{
			if (i % 2 == 0)
				cmd = child_process_in(pipefd, argv, env, i, argc, 0);
			else
				cmd = child_process_in(pipefd, argv, env, i, argc, 1);
			if (cmd == NULL)
			{
				free_pipe_argv(pipefd, argv);
				exit(0);
			}
			else
			{
				cmd_argument = ft_split(argv[i], ' ');
				while (cmd_argument[c++])
					fprintf(stderr, "pour execve cmd_argument = %s i = %d\n", cmd_argument[c], i);
				execve(cmd, cmd_argument, env);
			}
		}
			// return (free(pipefd[0]), free(pipefd[1]), free(pipefd), -1);
		else
			pipefd = parent_process(pipefd, i);
		i++;
	}
	i = 0;
	while (i < argc)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	fprintf(stderr, "=============HELOOOOO TOUT A REUSSIS NORMALEMENT=============\n");
	free(pipefd[0]);
	free(pipefd[1]);
	free(pipefd);
	return (0);
}

int	**alloc_pipe(int i, int **pipefd)
{
	if (i == 0)
	{
		pipefd = malloc(sizeof(int *) * 2);
		if (!pipefd)
			return (fprintf(stderr, "probleme happend in alloc_pipe"), NULL);
		pipefd[0] = malloc(sizeof(int) * 2);
		pipefd[1] = malloc(sizeof(int) * 2);
		if (!pipefd[0] || !pipefd[1])
		{
			free(pipefd[0]);
			free(pipefd[1]);
			return (pipefd);
		}
		pipe(pipefd[0]);
		pipe(pipefd[1]);
	}
	return (pipefd);
}

int	**parent_process(int **pipefd, int i)
{
	// fprintf(stderr, "je suis dans le parent et voici mon i %d\n", i);
	if (i % 2 == 0)
	{
		if (!pipefd[0] || !pipefd[1])
			return (free(pipefd[0]), free(pipefd[1]), free(pipefd), NULL);
		close(pipefd[0][0]);
		close(pipefd[0][1]);
		pipe(pipefd[0]);
	}
	else
	{
		if (!pipefd[0] || !pipefd[1])
			return (free(pipefd[0]), free(pipefd[1]), free(pipefd), NULL);
		close(pipefd[1][1]);
		close(pipefd[1][0]);
		pipe(pipefd[1]);
	}
	// if (!pipefd[1] || !pipefd[0])
	// 	return (free(pipefd), NULL);
	return (pipefd);
}

// faire 2 pipe puis dans la boucle tu fork et si dans parent et j'en recree dans le parent
// faire attention au modulo

/*
if (i % 2 == 0)
	{
		close(pipefd[0][0]);
		close(pipefd[0][1]);
		if (!pipefd[0] || !pipefd[1])
		{
			free(pipefd[0]);
			free(pipefd[1]);
			return (pipefd);
		}
		pipe(pipefd[0]);
	}
	else
	{
		close(pipefd[1][0]);
		close(pipefd[1][1]);
		if (!pipefd[0] || !pipefd[1])
		{
			free(pipefd[0]);
			free(pipefd[1]);
			return (pipefd);
		}
		pipe(pipefd[0]);
	}
*/

	// if (pid > 0)
		// parent_process(pipefd);
	// int	j = 0;
	// int status = 0;
	// close(pipefd[0][0]);
	// close(pipefd[0][1]);
	// close(pipefd[1][0]);
	// close(pipefd[1][1]);
	// while (j < argc)
	// {
	// 	waitpid(pid[j], &status, 0);
	// 	j++;
	// }