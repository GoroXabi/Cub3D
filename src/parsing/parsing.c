/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:41:00 by xortega           #+#    #+#             */
/*   Updated: 2024/11/14 17:41:42 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

int32_t make_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

int32_t **make_texture(mlx_texture_t* texture)
{
	uint32_t y;
	uint32_t x;
	uint32_t k;
	int32_t **new_texture;
	
	new_texture = malloc(sizeof(int32_t *) * texture->height);
	for (size_t i = 0; i < texture->height; i++)
	{
		new_texture[i] = malloc(sizeof(int32_t) * texture->width);
	}
	
	for (y = 0; y < texture->height; y++)
	{
		for (x = 0; x < texture->width; x++)
		{
			k = y*texture->width*4 + x*4;
			new_texture[x][y] = make_pixel(texture->pixels[k + 0], texture->pixels[k + 1], texture->pixels[k + 2], texture->pixels[k + 3]);
		}
	}
	return(new_texture);
}