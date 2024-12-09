/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsero.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:10:22 by andefern          #+#    #+#             */
/*   Updated: 2024/12/09 14:41:24 by xortega          ###   ########.fr       */
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

void	valid_character(t_data *data)
{
	int		x;
	int		y;
	bool	player_found;

	x = 0;
	y = 0;
	player_found = false;
	while (data->map[x])
	{
		y = 0;
		while (data->map[x][y])
		{
			if ((data->map[x][y] != '1') && (data->map[x][y] != '0')
			&& (data->map[x][y] != 'N') && (data->map[x][y] != 'S')
			&& (data->map[x][y] != 'E') && (data->map[x][y] != 'W')
			&& (data->map[x][y] != ' ') && (data->map[x][y] != '\n'))
				error_matic("Invalid character found\n");
			if ((data->map[x][y] == 'N') || (data->map[x][y] == 'S')
			|| (data->map[x][y] == 'E') || (data->map[x][y] == 'W'))
			{
				if (player_found == true)
					error_matic("esto es de un jugador pavo\n");
				data->px_p = y * 32;
				data->py_p = x * 32;
				player_found = true;
			}
			y++;
		}
		x++;
	}
}
void	count_characters(t_data *data)
{
	//int	i;
	int x;
	int y;
	int player;

	//i = 0;
	y = 0;
	x = 0;
	player = 0;
	while (data->map[x][y])
	{
		y = 0;
		while (data->map[x][y])
		{
			if (data->map[x][y] == 'N' || data->map[x][y] == 'S' || data->map[x][y] == 'E' || data->map[x][y] == 'W')
				player++;
			y++;
		}
		x++;
	}
	if (player < 1)
		error_matic("Player not found");
	if (player > 1)
		error_matic("Too many players found");
}

int	ber_size(const char *map_path)
{
	char 	*buffer;
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
	//check si el archivo esta bien y mide el alto
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		error_matic("Could not open file\n");
	data->map_str = malloc(sizeof(char *) * (lines + 1));
	//usa el alto para reservar el char**str_map
	while (count < lines)
	{
		data->map_str[count] = get_next_line(fd);
		count++;
	}
	//volcar el archivo entero en el char**str_map
	data->map_str[lines] = NULL;
	close(fd);
	no_se(data, lines);
	//no se por que se llama no se, aqui se parsea todo
}
