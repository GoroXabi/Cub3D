/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsero.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:10:22 by andefern          #+#    #+#             */
/*   Updated: 2024/12/19 14:40:24 by xortega          ###   ########.fr       */
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
	if (len < 5)
		error_matic(".cub not found\n");
	if (file_name[len - 1] != 'b')
		error_matic(".cub not found\n");
	if (file_name[len - 2] != 'u')
		error_matic(".cub not found\n");
	if (file_name[len - 3] != 'c')
		error_matic(".cub not found\n");
	if (file_name[len - 4] != '.')
		error_matic(".cub not found\n");
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
	data->map_str_lines = lines;
	close(fd);
}
