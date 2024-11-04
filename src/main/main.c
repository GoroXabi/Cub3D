/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/11/04 11:30:41 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cubed.h"


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define WIDTH 1920
#define HEIGHT 1080
#define M_PI   3.14159265358979323846

uint32_t colores[] = {
    0xFF0000FF, // Rojo (ARGB)
    0x00FF00FF, // Verde (ARGB)
    0x0000FFFF, // Azul (ARGB)
    0xFFFF00FF, // Amarillo (ARGB)
    0xFF00FFFF, // Magenta (ARGB)
    0x00FFFFFF, // Cian (ARGB)
    0xFFFFFFFF, // Blanco (ARGB)
    0x000000FF, // Negro (ARGB)
    0x808080FF, // Gris (ARGB)
    0xFFA500FF, // Naranja (ARGB)
    0x800080FF, // Púrpura (ARGB)
    0xA52A2AFF  // Marrón (ARGB)
};

#define HRED	"\033[91m"
#define HBLU	"\033[94m"
#define HGRE	"\033[92m"
#define HMAG	"\033[95m"
#define RST		"\033[0m"

#define EAST 0
#define NORTH M_PI/2 
#define WEST M_PI
#define SOUTH M_PI * 3/2

#define	RADIAN_TO_ANGLE 0.0174533

#define FOV 90


double	px;

double	py;

double	view_angle = NORTH;

double rays[FOV/2];

int MAP_SIZE = 10;

static mlx_image_t* map_image;

static mlx_image_t* screen_image;

static mlx_image_t* ray_image;

static mlx_image_t* player;

static mlx_image_t* pixel;

static mlx_image_t* player_direction;

mlx_t* mlx;

// MAP[y][x]

int screen[WIDTH][HEIGHT];

int MAP[10][10] = {

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

};

typedef struct pair_double_double {
    double first;
    double second;
}				t_pair_d_d;



