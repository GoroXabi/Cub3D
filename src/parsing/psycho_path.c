/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psycho_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:50 by andefern          #+#    #+#             */
/*   Updated: 2024/12/18 19:52:37 by xortega          ###   ########.fr       */
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

void	flud_fil(t_data *data, int y, int x)
{
	if (data->ffmap[y][x] == '1')
		return ;
	if (x == 0 || y == 0
		|| y == data->tall - 1 || x == data->longest - 1)
	{
		printf("%d , %d\n", x, y);
		error_matic("Invalid map\n");
	}
	data->ffmap[y][x] = '1';
	flud_fil(data, y - 1, x);
	flud_fil(data, y + 1, x);
	flud_fil(data, y, x + 1);
	flud_fil(data, y, x - 1);
}

void	make_floor(t_data *data)
{
	int	r;
	int	g;
	int	b;
	int	flag;

	r = 0;
	g = 0;
	b = 0;
	r = ft_atoi(data->floor_tex_path);
	flag = 0;
	while (data->floor_tex_path[flag] != ',')
	{
		if (data->floor_tex_path[flag + 1] == ',')
			g = ft_atoi(data->floor_tex_path + flag + 2);
		flag++;
	}
	flag++;
	while (data->floor_tex_path[flag] != ',')
	{
		if (data->floor_tex_path[flag + 1] == ',')
			b = ft_atoi(data->floor_tex_path + flag + 2);
		flag++;
	}
	data->floor_color = make_pixel(r, g, b, 255);
}

void	make_sky(t_data *data)
{
	int	r;
	int	g;
	int	b;
	int	flag;

	r = 0;
	g = 0;
	b = 0;
	r = ft_atoi(data->sky_tex_path);
	flag = 0;
	while (data->sky_tex_path[flag] != ',')
	{
		if (data->sky_tex_path[flag + 1] == ',')
			g = ft_atoi(data->sky_tex_path + flag + 2);
		flag++;
	}
	flag++;
	while (data->sky_tex_path[flag] != ',')
	{
		if (data->sky_tex_path[flag + 1] == ',')
			b = ft_atoi(data->sky_tex_path + flag + 2);
		flag++;
	}
	data->sky_color = make_pixel(r, g, b, 255);
}

void	import_textures(t_data *data)
{
	data->north_tex = mlx_load_png(data->north_tex_path);
	data->south_tex = mlx_load_png(data->south_tex_path);
	data->east_tex = mlx_load_png(data->east_tex_path);
	data->west_tex = mlx_load_png(data->west_tex_path);
	if (!data->north_tex
		|| !data->south_tex
		|| !data->east_tex
		|| !data->west_tex)
		error_matic("texture error");
	data->n_tex = make_texture(data->north_tex);
	data->s_tex = make_texture(data->south_tex);
	data->e_tex = make_texture(data->east_tex);
	data->w_tex = make_texture(data->west_tex);
	make_floor(data);
	make_sky(data);
}

int	get_texture_path(t_data *data)
{
	int		line;
	char	*trimed;

	line = -1;
	while (data->map_str[++line]
		&& !(data->north_tex_path && data->south_tex_path
			&& data->east_tex_path && data->west_tex_path
			&& data->floor_tex_path && data->sky_tex_path))
	{
		trimed = ft_strtrim(data->map_str[line], "\t ");
		if (trimed[0] == 'N' && trimed[1] == 'O')
			data->north_tex_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'S' && trimed[1] == 'O')
			data->south_tex_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'E' && trimed[1] == 'A')
			data->east_tex_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'W' && trimed[1] == 'E')
			data->west_tex_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'F')
			data->floor_tex_path = ft_strtrim(trimed + 1, " \t\n");
		else if (trimed[0] == 'C')
			data->sky_tex_path = ft_strtrim(trimed + 1, " \t\n");
		free(trimed);
	}
	return (line + 1);
}
/* void print_map(t_data *data, char *color)
{
	printf("%s", color);
	for (int i = 0; data->map[i]; i++)
		printf("%s\n", data->map[i]);
	printf("---------------------\n"RST);
} */

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
	valid_character(data);
	data->map_width = data->longest - 1;
	data->map_height = data->tall;
}

void	map_checks(t_data *data, int map_size)
{
	int	i;

	mutate_map(data, map_size);
	data->ffmap = malloc(sizeof(char *) * data->tall + 1);
	i = 0;
	while (i < data->tall)
	{
		data->ffmap[i] = ft_strdup(data->map[i]);
		i++;
	}
	data->ffmap[i] = NULL;
	data->start_x = data->px_p;
	data->start_y = data->py_p;
	flud_fil(data, data->py_p / 32, data->px_p / 32);
}
