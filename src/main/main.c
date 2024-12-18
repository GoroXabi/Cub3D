/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/12/18 19:57:48 by xortega          ###   ########.fr       */
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

int32_t	calc_trans(t_data *data, int row)
{
	if (row < HEIGHT / 2)
		return (data->sky_color);
	return (data->floor_color);
}

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

void	resize_n(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;

	medium = data->rays[col].pair.cx * data->n_tex->width / 32;
	tx = (int)medium % data->n_tex->width;
	ty = ((start + data->zoom / 2) * data->n_tex->height)
		/ (data->wall_size + data->zoom);
	mlx_put_pixel(data->screen_image, col, start + data->sky_size,
		color_mixer(calc_trans(data, start + data->sky_size),
			data->n_tex->tex[ty][(data->n_tex->width - 1) - tx]));
}

void	resize_s(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;

	medium = data->rays[col].pair.cx * data->s_tex->width;
	tx = (int)(medium / 32) % data->s_tex->width;
	ty = ((start + data->zoom / 2) * data->s_tex->height)
		/ (data->wall_size + data->zoom);
	mlx_put_pixel(data->screen_image, col, start + data->sky_size,
		color_mixer(calc_trans(data, start + data->sky_size),
			data->s_tex->tex[ty][tx]));
}

void	resize_w(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;

	medium = data->rays[col].pair.cy * data->w_tex->width;
	tx = (int)(medium / 32) % data->w_tex->width;
	ty = ((start + data->zoom / 2)
			* data->w_tex->height) / (data->wall_size + data->zoom);
	mlx_put_pixel(data->screen_image, col, start + data->sky_size,
		color_mixer(calc_trans(data, start + data->sky_size),
			data->w_tex->tex[ty][(data->w_tex->width - 1) - tx]));
}

void	resize_e(t_data *data, int col, int start)
{
	size_t	tx;
	size_t	ty;
	double	medium;

	medium = data->rays[col].pair.cy * data->e_tex->width;
	tx = (int)(medium / 32) % data->e_tex->width;
	ty = ((start + data->zoom / 2)
			* data->e_tex->height) / (data->wall_size + data->zoom);
	mlx_put_pixel(data->screen_image, col, start + data->sky_size,
		color_mixer(calc_trans(data, start + data->sky_size),
			data->e_tex->tex[ty][tx]));
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
		data->wall_size = (HEIGHT / data->rays[col].lenght) * 64;
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

void	ft_hook(void *param)
{
	t_data	*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	check_movements(data);
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP))
		data->fov += 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN))
		data->fov -= 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->v_a -= 0.1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->v_a += 0.1;
	if (data->v_a < 0)
		data->v_a = 2 * PI;
	if (data->v_a > 2 * PI)
		data->v_a = 0;
	ray_maker(data);
}

void	free_textures(t_tex *texture)
{
	int	i;

	i = 0;
	while (i < texture->height)
	{
		free(texture->tex[i]);
		i++;
	}
	free(texture);
}

void	free_data(t_data *data)
{
	mlx_delete_image(data->mlx, data->screen_image);
	free_textures(data->n_tex);
	free_textures(data->s_tex);
	free_textures(data->e_tex);
	free_textures(data->w_tex);
	mlx_delete_texture(data->north_tex);
	mlx_delete_texture(data->south_tex);
	mlx_delete_texture(data->east_tex);
	mlx_delete_texture(data->west_tex);
}

void	init_data(t_data *data)
{
	data->fov = 60;
	data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->north_tex_path = NULL;
	data->south_tex_path = NULL;
	data->east_tex_path = NULL;
	data->west_tex_path = NULL;
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 2)
		return (0);
	data = malloc(sizeof(t_data));
	init_data(data);
	get_map_str(data, argv[1]);
	printf("init_ok\n");
	mlx_image_to_window(data->mlx, data->screen_image, 0, 0);
	printf("image_to_window_ok\n");
	ray_maker(data);
	mlx_loop_hook(data->mlx, ft_hook, data);
	mlx_loop(data->mlx);
	free_data(data);
	mlx_terminate(data->mlx);
	free(data);
	return (EXIT_SUCCESS);
}
