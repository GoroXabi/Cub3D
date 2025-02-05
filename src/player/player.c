/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:48:27 by xortega           #+#    #+#             */
/*   Updated: 2025/02/05 10:42:45 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	upd_p(t_data *data, double dx, double dy)
{
	double	deltax;
	double	deltay;

	deltax = data->px_p + dx;
	deltay = data->py_p + dy;
	if (deltax > data->map_width * 32 || deltay > data->map_height * 32
		|| deltax < 0 || deltay < 0)
		return ;
	if (data->map[(int)data->py_p / 32][(int)deltax / 32] != '1')
		data->px_p += dx;
	if (data->map[(int)deltay / 32][(int)data->px_p / 32] != '1')
		data->py_p += dy;
}

void	check_movements(t_data *data)
{
	int		vel;

	vel = 2;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
		vel += 4;
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		upd_p(data, cos(data->v_a) * vel, sin(data->v_a) * vel);
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		upd_p(data, -cos(data->v_a) * vel, -sin(data->v_a) * vel);
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		upd_p(data, cos(data->v_a + PIH) * vel, sin(data->v_a + PIH) * vel);
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		upd_p(data, -cos(data->v_a + PIH) * vel, -sin(data->v_a + PIH) * vel);
}

void	check_cam(t_data *data)
{
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP))
		data->fov += 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN))
		data->fov -= 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_R))
		data->fov = 50;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->v_a -= 0.1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->v_a += 0.1;
	if (data->v_a < 0)
		data->v_a = 2 * PI;
	if (data->v_a > 2 * PI)
		data->v_a = 0;
}
