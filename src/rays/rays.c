/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:22:35 by xortega           #+#    #+#             */
/*   Updated: 2024/11/20 11:42:13 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

#define DISTANCE 32

int	g_map[10][10] = {
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

t_pair_d_d	ray_filler(double rx, double ry, double xo, double yo)
{
	int			depth;
	int			target_x;
	int			target_y;
	t_pair_d_d	ret;

	depth = 0;
	while (depth < MAX_DEPTH)
	{
		target_x = (int)rx / DISTANCE;
		target_y = (int)ry / DISTANCE;
		if (target_x >= 0 && target_x <= MAP_SIZE
			&& target_y >= 0 && target_y <= MAP_SIZE
			&& g_map[target_y][target_x] == 1)
			depth = MAX_DEPTH;
		else
		{
			rx += xo;
			ry += yo;
			depth++;
		}
	}
	ret.first = rx;
	ret.second = ry;
	return (ret);
}

t_pair_d_d	rayo_v(int32_t x, int32_t y, double ra)
{
	double	rx;
	double	ry;
	double	xo;
	double	yo;

	rx = 0;
	ry = 0;
	xo = 0;
	yo = 0;
	if (ra > PI / 2 && ra < PI * 3 / 2)
	{
		rx = ((x / DISTANCE) * DISTANCE) - 0.0001;
		ry = (x - rx) * -tan(ra) + y;
		xo = -DISTANCE;
		yo = -xo * -tan(ra);
	}
	if (ra < PI / 2 || ra > PI * 3 / 2)
	{
		rx = (((x / DISTANCE) + 1) * DISTANCE);
		ry = (x - rx) * -tan(ra) + y;
		xo = DISTANCE;
		yo = -xo * -tan(ra);
	}
	return (ray_filler(rx, ry, xo, yo));
}

t_pair_d_d	rayo_h(int32_t x, int32_t y, double ra)
{
	double	rx;
	double	ry;
	double	xo;
	double	yo;

	rx = 0;
	ry = 0;
	xo = 0;
	yo = 0;
	if (ra > PI)
	{
		ry = ((y / DISTANCE) * DISTANCE) - 0.0001;
		rx = (y - ry) * -1 / tan(ra) + x;
		yo = -DISTANCE;
		xo = -yo * -1 / tan(ra);
	}
	if (ra < PI)
	{
		ry = (((y / DISTANCE) + 1) * DISTANCE);
		rx = (y - ry) * -1 / tan(ra) + x;
		yo = DISTANCE;
		xo = -yo * -1 / tan(ra);
	}
	return (ray_filler(rx, ry, xo, yo));
}

t_pair_d_p	ret_info(t_pair_d_d ray, double length, double angle, double v_a)
{
	t_pair_d_p	ret;

	ret.angle = angle;
	ret.lenght = fabs(length * cos(angle - v_a));
	ret.pair = ray;
	return (ret);
}

double	abs_angle(double angle)
{
	if (angle < 0)
		angle += 2 * PI;
	if (angle > 2 * PI)
		angle -= 2 * PI;
	return (angle);
}

t_pair_d_p  rayo(t_data *data, double angle)
{
	t_pair_d_d	h;
	t_pair_d_d	v;
	t_pair_d_p	ret;
	double		dh;
	double		dv;

	angle = abs_angle(angle);
	h = rayo_h(data->px_p, data->py_p, angle);
	v = rayo_v(data->px_p, data->py_p, angle);
	dh = sqrt((h.first - data->px_p) * (h.first - data->px_p)
			+ (h.second - data->py_p) * (h.second - data->py_p));
	dv = sqrt((v.first - data->px_p) * (v.first - data->px_p)
			+ (v.second - data->py_p) * (v.second - data->py_p));
	if (dh <= dv)
	{
		ret = ret_info(h, dh, angle, data->v_a);
		ret.type = 'h';
		return (ret);
	}
	else
	{
		ret = ret_info(v, dv, angle, data->v_a);
		ret.type = 'v';
		return (ret);
	}
}

void	ray_maker(t_data *data)
{
	int		i;
	double	fan;

	i = 0;
	fan = data->v_a - (FOV / 2 * ANGLE_TO_RADIAN);
	while (i < WIDTH)
	{
		data->rays[i] = rayo(data, fan);
		fan += ((double)FOV / (double)WIDTH) * (double)ANGLE_TO_RADIAN;
		i++;
	}
	print_walls(data);
}
