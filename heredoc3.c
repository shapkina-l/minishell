/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lshapkin <lshapkin@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 19:18:48 by lshapkin          #+#    #+#             */
/*   Updated: 2025/05/07 23:26:45 by lshapkin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_heredoc_tempfile(void)
{
	static int	count;
	char		*num;
	size_t		len;
	char		*filename;

	num = ft_itoa(count++);
	len = ft_strlen("/tmp/minishell_heredoc_") + ft_strlen(num) + 1;
	filename = malloc(len);
	if (!filename)
	{
		free(num);
		return (NULL);
	}
	ft_strlcpy(filename, "/tmp/minishell_heredoc_", len);
	ft_strlcat(filename, num, ft_strlen(num));
	free(num);
	return (filename);
}

static int	expand_exit_status(t_expand_utils *ctx)
{
	char	*num;

	if (ctx->line[ctx->idx] == '$' && ctx->line[ctx->idx + 1] == '?')
	{
		num = ft_itoa(ctx->exit_status);
		ft_strlcpy(ctx->out + ctx->pos, num, ft_strlen(num) + 1);
		ctx->pos += ft_strlen(num);
		free(num);
		ctx->idx += 2;
		return (1);
	}
	return (0);
}

static int	expand_env_var_ctx(t_expand_utils *ctx, char **my_envp)
{
	int		start;
	int		len;
	char	name[256];
	char	*val;

	if (ctx->line[ctx->idx] != '$')
		return (0);
	start = ++ctx->idx;
	while (isalnum(ctx->line[ctx->idx]) || ctx->line[ctx->idx] == '_')
		ctx->idx++;
	len = ctx->idx - start;
	if (len <= 0)
		return (0);
	ft_memcpy(name, ctx->line + start, len);
	name[len] = '\0';
	val = get_env_value(name, my_envp);
	if (val)
	{
		ft_strlcpy(ctx->out + ctx->pos, val, ft_strlen(val) + 1);
		ctx->pos += strlen(val);
	}
	return (1);
}

static void	copy_next_char(t_expand_utils *ctx)
{
	ctx->out[ctx->pos++] = ctx->line[ctx->idx++];
}

char	*expand_vars_in_line(const char *line, int exit_status, char ** my_envp)
{
	t_expand_utils	ctx;

	ctx.line = line;
	ctx.out = malloc(BUF_SIZE);
	ctx.idx = 0;
	ctx.pos = 0;
	ctx.exit_status = exit_status;
	if (!ctx.out)
		return (NULL);
	while (line[ctx.idx])
	{
		if (expand_exit_status(&ctx))
			continue ;
		if (expand_env_var_ctx(&ctx, my_envp))
			continue ;
		copy_next_char(&ctx);
	}
	ctx.out[ctx.pos] = '\0';
	return (ctx.out);
}
