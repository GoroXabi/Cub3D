/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:45:31 by xortega           #+#    #+#             */
/*   Updated: 2024/12/19 14:08:23 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	art_attack(t_data *data, int col)
{
	int	start;

	start = 0;
	while (start < HEIGHT / 2)
	{
		mlx_put_pixel(data->screen_image, col, start, data->sky_color);
		start++;
	}
	while (start < HEIGHT)
	{
		mlx_put_pixel(data->screen_image, col, start, data->floor_color);
		start++;
	}
}

void	print_walls(t_data *data, int col)
{
	int		start;

	start = 0;
	while (start < data->wall_size)
	{
		if (data->rays[col].angle < PI && data->rays[col].type == 'h')
			resize_n(data, col, start);
		if (data->rays[col].angle > PI && data->rays[col].type == 'h')
			resize_s(data, col, start);
		if ((data->rays[col].angle > PIH && data->rays[col].angle < PI3H)
			&& data->rays[col].type == 'v')
			resize_w(data, col, start);
		if ((data->rays[col].angle > PI3H || data->rays[col].angle < PIH)
			&& data->rays[col].type == 'v')
			resize_e(data, col, start);
		start++;
	}
}

void	make_screen(t_data *data)
{
	int		col;

	col = 0;
	while (col < WIDTH)
	{
		data->zoom = 0;
		data->wall_size = (HEIGHT / data->rays[col].lenght) * 128;
		if (data->wall_size > HEIGHT)
		{
			data->zoom = data->wall_size - HEIGHT;
			data->wall_size = HEIGHT;
		}
		data->sky_size = (HEIGHT - data->wall_size)
			/ 2 + ((HEIGHT - data->wall_size) % 2);
		art_attack(data, col);
		print_walls(data, col);
		col++;
	}
}
