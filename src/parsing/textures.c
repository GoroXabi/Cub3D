/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:41:00 by xortega           #+#    #+#             */
/*   Updated: 2025/02/05 11:52:02 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	make_floor_sky(t_data *data, char *line, uint32_t *rgba)
{
	int		count;
	char	**splited;

	count = 0;
	splited = ft_split(line, ',');
	while (splited[count])
		count++;
	if (count != 3)
	{
		free_array((void **)splited);
		error_matic("Ceiling or floor color count not valid\n", data, 4);
	}
	if (check_rgb(rgba, ft_atoi(splited[0])
			, ft_atoi(splited[1])
			, ft_atoi(splited[2])))
	{
		free_array((void **)splited);
		error_matic("Ceiling or floor color out of range\n", data, 4);
	}
	free_array((void **)splited);
}

t_tex	*init_tex(mlx_texture_t *tex)
{
	size_t		i;
	t_tex		*new_tex;

	new_tex = malloc(sizeof(t_tex));
	new_tex->tex = malloc(sizeof(int32_t *) * tex->height);
	i = 0;
	while (i < tex->height)
	{
		new_tex->tex[i] = malloc(sizeof(int32_t) * tex->width);
		i++;
	}
	return (new_tex);
}

t_tex	*make_texture(mlx_texture_t *tex)
{
	uint32_t	y;
	uint32_t	x;
	uint32_t	k;
	t_tex		*new_tex;

	new_tex = init_tex(tex);
	y = 0;
	while (y < tex->height)
	{
		x = 0;
		while (x < tex->width)
		{
			k = y * tex->width * 4 + x * 4;
			new_tex->tex[y][x] = tex->pixels[k + 0] << 24
				| tex->pixels[k + 1] << 16
				| tex->pixels[k + 2] << 8
				| tex->pixels[k + 3];
			x++;
		}
		y++;
	}
	new_tex->height = tex->height;
	new_tex->width = tex->width;
	return (new_tex);
}

void	import_textures(t_data *data)
{
	data->north_tex = mlx_load_png(data->n_tex_path);
	data->south_tex = mlx_load_png(data->s_tex_path);
	data->east_tex = mlx_load_png(data->e_tex_path);
	data->west_tex = mlx_load_png(data->w_tex_path);
	if (!data->north_tex
		|| !data->south_tex
		|| !data->east_tex
		|| !data->west_tex)
		error_matic("Error importing texture\n", data, 2);
	data->n_tex = make_texture(data->north_tex);
	data->s_tex = make_texture(data->south_tex);
	data->e_tex = make_texture(data->east_tex);
	data->w_tex = make_texture(data->west_tex);
	make_floor_sky(data, data->floor_path, &data->floor_color);
	make_floor_sky(data, data->sky_path, &data->sky_color);
}

void	get_textures(t_data *data)
{
	int		line;
	char	*trimed;

	line = -1;
	while (data->map_str[++line]
		&& !(data->n_tex_path && data->s_tex_path && data->e_tex_path
			&& data->w_tex_path && data->floor_path && data->sky_path))
	{
		trimed = ft_strtrim(data->map_str[line], "\t ");
		check_lines(data, trimed);
		free(trimed);
	}
	data->info_end = line;
	import_textures(data);
}
