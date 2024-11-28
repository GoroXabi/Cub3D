/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:47:29 by xortega           #+#    #+#             */
/*   Updated: 2024/11/28 16:10:01 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# ifndef WIDTH
#  define WIDTH 1920
# endif

# ifndef HEIGTH
#  define HEIGHT 1080
# endif

typedef struct pair_double_double
{
	double	first;
	double	second;
}					t_pair_d_d;

typedef struct pair_double_pair
{
	double		lenght;
	double		angle;
	char		type;
	t_pair_d_d	pair;
}					t_pair_d_p;

typedef struct texture
{
	int32_t		**texture;
	int			height;
	int			width;
}					t_texture;

typedef struct data
{
	//INIT INFO
	int				**map;
	uint32_t		start_x;
	uint32_t		start_y;
	int				map_width;
	int				map_height;
	uint32_t		sky_color;
	uint32_t		floor_color;
	int				screen[WIDTH][HEIGHT];

	//MLX
	mlx_t			*mlx;
	mlx_image_t		*screen_image;

	//TEXTURES
	mlx_texture_t	*north_texture;
	mlx_texture_t	*south_texture;
	mlx_texture_t	*east_texture;
	mlx_texture_t	*west_texture;
	t_texture		*n_texture;
	t_texture		*s_texture;
	t_texture		*e_texture;
	t_texture		*w_texture;

	//PLAYER_INFO
	int				fov;
	double			speed;
	double			px_p;
	double			py_p;
	double			v_a;
	//RAYS
	t_pair_d_p		rays[WIDTH];
}								t_data;

#endif