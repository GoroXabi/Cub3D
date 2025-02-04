/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_str.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:10:22 by andefern          #+#    #+#             */
/*   Updated: 2025/02/04 11:12:26 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	is_cub_file(const char *file_name)
{
	size_t	len;

	len = ft_strlen(file_name);
	if (len < 5 || file_name[len - 1] != 'b' || file_name[len - 2] != 'u' || file_name[len - 3] != 'c' || file_name[len - 4] != '.')
	{
		ft_putstr_fd("archive not valid", 2);
		exit(1);
	}
}

int	ber_size(t_data *data, const char *map_path)
{
	char	*buffer;
	int		lines;
	int		fd;

	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		error_matic("Could not open file\n", data, 0);
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

void	save_path(t_data *data, char **tex_path, char *check_path)
{
	if (*tex_path)
	{
		free(check_path);
		error_matic("redundant information\n", data, 1);
	}
	*tex_path = check_path;
}

void	check_lines(t_data *data, char *trimed)
{
	if (trimed[0] == 'N' && trimed[1] == 'O')
		save_path(data, &data->n_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'S' && trimed[1] == 'O')
		save_path(data, &data->s_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'E' && trimed[1] == 'A')
		save_path(data, &data->e_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'W' && trimed[1] == 'E')
		save_path(data, &data->w_tex_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'F')
		save_path(data, &data->floor_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] == 'C')
		save_path(data, &data->sky_path, ft_strtrim(trimed + 2, " \t\n"));
	else if (trimed[0] != '\n')
	{
		free(trimed);
		error_matic("garbaje in the .cub file\n", data, 1);
	}
}

void	get_map_str(t_data *data, const char *map_path)
{
	int		fd;
	int		lines;
	int		count;

	count = 0;
	lines = ber_size(data, map_path);
	fd = open(map_path, O_RDONLY);
	if (fd < 0)
		error_matic("Could not open file\n", data, 0);
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
