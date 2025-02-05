/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_mutate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:41:42 by xortega           #+#    #+#             */
/*   Updated: 2025/02/05 10:33:52 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	crop_top_bottom(t_data *data)
{
	int		new_tall;
	int		y;
	char	**new_map;

	new_tall = data->tall - 1;
	while (new_tall > 0 && data->map[new_tall - 1][0] == '\n')
		new_tall--;
	new_map = malloc(sizeof(char *) * (new_tall + 1));
	y = 0;
	while (y < new_tall)
	{
		new_map[y] = ft_strdup(data->map[y]);
		y++;
	}
	data->tall = y;
	new_map[y] = NULL;
	free_array((void **)data->map);
	data->map = new_map;
}

void	map_shaper(t_data *data)
{
	int	i;
	int	line;

	i = 0;
	line = data->info_end;
	data->tall = data->map_str_lines - line;
	data->map = malloc(sizeof(char *) * (data->tall + 1));
	while (data->map_str[line])
	{
		data->map[i] = ft_strdup(data->map_str[line]);
		i++;
		line++;
	}
	data->map[i] = NULL;
}

void	line_filler(t_data *data)
{
	int		i;
	size_t	k;
	char	*line;

	i = 0;
	while (data->map[i])
	{
		if (ft_strlen(data->map[i]) < (size_t)data->longest)
		{
			line = malloc(sizeof(char) * data->longest);
			line[0] = '\0';
			ft_strlcat(line, data->map[i], ft_strlen(data->map[i]) + 1);
			k = ft_strlen(line);
			while (k < (size_t)data->longest)
			{
				line[k] = '0';
				k++;
			}
			line[k] = '\0';
			free(data->map[i]);
			data->map[i] = line;
		}
		i++;
	}
}

void	spaces_replacer(t_data *data)
{
	int	i;
	int	k;

	i = 0;
	while (data->map[i])
	{
		k = 0;
		while (data->map[i][k])
		{
			if (data->map[i][k] == ' ')
				data->map[i][k] = '0';
			k++;
		}
		i++;
	}
}

void	map_mutate(t_data *data)
{
	if (data->info_end + 1 == data->map_str_lines)
		error_matic("Missing parts in the .cub\n", data, 3);
	while (data->map_str[data->info_end]
		&& data->map_str[data->info_end][0] == '\n')
		data->info_end++;
	map_shaper(data);
	longest_line(data);
	crop_top_bottom(data);
	spaces_replacer(data);
	line_filler(data);
	data->map_width = data->longest - 1;
	data->map_height = data->tall;
}
