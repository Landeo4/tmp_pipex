/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child_process.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpotilli <tpotilli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 15:58:51 by tpotilli          #+#    #+#             */
/*   Updated: 2024/01/21 19:10:58 by tpotilli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	check_dup(int pipe, int token, int pipe2)
{
	if (token == 0)
	{
		if (dup2(STDIN_FILENO, STDIN_FILENO) < 0)
			return (printf("problem with dup2 1"), -1);
		if (dup2(pipe2, 1) < 0)
			return (printf("problem with dup2 2"), -1);
	}
	else if (token == 1)
	{
		// if (dup2(pipe, 0) < 0)
			// return (printf("problem with dup2 3"), -1);
		if (dup2(STDOUT_FILENO, 1) < 0)
			return (printf("problem with dup2 4"), -1);
			// return (free(pipesfd[0]), free(pipesfd[1]), free(pipesfd), printf("problem with dup2"), -1);
	}
	else if (token == 2)
	{
		if (dup2(pipe, 0) < 0)
			return (printf("problem with dup2 5"), -1);
		if (dup2(pipe2, 1) < 0)
			return (printf("problem with dup2 6"), -1);
	}
	return (0);
}

char	*child_process_in(int **pipefd, char **argv, char **env, int i, int argc, int token)
{
	char		*cmd;
	char		**buf;
	int			verif;

	fprintf(stderr, "DEBUT DE PROCESS IN \nVOICI MA COMMANDE %s i = %d\n", argv[i], i);
	if (i == 0)
	{
		fprintf(stderr, "je passe par entre\n");
		verif = check_dup(0, 0, pipefd[0][1]);
		close(pipefd[0][1]);
		// close(pipefd[0][0]);
		fprintf(stderr, "verif = %d\n", verif);
		if (verif == -1)
			return (free_pipe_argv(pipefd, argv), NULL);
	}
	else if (i == argc - 1)
	{
		// close(pipefd[1][1]);
		// close(pipefd[0][1]);
		if (token == 0)
			verif = check_dup(pipefd[1][0], 1, 0);
		else
			verif = check_dup(pipefd[0][0], 1, 0);
		fprintf(stderr, "token = %d\n", token);
		close(pipefd[1][0]);
		close(pipefd[0][0]);
		fprintf(stderr, "mon verif dans out %d et token %d et i %d\n", verif, token, i);
		if (verif == -1)
			return (free_pipe_argv(pipefd, argv), NULL);
	}
	else if (token == 0)
	{
		// close(pipefd[1][1]);
		// close(pipefd[0][0]);
		verif = check_dup(pipefd[1][0], 2, pipefd[0][1]);
		close(pipefd[1][0]);
		close(pipefd[0][1]);
		if (verif == -1)
			return (free_pipe_argv(pipefd, argv), NULL);
	}
	else if (token == 1)
	{
		fprintf(stderr, "donc je apsse par la??\n");
		// close(pipefd[0][1]);
		// close(pipefd[1][0]);
		verif = check_dup(pipefd[0][0], 2, pipefd[1][1]);
		close(pipefd[0][0]);
		close(pipefd[1][1]);
		if (verif == -1)
			return (free_pipe_argv(pipefd, argv), NULL);
	}
	buf = arg(argv[i]);
	fprintf(stderr, "buf de 0 = %s i = %d\n", buf[0], i);
	cmd = ft_do_process(env, buf[0], pipefd, i);
	if (cmd == NULL)
		return (free_pipe_argv(pipefd, argv), NULL);
	free(pipefd[0]);
	free(pipefd[1]);
	return (cmd);
}

int	child_process_middle(int **pipefd, char **argv, char **env, int i)
{
	char		**cmd_argument;
	char		*cmd;
	char		**buf;

	fprintf(stderr, "mid %s i = %d\n", argv[i], i);
	if (i % 2 == 1)
	{
		fprintf(stderr, "je passe par 1 au mid\n");
		close(pipefd[0][1]);
		close(pipefd[1][0]);
		if (dup2(pipefd[0][0], STDIN_FILENO) < 0)
			return (printf("problem with dup2 mid 1"), -1);
		close(pipefd[0][0]);
		if (dup2(pipefd[1][1], STDOUT_FILENO) < 0)
			return (printf("problem with dup2 mid 2"), -1);
		close(pipefd[1][1]);
	}
	else
	{
		// close(pipefd[0][0]);
		// close(pipefd[1][1]);
		if (dup2(pipefd[1][0], STDIN_FILENO) < 0)
			return (printf("problem with dup2 mid 3"), -1);
		close(pipefd[1][0]);
		if (dup2(pipefd[0][1], STDOUT_FILENO) < 0)
			return (printf("problem with dup2 mid 4"), -1);
		close(pipefd[0][1]);
	}
	buf = arg(argv[i]);
	cmd = ft_do_process(env, buf[0], pipefd, i);
	cmd_argument = ft_split(argv[i], ' ');
	if (cmd == NULL)
	{
		free_pipe_argv(pipefd, argv);
		exit(0);
	}
	else if (!cmd_argument)
		return (perror("problem with cmd_arg\n"), free(cmd_argument), free(cmd), -1);
	fprintf(stderr, "MID AVANT EXEC\n");
	execve(cmd, cmd_argument, env);
	fprintf(stderr, "EXEC DANS MID RATE\n");
	free(cmd);
	free(cmd_argument);
	free(pipefd[0]);
	free(pipefd[1]);
	return (0);
}

int	child_process_out(int **pipesfd, char **argv, char **env, int i)
{
	char		**cmd_argument;
	char		*cmd;
	char		**buf;

	fprintf(stderr, "je suis dans out %s %d\n", argv[i], i);
	if (i % 2 == 1)
	{
		fprintf(stderr, "JE SUIS DANS I %% 2 == 1\n");
		if (dup2(pipesfd[1][0], 0) < 0)
			return (free(pipesfd), printf("problem with dup2"), -1);
		close(pipesfd[1][1]);
		close(pipesfd[0][1]);
		close(pipesfd[0][0]);
		close(pipesfd[1][0]);
		if (dup2(STDOUT_FILENO, 1) < 0)
			return (free(pipesfd), printf("problem with dup2"), -1);
	}
	else
	{
		fprintf(stderr, "JE SUIS DANS I %% 2 == 0\n");
		if (dup2(pipesfd[0][0], 0) < 0)
			return (free(pipesfd), printf("problem with dup2"), -1);
		close(pipesfd[0][1]);
		close(pipesfd[1][0]);
		close(pipesfd[1][1]);
		close(pipesfd[0][0]);
		if (dup2(STDOUT_FILENO, 1) < 0)
			return (free(pipesfd[0]), free(pipesfd[1]), free(pipesfd), printf("problem with dup2"), -1);
	}
	buf = arg(argv[i]);
	cmd = ft_do_process(env, buf[0], pipesfd, i);
	cmd_argument = ft_split(argv[i], ' ');
	free(buf);
	// fprintf(stderr, "buf de 0 = %s\n", buf[0]);
	if (!cmd)
		return (perror("problem with cmd\n"), free(cmd_argument), free(cmd), -1);
	else if (!cmd_argument)
		return (perror("problem with cmd_arg\n"), free(cmd_argument), free(cmd), -1);
	fprintf(stderr, "execve dans out commence, le cmd = %s\n", cmd);
	execve(cmd, cmd_argument, env);
	free(cmd);
	free(cmd_argument);
	free(pipesfd[0]);
	free(pipesfd[1]);
	perror("EXECVE A ECHOUER DANS OUT");
	// exit(0);
	return (0);
}

char	**arg(char *str)
{
	char	**buf;

	buf = ft_split(str, ' ');
	return (buf);
}

void	free_pipe_argv(int **pipefd, char *argv[])
{
	close(pipefd[0][0]);
	close(pipefd[0][1]);
	close(pipefd[1][1]);
	close(pipefd[1][0]);
	free(pipefd[1]);
	free(pipefd[0]);
	free(pipefd);
	ft_freedb_essaie(argv);
}

int	child_process_single(int **pipesfd, char **argv, char **env, int i)
{
	(void)pipesfd;
	char		**cmd_argument;
	char		*cmd;

	// cmd_argument = NULL;
	// (void)i;
	// (void)argv;
	// (void)env;
	// fprintf(stderr, "je suis dans out %s %d\n", argv[i], i);
	// if (dup2(STDIN_FILENO, STDIN_FILENO) < 0)
	// 	return (printf("problem with dup2"), -1);
	// if (dup2(STDOUT_FILENO, 1) < 0)
	// 	return (printf("problem with dup2"), -1);
	// close(pipesfd[0][1]);
	// close(pipesfd[0][0]);
	// close(pipesfd[1][0]);
	// close(pipesfd[1][1]);
	// fprintf(stderr, "Avant execve : cmd_tout_cours = %s\n", cmd);
	cmd = ft_do_process(env, argv[1], pipesfd, i);
	cmd_argument = ft_split(argv[1], ' ');
	execve(cmd, cmd_argument, env);
	return (0);
}

/*
int	child_process_in(int **pipesfd, char **argv, char **env, int i)
{
	fprintf(stderr, "in %s %d\n", argv[i], i);
	if (dup2(STDIN_FILENO, STDIN_FILENO) < 0)
		return (printf("problem with dup2"), -1);
	// close(pipesfd[i].pipes[0])
	close(pipesfd[0][1]);
	if (dup2(pipesfd[1][1], STDOUT_FILENO) < 0)
		return (printf("problem with dup2"), -1);
	close(pipesfd[1][1]);
	if (ft_do_process(env, argv[i]) == -1)
		return (free(pipesfd[0]), free(pipesfd[1]), free(pipesfd), -1);
	return (0);
}

int	child_process_middle(int **pipesfd, char **argv, char **env, int i)
{
	fprintf(stderr, "mid %s %d\n", argv[i], i);
	if (i % 2 == 1)
	{
		close(pipesfd[0][1]);
		if (dup2(pipesfd[0][0], STDIN_FILENO) < 0)
			return (printf("problem with dup2"), -1);
		close(pipesfd[0][0]);
		// close(pipesfd[1].pipes[0])
		if (dup2(pipesfd[1][1], STDOUT_FILENO) < 0)
			return (printf("problem with dup2"), -1);
		close(pipesfd[1][1]);
		close(pipesfd[1][0]);
	}
	else
	{
		close(pipesfd[1][1]);
		if (dup2(pipesfd[1][0], STDIN_FILENO) < 0)
			return (printf("problem with dup2"), -1);
		close(pipesfd[1][0]);
		// close(pipesfd[0].pipes[0])
		if (dup2(pipesfd[0][1], STDOUT_FILENO) < 0)
			return (printf("problem with dup2"), -1);
		close(pipesfd[0][1]);
		close(pipesfd[0][0]);
	}
	ft_do_process(env, argv[i]);
	return (0);
}

int	child_process_out(int **pipesfd, char **argv, char **env, int i)
{
	fprintf(stderr, "je suis dans out %s %d\n", argv[i], i);
	if (i % 2 == 1)
	{
		close(pipesfd[1][1]);
		close(pipesfd[0][0]);
		close(pipesfd[0][1]);
		if (dup2(pipesfd[1][0], 0) < 0)
			return (free(pipesfd), printf("problem with dup2"), -1);
		close(pipesfd[1][0]);
		if (dup2(STDOUT_FILENO, 1) < 0)
			return (free(pipesfd), printf("problem with dup2"), -1);
	}
	else
	{
		close(pipesfd[1][1]);
		close(pipesfd[1][0]);
		close(pipesfd[0][1]);
		if (dup2(pipesfd[0][0], 0) < 0)
			return (free(pipesfd), printf("problem with dup2"), -1);
		close(pipesfd[0][0]);
		if (dup2(STDOUT_FILENO, 1) < 0)
			return (free(pipesfd[0]), free(pipesfd[1]), free(pipesfd), printf("problem with dup2"), -1);
	}
	ft_do_process(env, argv[i]);
	return (0);
}

int	child_process_single(int **pipesfd, char **argv, char **env, int i)
{
	// fprintf(stderr, "je suis dans out %s %d\n", argv[i], i);
	if (dup2(STDIN_FILENO, STDIN_FILENO) < 0)
		return (printf("problem with dup2"), -1);
	if (dup2(STDOUT_FILENO, 1) < 0)
		return (free(pipesfd[0]), free(pipesfd[1]), free(pipesfd), printf("problem with dup2"), -1);
	ft_do_process(env, argv[i]);
	return (0);
}

*/