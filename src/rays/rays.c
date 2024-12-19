/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:22:35 by xortega           #+#    #+#             */
/*   Updated: 2024/12/19 13:16:35 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

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

t_pair_d_p	rayo(t_data *data, double angle)
{
	t_pair_d_d	h;
	t_pair_d_d	v;
	t_pair_d_p	ret;
	double		dh;
	double		dv;

	angle = abs_angle(angle);
	h = rayo_h(data, data->px_p, data->py_p, angle);
	v = rayo_v(data, data->px_p, data->py_p, angle);
	dh = sqrt((h.cx - data->px_p) * (h.cx - data->px_p)
			+ (h.cy - data->py_p) * (h.cy - data->py_p));
	dv = sqrt((v.cx - data->px_p) * (v.cx - data->px_p)
			+ (v.cy - data->py_p) * (v.cy - data->py_p));
	if (dh <= dv)
	{
		ret = ret_info(h, dh, angle, data->v_a);
		ret.type = 'h';
	}
	else
	{
		ret = ret_info(v, dv, angle, data->v_a);
		ret.type = 'v';
	}
	return (ret);
}

void	ray_maker(t_data *data)
{
	int		i;
	double	fan;

	i = 0;
	fan = data->v_a - (data->fov / 2 * ANGLE_TO_RADIAN);
	while (i < WIDTH)
	{
		data->rays[i] = rayo(data, fan);
		fan += ((double)data->fov / (double)WIDTH) * (double)ANGLE_TO_RADIAN;
		i++;
	}
	make_screen(data);
}
