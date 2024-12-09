/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/12/05 13:47:12 by xortega          ###   ########.fr       */
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
/*	return (((((pixel >> 24) & 0xFF) * (pixel & 0xFF)
				+ ((background >> 24) & 0xFF) * (255 - (pixel & 0xFF))) / 255)
		<< 24
		| ((((pixel >> 16) & 0xFF) * (pixel & 0xFF)
				+ ((background >> 16) & 0xFF) * (255 - (pixel & 0xFF))) / 255)
		<< 16
		| ((((pixel >> 8) & 0xFF) * (pixel & 0xFF)
				+ ((background >> 8) & 0xFF) * (255 - (pixel & 0xFF))) / 255)
		<< 8
		| 255);*/
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

void	print_walls(t_data *data, int col, int wall_size, int zoom)
{
	int		start;
	int		sky_size;
	int		tx;
	int		ty;
	double	medium;

	sky_size = (HEIGHT - wall_size) / 2 + ((HEIGHT - wall_size) % 2);
	start = 0;
	while (start < wall_size)
	{
		if (data->rays[col].angle < PI && data->rays[col].type == 'h')
		{
			medium = data->rays[col].pair.first * data->n_texture->width / 32;
			tx = (int)medium % data->n_texture->width;
			ty = ((start + zoom / 2) * data->n_texture->height)
				/ (wall_size + zoom);
			if (data->n_texture->texture[ty][(data->n_texture->width - 1) - tx]
				& 0xFF)
				mlx_put_pixel(data->screen_image, col, start + sky_size,
					color_mixer(calc_trans(data, start + sky_size), data->n_texture->texture[ty]
				[(data->n_texture->width - 1) - tx]));
		}
		if (data->rays[col].angle > PI && data->rays[col].type == 'h')
		{
			medium = data->rays[col].pair.first * data->s_texture->width;
			tx = (int)(medium / 32) % data->s_texture->width;
			ty = ((start + zoom / 2) * data->s_texture->height)
				/ (wall_size + zoom);
			if (data->s_texture->texture[ty][tx] & 0xFF)
				mlx_put_pixel(data->screen_image, col, start + sky_size,
					color_mixer(calc_trans(data, start + sky_size), data->s_texture->texture[ty][tx]));
		}
		if ((data->rays[col].angle > PIH && data->rays[col].angle < PI3H)
			&& data->rays[col].type == 'v')
		{
			medium = data->rays[col].pair.second * data->w_texture->width;
			tx = (int)(medium / 32) % data->w_texture->width;
			ty = ((start + zoom / 2)
					* data->w_texture->height) / (wall_size + zoom);
			if (data->w_texture->texture[ty][(data->w_texture->width - 1) - tx]
				& 0xFF)
				mlx_put_pixel(data->screen_image, col, start + sky_size,
					color_mixer(calc_trans(data, start + sky_size), data->w_texture->texture[ty]
				[(data->w_texture->width - 1) - tx]));
		}
		if ((data->rays[col].angle > PI3H || data->rays[col].angle < PIH)
			&& data->rays[col].type == 'v')
		{
			medium = data->rays[col].pair.second * data->e_texture->width;
			tx = (int)(medium / 32) % data->e_texture->width;
			ty = ((start + zoom / 2)
					* data->e_texture->height) / (wall_size + zoom);
			if (data->e_texture->texture[ty][tx] & 0xFF)
				mlx_put_pixel(data->screen_image, col, start + sky_size,
					color_mixer(calc_trans(data, start + sky_size), data->e_texture->texture[ty][tx]));
		}
		start++;
	}
}

void	make_screen(t_data *data)
{
	int	wall_size;
	int	zoom;
	int	col;

	col = 0;
	while (col < WIDTH)
	{
		zoom = 0;
		wall_size = (HEIGHT / data->rays[col].lenght) * 32;
		if (wall_size > HEIGHT)
		{
			zoom = wall_size - HEIGHT;
			wall_size = HEIGHT;
		}
		art_attack(data, col);
		print_walls(data, col, wall_size, zoom);
		col++;
	}
}
void	upd_p(t_data *data, double dx, double dy)
{
	double	deltax;
	double	deltay;

	deltax = data->px_p + dx;
	deltay = data->py_p + dy;
	if (data->map[(int)deltay / 32][(int)deltax / 32] != 1)
	{
		data->px_p += dx;
		data->py_p += dy;
	}
}

void	ft_hook(void *param)
{
	t_data	*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		upd_p(data, cos(data->v_a) * 2, sin(data->v_a) * 2);
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP))
		data->fov += 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN))
		data->fov -= 1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		upd_p(data, -cos(data->v_a) * 2, -sin(data->v_a) * 2);
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		upd_p(data, cos(data->v_a + PIH) * 2, sin(data->v_a + PIH) * 2);
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		upd_p(data, -cos(data->v_a + PIH) * 2, -sin(data->v_a + PIH) * 2);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->v_a -= 0.1;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->v_a += 0.1;
	if (data->v_a < 0)
		data->v_a = 2 * PI;
	if (data->v_a > 2 * PI)
		data->v_a = 0;
	if (mlx_is_key_down(data->mlx, MLX_KEY_O))
		printf(HBLU"player cords : (%f|%f)\n"HGRE"vision angle : (%f)\n"HRED"fov : (%d)\n", data->px_p, data->py_p, data->v_a, data->fov);
	ray_maker(data);
}

void	free_textures(t_texture *texture)
{
	int	i;

	i = 0;
	while (i < texture->height)
	{
		free(texture->texture[i]);
		i++;
	}
	free(texture);
}

void	free_data(t_data *data)
{
	mlx_delete_image(data->mlx, data->screen_image);
	free_textures(data->n_texture);
	free_textures(data->s_texture);
	free_textures(data->e_texture);
	free_textures(data->w_texture);
	mlx_delete_texture(data->north_texture);
	mlx_delete_texture(data->south_texture);
	mlx_delete_texture(data->east_texture);
	mlx_delete_texture(data->west_texture);
}

void init_data(t_data *data)
{
	data->map = malloc(sizeof(int *) * 10);
	for (size_t i = 0; i < 10; i++)
		data->map[i] = malloc(sizeof(int) * 10);
	for (size_t i = 0; i < 10; i++)
	{
		data->map[0][i] = 1;
		data->map[9][i] = 1;
		data->map[i][0] = 1;
		data->map[i][9] = 1;
	}
	data->map[4][4] = 1;
	data->map[0][0] = 0;
	data->map[0][9] = 0;
	data->map[9][0] = 0;
	data->map[9][9] = 0;
	data->fov = 60;
	data->start_x = 75;
	data->start_y = 75;
	data->map_width = 10;
	data->map_height = 10;
	data->sky_color = make_pixel(210, 140, 70, 255);
	data->floor_color = make_pixel(145, 145, 145, 255);
	data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->north_texture = mlx_load_png("b.png");
	data->south_texture = mlx_load_png("pedra.png");
	data->east_texture = mlx_load_png("coche.png");
	data->west_texture = mlx_load_png("emo.png");
	data->n_texture = make_texture(data->north_texture);
	data->s_texture = make_texture(data->south_texture);
	data->e_texture = make_texture(data->east_texture);
	data->w_texture = make_texture(data->west_texture);
	data->north_texture_path = NULL;
	data->south_texture_path = NULL;
	data->east_texture_path = NULL;
	data->west_texture_path = NULL;
	data->speed = 2;
	data->px_p = data->start_x;
	data->py_p = data->start_y;
	data->v_a = NORTH;
}

int32_t main(int argc, char **argv)
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
