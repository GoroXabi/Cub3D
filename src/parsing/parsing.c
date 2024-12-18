/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:41:00 by xortega           #+#    #+#             */
/*   Updated: 2024/12/18 19:47:26 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

int32_t	make_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
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
			new_tex->tex[y][x] = make_pixel(
					tex->pixels[k + 0],
					tex->pixels[k + 1],
					tex->pixels[k + 2],
					tex->pixels[k + 3]);
			x++;
		}
		y++;
	}
	new_tex->height = tex->height;
	new_tex->width = tex->width;
	return (new_tex);
}

void	free_array(void **array)
{
	int	i;

	i = -1;
	while (array && array[++i])
	{
		free(array[i]);
		array[i] = NULL;
	}
	if (array)
		free(array);
}