void dibujar_linea(int x0, int y0, int x1, int y1)
{

	if (!ray_image)
		return ;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
	{
		mlx_put_pixel(ray_image, x0, y0, 0xFFFFFFFF); // Dibuja el pixel
        if (x0 == x1 && y0 == y1) break;
        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void make_player(uint32_t color)
{
	int k = 15;

	while (k > 0)
	{
		int l = 15;

		while (l > 0)
		{
			mlx_put_pixel(player, l, k, color);
			l--;
		}
		
		k--;
	}
}
void make_direction(void)
{
	int k = 7;

	while (k > 0)
	{
		int l = 7;

		while (l > 0)
		{
			mlx_put_pixel(player_direction, l, k, 0x00FFFFFF);
			l--;
		}
		
		k--;
	}
}

void	print_map(mlx_t *mlx)
{
	
	if (!(map_image = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		exit(EXIT_FAILURE);
	}

	int col = MAP_SIZE - 1;

	//make_tail(0x00FF00FF);
	while(col >= 0)
	{
		int row = MAP_SIZE - 1;
		
		while(row >= 0)
		{
			if (MAP[col][row] == 1)
			{
				int32_t x = 0;
				
				while (x < 31)
				{
					int32_t y = 0;
					while (y < 31)
					{
						mlx_put_pixel(map_image, (row * 32) + x, (col * 32) + y, 0x00FF00FF);
						y++;
					}
					x++;
				}	
			}
			else
			{
				int32_t x = 0;
				
				while (x < 31)
				{
					int32_t y = 0;
					while (y < 31)
					{
						mlx_put_pixel(map_image, (row * 32) + x, (col * 32) + y, 0);
						y++;
					}
					x++;
				}	
			}
			row--;
		}

		col--;
	}		
	mlx_image_to_window(mlx, map_image, 0, 0);
}

void    ft_put_pixel(int x, int y)
{
    pixel = mlx_new_image(mlx, 8, 8);
	int k = 7;
	while (k > 0)
	{
		int l = 7;

		while (l > 0)
		{
			mlx_put_pixel(pixel, l, k, 0x800080FF);
			l--;
		}
		
		k--;
	}
    mlx_image_to_window(mlx, pixel, x, y);
}
void draw_v_line(int x, int max_y)
{
    int y = -1;
    pixel = mlx_new_image(mlx, 1, max_y);
    while (++y < max_y)
    {
        mlx_put_pixel(pixel, 0, y, 0xFFFFFF);
    }
    mlx_image_to_window(mlx, pixel, x, 0);

}
void    draw_h_line(mlx_t *mlx, int y, int max_x)
{
    int x = -1;
    pixel = mlx_new_image(mlx, max_x, 1);
    while (++x < max_x)
    {
        mlx_put_pixel(pixel, x, 0, 0xFFFFFF);
    }
    mlx_image_to_window(mlx, pixel, 0, y);
}

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
		depth = 10;
		target_x = 0;
		target_y = 0;
	}

	while (depth < 10)
	{
		target_x = (int)rx / 32;
		target_y = (int)ry / 32;
		if (target_x >= 0 && target_x <= MAP_SIZE
			&& target_y >= 0 && target_y <= MAP_SIZE
				&& MAP[target_y][target_x] == 1)
			depth = 10;
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
	/*if (target_x > MAP_SIZE * 32 || target_y > MAP_SIZE * 32)
	{
		ret.first = HEIGHT * 2;
		ret.second = HEIGHT * 2;
	}*/
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
		rx = (y - ry) *  aTan + x;
		yo = 32;
		xo = -yo * aTan;
	}

	if (ra == 0 || ra == M_PI)
	{
		rx = x;
		ry = y;
		depth = 10;
		target_x = 0;
		target_y = 0;
	}

	while (depth < 10)
	{
		target_x = (int)rx / 32;
		target_y = (int)ry / 32;
		if (target_x >= 0 && target_x <= MAP_SIZE
			&& target_y >= 0 && target_y <= MAP_SIZE
				&& MAP[target_y][target_x] == 1)
			depth = 10;
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
	/*if (target_x > MAP_SIZE * 32 || target_y > MAP_SIZE * 32)
	{
		ret.first = HEIGHT * 2;
		ret.second = HEIGHT * 2;
	}*/
	return(ret);
}

double  rayo(int32_t x, int32_t y, double angle)
{
	t_pair_d_d h;
	t_pair_d_d v;

	if (angle < 0)
		angle += 2*M_PI;
	if (angle > 2*M_PI)
		angle -= 2*M_PI;
		
	h = rayo_h(x, y, angle);
	v = rayo_v(x, y, angle);

	double dh;
	double dv;

	dh = sqrt((h.first - x) * (h.first - x) + (h.second -y) * (h.second -y));
	dv = sqrt((v.first - x) * (v.first - x) + (v.second -y) * (v.second -y));

	if (dh <= dv)
		return(fabs(dh * cos(angle - view_angle)));
	else
		return(fabs(dv * cos(angle - view_angle)));
}

void refresh_screen(void)
{
	for (int i = 0; i < WIDTH; i++)
		for (int k = 0; k < HEIGHT; k++)
				mlx_put_pixel(screen_image, i,  k, 0);
	mlx_image_to_window(mlx, screen_image, 0, 0);
}

void print_screen(void)
{	
	refresh_screen();
	for (int i = 0; i < WIDTH; i++)
	{

		for (int k = 0; k < HEIGHT; k++)
		{
			if (screen[i][k] == 1)
				mlx_put_pixel(screen_image, i,  k, 0x00FF00FF);
		}
	}
	mlx_image_to_window(mlx, screen_image, 0, 0);
}

void print_walls(void)
{

	for (int i = 0; i < WIDTH; i++)
		for (int k = 0; k < HEIGHT; k++)
			screen[i][k] = 0;

	int ray_width = WIDTH / (FOV / 2);

	int wall_size;
	int sky_size;


	int i = 0;
	int j = 0;
	int k = 0;
	
	while (i < WIDTH)
	{
		wall_size = (int)HEIGHT/rays[j];
		if (wall_size > HEIGHT)
			wall_size = HEIGHT;
		sky_size = (HEIGHT - wall_size) / 2;
		k = sky_size;
		/*
		printf("i : [%d]\n", i);
		printf("k : [%d]\n", k);
		printf("j : [%d]\n", j);
		printf("ray : [%d]\n", j);
		printf("ray_size : [%f]\n", rays[j]);
		printf("wall_size : [%d]\n", wall_size);
		printf("ray_width : [%d]\n", ray_width);
		printf("sky_size : [%d]\n", sky_size);
		*/
		while (k < (HEIGHT - sky_size))
		{
			screen[i][k] = 1;
			k++;
		}
		if (i && i % ray_width == 0 && j < (FOV/2) - 1)
			j++;
		i++;
	}

	print_screen();
	
}

int32_t round_if_small(double value) {
	if (value < 1 && value > 0.5)
		return (1);
	if (value < 0.5 && value > 0)
		return (0);
	return((int32_t)round(value));
}

void ft_hook(void* param)
{

	int move_distance = 1;

	mlx_t* mlx = param;


	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);


	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		double deltaX = cos(view_angle) * move_distance;
		double deltaY = sin(view_angle) * move_distance;

		px += deltaX;
		py += deltaY;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		double deltaX = cos(view_angle) * move_distance;
		double deltaY = sin(view_angle) * move_distance;

		px -= deltaX;
		py -= deltaY;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		double deltaX = cos(view_angle + M_PI/2) * move_distance;
		double deltaY = sin(view_angle + M_PI/2) * move_distance;

		px += deltaX;
		py += deltaY;
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		double deltaX = cos(view_angle + M_PI/2) * move_distance;
		double deltaY = sin(view_angle + M_PI/2) * move_distance;

		px -= deltaX;
		py -= deltaY;
	}
	
	player->instances->x = round(px);
	player->instances->y = round(py);

	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		view_angle -= 0.1;
		if (view_angle < 0)
			view_angle = 2*M_PI;	
	}
	
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	{
		view_angle += 0.1;
		if (view_angle > 2*M_PI)
			view_angle = 0;
	}
	
	printf( HBLU "view_angle : [%f]\n" RST, view_angle * (1 / RADIAN_TO_ANGLE));
	printf( HRED "player_pos : (%d, %d)\n" RST, player->instances[0].x, player->instances[0].y);

	int	i = 0;

	double fan;

	fan = view_angle - FOV/2 * RADIAN_TO_ANGLE;

	while (i < FOV/2)
	{
		rays[i] = rayo(player->instances->x, player->instances->y, fan);
		i++;
		fan += RADIAN_TO_ANGLE * 2;
	}
	
	print_walls();

}

int32_t main(void)
{
	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
		

	if (!(screen_image = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	
	if (!(player = mlx_new_image(mlx, 16, 16)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	make_player(0xFF0000FF * 0);
	mlx_image_to_window(mlx, player, 32 * 5, 32 * 5);
	//print_map(mlx);

	px = player->instances[0].x;
	py = player->instances[0].y;
	

	mlx_loop_hook(mlx, ft_hook, mlx);
	//ft_hook(mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}