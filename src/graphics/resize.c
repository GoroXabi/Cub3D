/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:46:58 by xortega           #+#    #+#             */
/*   Updated: 2024/12/19 12:34:22 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

int32_t	color_mixer(int32_t background, int32_t pixel)
{
	int32_t	new_r;
	int32_t	new_g;
	int32_t	new_b;

	new_r = (((pixel >> 24) & 0xFF) * (pixel & 0xFF)
			+ ((background >> 24) & 0xFF) * (255 - (pixel & 0xFF))) / 255;
	new_g = (((pixel >> 16) & 0xFF) * (pixel & 0xFF)
			+ ((background >> 16) & 0xFF) * (255 - (pixel & 0xFF))) / 255;
	new_b = (((pixel >> 8) & 0xFF) * (pixel & 0xFF)
			+ ((background >> 8) & 0xFF) * (255 - (pixel & 0xFF))) / 255;
	return ((new_r << 24) | (new_g << 16) | (new_b << 8) | 255);
}

void	resize_n(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;
	int32_t	back_color;
	int32_t	pixel_color;

	if (start + data->sky_size < HEIGHT / 2)
		back_color = data->sky_color;
	else
		back_color = data->floor_color;
	medium = data->rays[col].pair.cx * data->n_tex->width / 32;
	tx = (int)medium % data->n_tex->width;
	ty = ((start + data->zoom / 2) * data->n_tex->height)
		/ (data->wall_size + data->zoom);
	pixel_color = color_mixer(back_color,
			data->n_tex->tex[ty][(data->n_tex->width - 1) - tx]);
	mlx_put_pixel(data->screen_image,
		col, start + data->sky_size, pixel_color);
}

void	resize_s(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;
	int32_t	back_color;
	int32_t	pixel_color;

	if (start + data->sky_size < HEIGHT / 2)
		back_color = data->sky_color;
	else
		back_color = data->floor_color;
	medium = data->rays[col].pair.cx * data->s_tex->width;
	tx = (int)(medium / 32) % data->s_tex->width;
	ty = ((start + data->zoom / 2) * data->s_tex->height)
		/ (data->wall_size + data->zoom);
	pixel_color = color_mixer(back_color, data->s_tex->tex[ty][tx]);
	mlx_put_pixel(data->screen_image,
		col, start + data->sky_size, pixel_color);
}

void	resize_w(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;
	int32_t	back_color;
	int32_t	pixel_color;

	if (start + data->sky_size < HEIGHT / 2)
		back_color = data->sky_color;
	else
		back_color = data->floor_color;
	medium = data->rays[col].pair.cy * data->w_tex->width;
	tx = (int)(medium / 32) % data->w_tex->width;
	ty = ((start + data->zoom / 2)
			* data->w_tex->height) / (data->wall_size + data->zoom);
	pixel_color = color_mixer(back_color,
			data->w_tex->tex[ty][(data->w_tex->width - 1) - tx]);
	mlx_put_pixel(data->screen_image,
		col, start + data->sky_size, pixel_color);
}

void	resize_e(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;
	int32_t	back_color;
	int32_t	pixel_color;

	if (start + data->sky_size < HEIGHT / 2)
		back_color = data->sky_color;
	else
		back_color = data->floor_color;
	medium = data->rays[col].pair.cy * data->e_tex->width;
	tx = (int)(medium / 32) % data->e_tex->width;
	ty = ((start + data->zoom / 2)
			* data->e_tex->height) / (data->wall_size + data->zoom);
	pixel_color = color_mixer(back_color, data->e_tex->tex[ty][tx]);
	mlx_put_pixel(data->screen_image,
		col, start + data->sky_size, pixel_color);
}
