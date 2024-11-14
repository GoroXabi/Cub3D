/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 09:22:35 by xortega           #+#    #+#             */
/*   Updated: 2024/11/14 17:38:41 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

int MAP[10][10] = {

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

t_pair_d_d	rayo_v(int32_t x, int32_t y, double angle)
{
	int depth = 0;
	int target_x;
	int target_y;

	double rx = 0;
	double ry = 0;
	double ra;

	double xo = 0;
	double yo = 0;
	
	ra = angle;

	float nTan = -tan(ra);

	if (ra > M_PI / 2 && ra < M_PI * 3/2)
	{
		rx = ((x / 32) * 32) - 0.0001;
		ry = (x - rx) * nTan + y;
		xo = -32;
		yo = -xo * nTan;
	}

	if (ra < M_PI / 2 || ra > M_PI * 3/2)
	{
		rx = (((x / 32) + 1) * 32);
		ry = (x - rx) *  nTan + y;
		xo = 32;
		yo = -xo * nTan;
	}

	if (ra == M_PI / 2 || ra == M_PI * 3/2)
	{
		rx = x;
		ry = y;
		depth = MAX_DEPTH;
		target_x = 0;
		target_y = 0;
	}

	while (depth < MAX_DEPTH)
	{
		target_x = (int)rx / 32;
		target_y = (int)ry / 32;
		if (target_x >= 0 && target_x <= MAP_SIZE
			&& target_y >= 0 && target_y <= MAP_SIZE
				&& MAP[target_y][target_x] == 1)
			depth = MAX_DEPTH;
		else
		{
			rx += xo;
			ry += yo;
			depth++;
		}
	}
	
	t_pair_d_d ret;
	ret.first = rx;
	ret.second = ry;
	return(ret);
}

t_pair_d_d	rayo_h(int32_t x, int32_t y, double angle)
{
	int depth = 0;
	int target_x;
	int target_y;

	double rx = 0;
	double ry = 0;
	double ra;

	double xo = 0;
	double yo = 0;
	
	ra = angle;

	float aTan = -1/tan(ra);
	if (ra > M_PI)
	{
		ry = ((y / 32) * 32) - 0.0001;
		rx = (y - ry) * aTan + x;
		yo = -32;
		xo = -yo * aTan;
	}

	if (ra < M_PI)
	{
		ry = (((y / 32) + 1) * 32);
		rx = (y - ry) * aTan + x;
		yo = 32;
		xo = -yo * aTan;
	}

	if (ra == 0 || ra == M_PI)
	{
		rx = x;
		ry = y;
		depth = MAX_DEPTH;
		target_x = 0;
		target_y = 0;
	}

	while (depth < MAX_DEPTH)
	{
		target_x = (int)rx / 32;
		target_y = (int)ry / 32;
		if (target_x >= 0 && target_x <= MAP_SIZE
			&& target_y >= 0 && target_y <= MAP_SIZE
				&& MAP[target_y][target_x] == 1)
			depth = MAX_DEPTH;
		else
		{
			rx += xo;
			ry += yo;
			depth++;
		}
	}
	
	t_pair_d_d ret;
	ret.first = rx;
	ret.second = ry;
	return(ret);
}

t_pair_d_p  rayo(t_data *data, double angle)
{
	t_pair_d_d h;
	t_pair_d_d v;

	t_pair_d_p ret;

	if (angle < 0)
		angle += 2 * M_PI;
	if (angle > 2 * M_PI)
		angle -= 2 * M_PI;
		
	ret.angle = angle;
	h = rayo_h(data->px_position, data->py_position, angle);
	v = rayo_v(data->px_position,data->py_position, angle);

	double dh;
	double dv;

	dh = sqrt((h.first - data->px_position) * (h.first - data->px_position) + (h.second -data->py_position) * (h.second -data->py_position));
	dv = sqrt((v.first - data->px_position) * (v.first - data->px_position) + (v.second -data->py_position) * (v.second -data->py_position));

	if (dh <= dv)
	{
		ret.lenght = fabs(dh * cos(angle - data->view_angle));
		ret.pair = h;
		ret.type = 'h';
		return(ret);
	}
	else
	{
		ret.lenght = fabs(dv * cos(angle - data->view_angle));
		ret.pair = v;
		ret.type = 'v';
		return(ret);
	}
}
void ray_maker(t_data *data)
{
	int	i = 0;

	double fan;


	fan = data->view_angle - (FOV/2 * ANGLE_TO_RADIAN);

	while (i < WIDTH)
	{
		data->rays[i] = rayo(data, fan);
		fan += ((double)FOV / (double)WIDTH) * (double)ANGLE_TO_RADIAN;
		i++;
	}
	print_walls(data);
}
