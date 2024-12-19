/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psycho_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:50 by andefern          #+#    #+#             */
/*   Updated: 2024/12/19 14:42:47 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

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

void	init_angle(t_data *data, char d)
{
	if (d == 'N')
		data->v_a = NORTH;
	if (d == 'S')
		data->v_a = SOUTH;
	if (d == 'E')
		data->v_a = EAST;
	if (d == 'W')
		data->v_a = WEST;
}

void	count_player(t_data *data)
{
	int	x;
	int	y;
	int	player;

	y = 0;
	player = 0;
	while (data->map[y])
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == 'N' || data->map[y][x] == 'S'
				|| data->map[y][x] == 'E' || data->map[y][x] == 'W')
			{
				player++;
				init_angle(data, data->map[y][x]);
			}
			x++;
		}
		y++;
	}
	if (player < 1)
		error_matic("Player not found");
	if (player > 1)
		error_matic("Too many players found");
}

void	valid_character(t_data *data)
{
	int		x;
	int		y;

	y = 0;
	count_player(data);
	while (data->map[y])
	{
		x = 0;
		while (data->map[y][x])
		{
			if ((data->map[y][x] != '1') && (data->map[y][x] != '0')
			&& (data->map[y][x] != 'N') && (data->map[y][x] != 'S')
			&& (data->map[y][x] != 'E') && (data->map[y][x] != 'W')
			&& (data->map[y][x] != ' ') && (data->map[y][x] != '\n'))
				error_matic("Invalid character found\n");
			if ((data->map[y][x] == 'N') || (data->map[y][x] == 'S')
			|| (data->map[y][x] == 'E') || (data->map[y][x] == 'W'))
			{
				data->px_p = x * 32 + (double)(DISTANCE / 2);
				data->py_p = y * 32 + (double)(DISTANCE / 2);
			}
			x++;
		}
		y++;
	}
}

void	map_checks(t_data *data)
{
	int	i;

	mutate_map(data, data->map_str_lines);
	valid_character(data);
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
