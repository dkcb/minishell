/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tbouma <tbouma@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/07/13 15:52:01 by tiemen        #+#    #+#                 */
/*   Updated: 2022/09/14 13:39:00 by dkocob        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	**g_pipe_heredoc;

/*
oxs = OLD_EXIT_STATUS
*/
int	basic_error_handeling(struct s_main *m_s)
{
	if (m_s->all_tokens[0] == NULL)
		return (1);
	if (ft_strncmp(m_s->all_tokens[0], "|", 2) == 0)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		m_s->oxs = 258;
		return (1);
	}
	return (0);
}

int	init_m_s(struct	s_main *m_s)
{
	m_s->all_tokens = NULL;
	m_s->cmd_count = 0;
	m_s->cmd_lines = NULL;
	m_s->c_s_arr = NULL;
	m_s->input_str = NULL;
	m_s->root_paths = NULL;
	m_s->err = 0;
	return (0);
}

int	check_error(struct s_main *m_s)
{
	int	i;

	i = 0;
	while (i < m_s->cmd_count)
	{
		if (m_s->c_s_arr[i].err_syntax > 0 || m_s->c_s_arr[i].set_file_err > 0)
		{
			m_s->err = 1;
			m_s->oxs = 258;
			free_struct(m_s);
			return (1);
		}
		i++;
	}
	return (0);
}

//rl_catch_signals = 0;

void	minishell_loop(struct s_main *m_s, int argc, char **argv)
{
	while (1)
	{
		init_m_s(m_s);
		m_s->root_paths = find_path(&m_s->env_llist);
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		{
			if (ft_strncmp(argv[2], "|", 1) == 0)
				exit (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2));
			m_s->input_str = ft_strdup(argv[2]);
		}
		else
			m_s->input_str = readline("minishell-1.0$ ");
		add_history(m_s->input_str);
		if (m_s->input_str == NULL)
		{
			write(2, "exit\n", 5);
			free_struct(m_s);
			break ;
		}
		expand_variables(&m_s->input_str, &m_s->env_llist, m_s->oxs, 0);
		//printf("&m_s->input_str = %s\n", m_s->input_str);
		m_s->all_tokens = split_tokens(m_s->input_str);
		//print_dubble_str(m_s->all_tokens, "all tokens: ");
		if (basic_error_handeling(m_s))
		{
			free_struct(m_s);
			continue ;
		}
		m_s->cmd_lines = make_cmd_lines(m_s->all_tokens);
		if (make_cmd_structs(m_s) == -1)
		{
			free_struct(m_s);
			continue ;
		}
		//print_structs(m_s);
		if (check_error(m_s))
			continue ;
		m_s->oxs = exec(m_s);
		if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
			exit(m_s->oxs);
		free_struct(m_s);
	}
}

int	main(int argc, char **argv)
{
	extern char		**environ;
	struct s_main	m_s;

	g_pipe_heredoc = NULL;
	m_s.oxs = 0;
	m_s.minishell_nr = 0;
	m_s.env_llist = add_env_to_list(&m_s, environ);
	signals_handeler();
	minishell_loop(&m_s, argc, argv);
	ft_free_linked_list(&m_s.env_llist);
	return (m_s.oxs);
}

	//print_linked_list(&m_s.env_llist);
		// print_dubble_str(m_s.root_paths, "PATHS\n");
		// ft_find_and_remove_node(&m_s.env_llist, "PATH");