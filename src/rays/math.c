/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:16:18 by xortega           #+#    #+#             */
/*   Updated: 2024/12/19 13:17:18 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

t_pair_d_d	ray_init(double rx, double ry)
{
	t_pair_d_d	ret;

	ret.cx = rx;
	ret.cy = ry;
	return (ret);
}

t_pair_d_d	ray_filler(t_data *data, t_pair_d_d	ret, double xo, double yo)
{
	int			depth;
	int			target_x;
	int			target_y;

	depth = 0;
	while (depth < MAX_DEPTH)
	{
		target_x = ret.cx / DISTANCE;
		target_y = ret.cy / DISTANCE;
		if (target_x >= 0 && target_x < data->map_width
			&& target_y >= 0 && target_y < data->map_height
			&& data->map[target_y][target_x] == '1')
			depth = MAX_DEPTH;
		else
		{
			ret.cx += xo;
			ret.cy += yo;
			depth++;
		}
	}
	return (ret);
}

t_pair_d_d	rayo_v(t_data *data, double x, double y, double ra)
{
	double	rx;
	double	ry;
	double	xo;
	double	yo;

	rx = 0;
	ry = 0;
	xo = 0;
	yo = 0;
	if (ra > PI / 2 && ra <= PI * 3 / 2)
	{
		rx = (((int)(x / DISTANCE)) * DISTANCE) - 0.0001;
		ry = (x - rx) * -tan(ra) + y;
		xo = -DISTANCE;
		yo = -xo * -tan(ra);
	}
	if (ra < PI / 2 || ra > PI * 3 / 2)
	{
		rx = (((int)(x / DISTANCE) + 1) * DISTANCE);
		ry = (x - rx) * -tan(ra) + y;
		xo = DISTANCE;
		yo = -xo * -tan(ra);
	}
	return (ray_filler(data, ray_init(rx, ry), xo, yo));
}

t_pair_d_d	rayo_h(t_data *data, double x, double y, double ra)
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
		ry = (((int)(y / DISTANCE)) * DISTANCE) - 0.0001;
		rx = (y - ry) * -1 / tan(ra) + x;
		yo = -DISTANCE;
		xo = -yo * -1 / tan(ra);
	}
	if (ra < PI)
	{
		ry = (((int)(y / DISTANCE) + 1) * DISTANCE);
		rx = (y - ry) * -1 / tan(ra) + x;
		yo = DISTANCE;
		xo = -yo * -1 / tan(ra);
	}
	return (ray_filler(data, ray_init(rx, ry), xo, yo));
}
