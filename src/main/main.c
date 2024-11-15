/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/11/15 13:52:22 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void print_screen(t_data *data)
{
	for (int i = 0; i < WIDTH; i++)
	{

		for (int k = 0; k < HEIGHT; k++)
		{
			mlx_put_pixel(data->screen_image, i,  k, data->screen[i][k]);
		}
	}
}

void print_walls(t_data *data)
{
	int wall_size;
	int sky_size;

	int i = 0;
	
	while (i < WIDTH)
	{
		int k = 0;
		int j = 0;
		wall_size = (HEIGHT/data->rays[i].lenght) * 32;
		if (wall_size > HEIGHT)
		{
			j = wall_size - HEIGHT;
			wall_size = HEIGHT;
		}
		sky_size = (HEIGHT - wall_size) / 2 + ((HEIGHT - wall_size) % 2);
		while (k < sky_size)
		{
			data->screen[i][k] = data->sky_color;
			k++;
		}
		int col;
		if (data->rays[i].type == 'h')
			col = (int)(data->rays[i].pair.first) % 32;
		else
			col = (int)(data->rays[i].pair.second) % 32;
		while (k < (HEIGHT - sky_size))
		{
			if (data->rays[i].angle < M_PI && data->rays[i].type == 'h')
				data->screen[i][k] = data->n_texture[col][((k - sky_size + j / 2) * 32)/(wall_size + j)];
			if ((data->rays[i].angle < M_PI/2 || data->rays[i].angle > M_PI * 3/2) && data->rays[i].type == 'v')
				data->screen[i][k] = data->e_texture[col][((k - sky_size + j / 2) * 32)/(wall_size + j)];
			if (data->rays[i].angle > M_PI && data->rays[i].type == 'h')
				data->screen[i][k] = data->s_texture[col][((k - sky_size + j / 2) * 32)/(wall_size + j)];
			if ((data->rays[i].angle > M_PI/2 && data->rays[i].angle < M_PI * 3/2) && data->rays[i].type == 'v')
				data->screen[i][k] = data->w_texture[col][((k - sky_size + j / 2) * 32)/(wall_size + j)];
			k++;
		}
		while (k < HEIGHT)
		{
			data->screen[i][k] = data->floor_color;
			k++;
		}
		i++;
	}
	print_screen(data);
	
}

void ft_hook(void* param)
{

	double move_distance = 2;

	t_data* data = param;


	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);

	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		data->px_position += cos(data->view_angle) * move_distance;
		data->py_position += sin(data->view_angle) * move_distance;
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		data->px_position -= cos(data->view_angle) * move_distance;
		data->py_position -= sin(data->view_angle) * move_distance;
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		data->px_position += cos(data->view_angle + M_PI/2) * move_distance;
		data->py_position += sin(data->view_angle + M_PI/2) * move_distance;
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		data->px_position -= cos(data->view_angle + M_PI/2) * move_distance;
		data->py_position -= sin(data->view_angle + M_PI/2) * move_distance;
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
	{
		data->view_angle -= 0.1;
		if (data->view_angle < 0)
			data->view_angle = 2 * M_PI;
		ray_maker(data);
	}
	
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
	{
		data->view_angle += 0.1;
		if (data->view_angle > 2 * M_PI)
			data->view_angle = 0;
		ray_maker(data);
	}
}

void free_textures(int32_t **texture, int height)
{
	int i;
	
	i = 0;
	while (i < height)
	{
		free(texture[i]);
		i++;
	}
	free(texture);
}

void free_data(t_data *data)
{
	mlx_delete_image(data->mlx, data->screen_image);
	free_textures(data->n_texture, data->north_texture->height);
	free_textures(data->s_texture, data->south_texture->height);
	free_textures(data->e_texture, data->east_texture->height);
	free_textures(data->w_texture, data->west_texture->height);
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
	//MLX
	data->mlx =  mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	//TEXTURE
	data->north_texture = mlx_load_png("minecra.png");
	data->south_texture = mlx_load_png("TPG.png");
	data->east_texture = mlx_load_png("tom.png");
	data->west_texture = mlx_load_png("syl.png");
	data->n_texture = make_texture(data->north_texture);
	data->s_texture = make_texture(data->south_texture);
	data->e_texture = make_texture(data->east_texture);
	data->w_texture = make_texture(data->west_texture);
	//PLAYER_INFO
	data->px_position = data->start_x;
	data->py_position = data->start_y;
	data->view_angle = NORTH;
	
}


int32_t main(void)
{
	t_data *data;
	
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
