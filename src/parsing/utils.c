/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:41:42 by xortega           #+#    #+#             */
/*   Updated: 2025/02/05 11:37:54 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

bool	check_rgb(uint32_t *rgba, int32_t r, int32_t g, int32_t b)
{
	if (r > 255 || g > 255 || b > 255)
		return (1);
	if (r < 0 || g < 0 || b < 0)
		return (1);
	*rgba = r << 24 | g << 16 | b << 8 | 255;
	return (0);
}

void	longest_line(t_data *data)
{
	int		l;
	size_t	v;

	if (!data->map[0])
	{
		data->longest = 0;
		return ;
	}
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

void	free_textures_path(t_data *data)
{
	free(data->n_tex_path);
	free(data->s_tex_path);
	free(data->e_tex_path);
	free(data->w_tex_path);
	free(data->sky_path);
	free(data->floor_path);
}
