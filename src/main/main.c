/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/11/12 13:16:50 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cubed.h"


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define WIDTH 720
#define HEIGHT 720
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

uint32_t sky_color = 0xFFA500FF;

uint32_t floor_color = 0x800080FF;

#define HRED	"\033[91m"
#define HBLU	"\033[94m"
#define HGRE	"\033[92m"
#define HMAG	"\033[95m"
#define RST		"\033[0m"

#define EAST 0
#define NORTH M_PI/2 
#define WEST M_PI
#define SOUTH M_PI * 3/2

#define MAX_DEPTH round(sqrt((MAP_SIZE * MAP_SIZE + MAP_SIZE * MAP_SIZE)))

#define	ANGLE_TO_RADIAN 0.0174533

#define FOV 60

typedef struct pair_double_double {
    double first;
    double second;
}				t_pair_d_d;

typedef struct pair_double_pair {
    double		lenght;
	double		angle;
	char		type;
    t_pair_d_d	pair;
}				t_pair_d_p;

double	px;

double	py;

double	view_angle = NORTH;

t_pair_d_p rays[WIDTH];

int MAP_SIZE = 10;

static mlx_image_t* map_image;

static mlx_texture_t* north_texture;
static mlx_texture_t* south_texture;
static mlx_texture_t* east_texture;
static mlx_texture_t* west_texture;

static mlx_image_t* screen_image;

static mlx_image_t* player;

static mlx_image_t* player_direction;

mlx_t* mlx;

// MAP[y][x]

int screen[WIDTH][HEIGHT];

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

int32_t n_texture[32][32];

int32_t s_texture[32][32];

int32_t e_texture[32][32];

int32_t w_texture[32][32];

int32_t make_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void make_texture(mlx_texture_t* texture, int32_t m_texture[32][32])
{
	int y;
	int x;
	int k;
	
	for (y = 0; y < 32; y++)
	{
		for (x = 0; x < 32; x++)
		{
			k = y*32*4 + x*4;
			m_texture[x][y] = make_pixel(texture->pixels[k + 0], texture->pixels[k + 1], texture->pixels[k + 2], texture->pixels[k + 3]);
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
		int col = MAP_SIZE - 1;
		
		while(col >= 0)
		{
			if (MAP[col][col] == 1)
			{
				int32_t x = 0;
				
				while (x < 31)
				{
					int32_t y = 0;
					while (y < 31)
					{
						mlx_put_pixel(map_image, (col * 32) + x, (col * 32) + y, 0x00FF00FF);
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
						mlx_put_pixel(map_image, (col * 32) + x, (col * 32) + y, 0);
						y++;
					}
					x++;
				}	
			}
			col--;
		}

		col--;
	}		
	mlx_image_to_window(mlx, map_image, 0, 0);
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

t_pair_d_p  rayo(int32_t x, int32_t y, double angle)
{
	t_pair_d_d h;
	t_pair_d_d v;

	t_pair_d_p ret;

	if (angle < 0)
		angle += 2*M_PI;
	if (angle > 2*M_PI)
		angle -= 2*M_PI;
		
	ret.angle = angle;
	h = rayo_h(x, y, angle);
	v = rayo_v(x, y, angle);

	double dh;
	double dv;

	dh = sqrt((h.first - x) * (h.first - x) + (h.second -y) * (h.second -y));
	dv = sqrt((v.first - x) * (v.first - x) + (v.second -y) * (v.second -y));

	if (dh <= dv)
	{
		ret.lenght = fabs(dh * cos(angle - view_angle));
		ret.pair = h;
		ret.type = 'h';
		return(ret);
	}
	else
	{
		ret.lenght = fabs(dv * cos(angle - view_angle));
		ret.pair = v;
		ret.type = 'v';
		return(ret);
	}
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
			mlx_put_pixel(screen_image, i,  k, screen[i][k]);
		}
	}
}

void print_walls(void)
{

	for (int i = 0; i < WIDTH; i++)
		for (int k = 0; k < HEIGHT; k++)
			screen[i][k] = 0;

	int wall_size;
	int sky_size;


	int i = 0;
	
	while (i < WIDTH)
	{
		int k = 0;
		int j = 0;
		wall_size = (HEIGHT/rays[i].lenght) * 32;
		if (wall_size > HEIGHT)
		{
			j = wall_size - HEIGHT;
			wall_size = HEIGHT;
		}
		sky_size = (HEIGHT - wall_size) / 2;
		while (k < sky_size)
		{
			screen[i][k] = sky_color;
			k++;
		}
		int col;

		if (rays[i].type == 'h')
			col = (int)(rays[i].pair.first) % 32;
		else
			col = (int)(rays[i].pair.second) % 32;
		while (k < (HEIGHT - sky_size))
		{
			if (rays[i].angle < M_PI && rays[i].type == 'h')	
				screen[i][k] = n_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			if ((rays[i].angle < M_PI/2 || rays[i].angle > M_PI * 3/2) && rays[i].type == 'v')
				screen[i][k] = e_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			if (rays[i].angle > M_PI && rays[i].type == 'h')
				screen[i][k] = s_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			if ((rays[i].angle > M_PI/2 && rays[i].angle < M_PI * 3/2) && rays[i].type == 'v')
				screen[i][k] = w_texture[col][((k - sky_size + j / 2) *32)/(wall_size + j)];
			k++;
		}
		while (k < HEIGHT)
		{
			screen[i][k] = floor_color;
			k++;
		}
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

void print()
{
	int	i = 0;

	double fan;


	fan = view_angle - (FOV/2 * ANGLE_TO_RADIAN);

	while (i < WIDTH)
	{
		rays[i] = rayo(player->instances->x, player->instances->y, fan);
		fan += ((double)FOV / (double)WIDTH) * (double)ANGLE_TO_RADIAN;
		i++;
	}
	
	print_walls();
}

void ft_hook(void* param)
{

	int move_distance = 2;

	mlx_t* mlx = param;


	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);

	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		double deltaX = cos(view_angle) * move_distance;
		double deltaY = sin(view_angle) * move_distance;

		px += deltaX;
		py += deltaY;
		print();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		double deltaX = cos(view_angle) * move_distance;
		double deltaY = sin(view_angle) * move_distance;

		px -= deltaX;
		py -= deltaY;
		print();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		double deltaX = cos(view_angle + M_PI/2) * move_distance;
		double deltaY = sin(view_angle + M_PI/2) * move_distance;

		px += deltaX;
		py += deltaY;
		print();
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		double deltaX = cos(view_angle + M_PI/2) * move_distance;
		double deltaY = sin(view_angle + M_PI/2) * move_distance;

		px -= deltaX;
		py -= deltaY;
		print();
	}
	
	player->instances->x = round(px);
	player->instances->y = round(py);

	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
	{
		view_angle -= 0.1;
		if (view_angle < 0)
			view_angle = 2*M_PI;
		print();
	}
	
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
	{
		view_angle += 0.1;
		if (view_angle > 2*M_PI)
			view_angle = 0;
		print();
	}
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
	mlx_image_to_window(mlx, screen_image, 0, 0);
	//print_map(mlx);

	px = player->instances[0].x;
	py = player->instances[0].y;
	
	north_texture = mlx_load_png("minecra.png");
	south_texture = mlx_load_png("TPG.png");
	east_texture = mlx_load_png("tom.png");
	west_texture = mlx_load_png("syl.png");
	make_texture(north_texture, n_texture);
	make_texture(south_texture, s_texture);
	make_texture(east_texture, e_texture);
	make_texture(west_texture, w_texture);
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}