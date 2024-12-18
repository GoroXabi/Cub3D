/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsero.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:10:22 by andefern          #+#    #+#             */
/*   Updated: 2024/12/18 19:48:17 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	error_matic(char *error)
{
	ft_putstr_fd(error, 2);
	exit(1);
}

void	is_cub_file(const char *file_name)
{
	size_t	len;

	len = ft_strlen(file_name);
	if (file_name[len - 1] != 'b')
		error_matic(".cub not found\n");
	if (file_name[len - 2] != 'u')
		error_matic(".cub not found\n");
	if (file_name[len - 3] != 'c')
		error_matic(".cub not found\n");
	if (file_name[len - 4] != '.' || file_name[0] == '.')
		error_matic(".cub not found\n");
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

int	ber_size(const char *map_path)
{
	char	*buffer;
	int		lines;
	int		fd;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		error_matic("Could not open file\n");
	buffer = get_next_line(fd);
	lines = 1;
	if (!buffer)
		return (close(fd), 0);
	while (buffer)
	{
		free(buffer);
		buffer = get_next_line(fd);
		lines++;
	}
	close(fd);
	return (lines);
}

void	get_map_str(t_data *data, const char *map_path)
{
	int		fd;
	int		lines;
	int		count;

	count = 0;
	lines = ber_size(map_path);
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		error_matic("Could not open file\n");
	data->map_str = malloc(sizeof(char *) * (lines + 1));
	while (count < lines)
	{
		data->map_str[count] = get_next_line(fd);
		count++;
	}
	data->map_str[lines] = NULL;
	close(fd);
	map_checks(data, lines);
}
