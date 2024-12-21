/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:10:22 by andefern          #+#    #+#             */
/*   Updated: 2024/12/21 13:18:10 by xabier           ###   ########.fr       */
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

void	save_path(char **tex_path, char *check_path)
{
	if (*tex_path)
	{
		free(check_path);
		error_matic("redundant information\n");
	}
	*tex_path = check_path;
}

void	check_lines(t_data *data, char *trimed)
{
	if (trimed[0] == 'N' && trimed[1] == 'O')
		save_path(&data->n_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'S' && trimed[1] == 'O')
		save_path(&data->s_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'E' && trimed[1] == 'A')
		save_path(&data->e_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'W' && trimed[1] == 'E')
		save_path(&data->w_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'F')
		save_path(&data->floor_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'C')
		save_path(&data->sky_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] != '\n')
	{
		free(trimed);
		error_matic("garbaje in the .cub file");
	}
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
