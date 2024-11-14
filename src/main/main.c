/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/11/14 17:27:23 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cubed.h"

int32_t make_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

int32_t **make_texture(mlx_texture_t* texture)
{
	uint32_t y;
	uint32_t x;
	uint32_t k;
	int32_t **new_texture;
	
	new_texture = malloc(sizeof(int32_t *) * texture->height);
	for (size_t i = 0; i < texture->height; i++)
	{
		new_texture[i] = malloc(sizeof(int32_t) * texture->width);
	}
	
	for (y = 0; y < texture->height; y++)
	{
		for (x = 0; x < texture->width; x++)
		{
			k = y*texture->width*4 + x*4;
			new_texture[x][y] = make_pixel(texture->pixels[k + 0], texture->pixels[k + 1], texture->pixels[k + 2], texture->pixels[k + 3]);
		}
	}
	return(new_texture);
}

void make_player(uint32_t color, t_data *data)
{
	int k = 15;

	while (k > 0)
	{
		int l = 15;

		while (l > 0)
		{
			mlx_put_pixel(data->player, l, k, color);
			l--;
		}
		
		k--;
	}
}
void refresh_screen(t_data *data)
{
	for (int i = 0; i < WIDTH; i++)
		for (int k = 0; k < HEIGHT; k++)
				mlx_put_pixel(data->screen_image, i,  k, 0);
	mlx_image_to_window(data->mlx, data->screen_image, 0, 0);
}

void print_screen(t_data *data)
{	
	refresh_screen(data);
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

	for (int i = 0; i < WIDTH; i++)
		for (int k = 0; k < HEIGHT; k++)
			data->screen[i][k] = 0;

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
		sky_size = (HEIGHT - wall_size) / 2;
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
				data->screen[i][k] = data->n_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			if ((data->rays[i].angle < M_PI/2 || data->rays[i].angle > M_PI * 3/2) && data->rays[i].type == 'v')
				data->screen[i][k] = data->e_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			if (data->rays[i].angle > M_PI && data->rays[i].type == 'h')
				data->screen[i][k] = data->s_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			if ((data->rays[i].angle > M_PI/2 && data->rays[i].angle < M_PI * 3/2) && data->rays[i].type == 'v')
				data->screen[i][k] = data->w_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			k++;
		}
		while (k < HEIGHT)
		{
			data->screen[i][k] = data->floor_color;
			k++;
		}
		i++;
	}

	printf("print_walls_ok\n");
	print_screen(data);
	
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

void ft_hook(void* param)
{

	int move_distance = 2;

	t_data* data = param;


	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);

	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		data->px_position += cos(data->view_angle) * move_distance;
		data->py_position += sin(data->view_angle) * move_distance;
		
		printf("(%f, %f)\n", data->px_position, data->py_position);
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		data->px_position -= cos(data->view_angle) * move_distance;
		data->py_position -= sin(data->view_angle) * move_distance;
		
		printf("(%f, %f)\n", data->px_position, data->py_position);
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		data->px_position += cos(data->view_angle + M_PI/2) * move_distance;
		data->py_position += sin(data->view_angle + M_PI/2) * move_distance;
		
		printf("(%f, %f)\n", data->px_position, data->py_position);
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		data->px_position -= cos(data->view_angle + M_PI/2) * move_distance;
		data->py_position -= sin(data->view_angle + M_PI/2) * move_distance;
		
		printf("(%f, %f)\n", data->px_position, data->py_position);
		ray_maker(data);
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
	{
		data->view_angle -= 0.1;
		if (data->view_angle < 0)
			data->view_angle = 2 * M_PI;
		
		printf("(%f, %f)\n", data->px_position, data->py_position);
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
	mlx_delete_image(data->mlx, data->player);
	free_textures(data->n_texture, data->north_texture->height);
	free_textures(data->s_texture, data->south_texture->height);
	free_textures(data->e_texture, data->east_texture->height);
	free_textures(data->w_texture, data->west_texture->height);
	mlx_delete_texture(data->north_texture);
	mlx_delete_texture(data->south_texture);
	mlx_delete_texture(data->east_texture);
	mlx_delete_texture(data->west_texture);
	
}

void malloc_data(t_data *data)
{
	data->mlx = malloc(sizeof(mlx_t));
	
	data->screen_image = malloc(sizeof(mlx_image_t));
	data->player = malloc(sizeof(mlx_image_t));
}

void init_data(t_data *data)
{
	//malloc_data(data);
	//printf("malloc_ok\n");
	//INIT INFO
	data->start_x = 75;
	data->start_y = 75;
	data->map_width = 10;
	data->map_height = 10;
	data->sky_color = 0xFFA500FF;
	data->floor_color = 0x800080FF;
	printf("init_info_ok\n");
	//MLX
	data->mlx =  mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->player =  mlx_new_image(data->mlx, data->start_y, data->start_x);
	printf("mlx_ok\n");
	//TEXTURE
	data->north_texture = mlx_load_png("minecra.png");
	data->south_texture = mlx_load_png("TPG.png");
	data->east_texture = mlx_load_png("tom.png");
	data->west_texture = mlx_load_png("syl.png");
	data->n_texture = make_texture(data->north_texture);
	data->s_texture = make_texture(data->south_texture);
	data->e_texture = make_texture(data->east_texture);
	data->w_texture = make_texture(data->west_texture);
	printf("texture_ok\n");
	//PLAYER_INFO
	data->px_position = data->start_x;
	data->py_position = data->start_y;
	data->view_angle = NORTH;
	printf("player_info_ok\n");
	
}


int32_t main(void)
{
	t_data *data;
	
	data = malloc(sizeof(t_data));
	init_data(data);
	printf("init_ok\n");
	mlx_image_to_window(data->mlx, data->player, 32 * 5, 32 * 5);
	mlx_image_to_window(data->mlx, data->screen_image, 0, 0);
	printf("image_to_window_ok\n");
	//print_map(mlx);

	ray_maker(data);
	mlx_loop_hook(data->mlx, ft_hook, data);
	mlx_loop(data->mlx);
	free_data(data);
	mlx_terminate(data->mlx);
	free(data);
	return (EXIT_SUCCESS);
}
