/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:50 by andefern          #+#    #+#             */
/*   Updated: 2024/12/21 13:02:18 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	flud_fil(t_data *data, int y, int x)
{
	if (data->ffmap[y][x] == '1')
		return ;
	if (x == 0 || y == 0
		|| y == data->tall - 1 || x == data->longest - 1)
		error_matic("Invalid map\n");
	data->ffmap[y][x] = '1';
	flud_fil(data, y - 1, x);
	flud_fil(data, y + 1, x);
	flud_fil(data, y, x + 1);
	flud_fil(data, y, x - 1);
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
