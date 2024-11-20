/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/11/20 12:07:57 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	print_screen(t_data *data)
{
	int	i;
	int	k;

	i = 0;
	while (i < WIDTH)
	{
		k = 0;
		while (k < HEIGHT)
		{
			mlx_put_pixel(data->screen_image, i, k, data->screen[i][k]);
			k++;
		}
		i++;
	}
}

void	paco_sainz(t_data *data, int row, int wall_size, int zoom)
{
	int	col;
	int	pixel;
	int	start;
	int	sky_size;

	sky_size = (HEIGHT - wall_size) / 2 + ((HEIGHT - wall_size) % 2);
	if (data->rays[row].type == 'h')
		col = (int)(data->rays[row].pair.first) % 32;
	else
		col = (int)(data->rays[row].pair.second) % 32;
	start = sky_size - 1;
	while (++start < (HEIGHT - sky_size))
	{
		pixel = ((start - sky_size + zoom / 2) * 32) / (wall_size + zoom);
		if (data->rays[row].angle < PI && data->rays[row].type == 'h')
			data->screen[row][start] = data->n_texture->texture[col][pixel];
		if (data->rays[row].angle > PI && data->rays[row].type == 'h')
			data->screen[row][start] = data->s_texture->texture[col][pixel];
		if ((data->rays[row].angle < PI / 2
				|| data->rays[row].angle > PI3H) && data->rays[row].type == 'v')
			data->screen[row][start] = data->e_texture->texture[col][pixel];
		if ((data->rays[row].angle > PI / 2
				&& data->rays[row].angle < PI3H) && data->rays[row].type == 'v')
			data->screen[row][start] = data->w_texture->texture[col][pixel];
	}
}

void	art_attack(t_data *data, int row, int sky_size, int wall_size)
{
	int	start;

	start = 0;
	while (start < sky_size)
	{
		data->screen[row][start] = data->sky_color;
		start++;
	}
	start += wall_size;
	while (start < HEIGHT)
	{
		data->screen[row][start] = data->floor_color;
		start++;
	}
}

void	print_walls(t_data *data)
{
	int	wall_size;
	int	zoom;
	int	sky_size;
	int	i;

	i = 0;
	zoom = 0;
	while (i < WIDTH)
	{
		wall_size = (HEIGHT / data->rays[i].lenght) * 32;
		if (wall_size > HEIGHT)
		{
			zoom = wall_size - HEIGHT;
			wall_size = HEIGHT;
		}
		paco_sainz(data, i, wall_size, zoom);
		sky_size = (HEIGHT - wall_size) / 2 + ((HEIGHT - wall_size) % 2);
		art_attack(data, i, sky_size, wall_size);
		i++;
	}
	print_screen(data);
}

void	upd_p(t_data *data, double dx, double dy)
{
	data->px_p += dx;
	data->py_p += dy;
}

void	ft_hook(void *param)
{
	t_data	*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		upd_p(data, cos(data->v_a) * 2, sin(data->v_a) * 2);
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
	data->start_x = 75;
	data->start_y = 75;
	data->map_width = 10;
	data->map_height = 10;
	data->sky_color = 0xFFA500FF;
	data->floor_color = 0x800080FF;
	data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->north_texture = mlx_load_png("minecra.png");
	data->south_texture = mlx_load_png("TPG.png");
	data->east_texture = mlx_load_png("tom.png");
	data->west_texture = mlx_load_png("syl.png");
	data->n_texture = make_texture(data->north_texture);
	data->s_texture = make_texture(data->south_texture);
	data->e_texture = make_texture(data->east_texture);
	data->w_texture = make_texture(data->west_texture);
	data->speed = 2;
	data->px_p = data->start_x;
	data->py_p = data->start_y;
	data->v_a = NORTH;
}

int32_t main(void)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	init_data(data);
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
