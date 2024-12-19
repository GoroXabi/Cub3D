/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_mutate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:41:42 by xortega           #+#    #+#             */
/*   Updated: 2024/12/19 14:43:00 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	longest_line(t_data *data)
{
	int		l;
	size_t	v;

	l = 0;
	v = ft_strlen(data->map[l]);
	while (data->map[l])
	{
		if (ft_strlen(data->map[l]) > v)
			v = ft_strlen(data->map[l]);
		l++;
	}
	data->longest = v;
}

void	new_line_eraser(t_data *data)
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

void	erase_new_line(t_data *data)
{
	char	*line;
	int		k;

	k = 0;
	while (data->map[k])
	{
		line = ft_strtrim(data->map[k], "\n");
		free(data->map[k]);
		data->map[k] = line;
		k++;
	}
	data->map[k] = NULL;
}

void	map_shaper(t_data *data, int map_size, int line)
{
	int	i;

	i = 0;
	data->tall = map_size - line;
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

void	mutate_map(t_data *data, int map_size)
{
	int	line;

	line = get_texture_path(data);
	import_textures(data);
	if (line + 1 == map_size)
		error_matic("Missing parts in the .cub");
	while (data->map_str[line] && data->map_str[line][0] == '\n')
		line++;
	map_shaper(data, map_size, line);
	longest_line(data);
	new_line_eraser(data);
	erase_new_line(data);
	spaces_replacer(data);
	line_filler(data);
	data->map_width = data->longest - 1;
	data->map_height = data->tall;
}
