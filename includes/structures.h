/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 10:47:29 by xortega           #+#    #+#             */
/*   Updated: 2024/12/18 19:56:44 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# ifndef WIDTH
#  define WIDTH 1920
# endif

# ifndef HEIGHT
#  define HEIGHT 1080
# endif

typedef struct pair_double_double
{
	double	cx;
	double	cy;
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
	int32_t		**tex;
	int			height;
	int			width;
}					t_tex;

typedef struct data
{
	//INIT INFO
	uint32_t		start_x;
	uint32_t		start_y;
	uint32_t		sky_color;
	uint32_t		floor_color;
	int				screen[WIDTH][HEIGHT];

	//MAP INFO
	char			**map;
	char			**ffmap;
	int				map_width;
	int				map_height;
	int				longest;
	int				tall;
	char			**map_str;

	//MLX
	mlx_t			*mlx;
	mlx_image_t		*screen_image;

	//TEXTURES
	char			*north_tex_path;
	char			*south_tex_path;
	char			*east_tex_path;
	char			*west_tex_path;
	char			*sky_tex_path;
	char			*floor_tex_path;
	mlx_texture_t	*north_tex;
	mlx_texture_t	*south_tex;
	mlx_texture_t	*east_tex;
	mlx_texture_t	*west_tex;
	t_tex			*n_tex;
	t_tex			*s_tex;
	t_tex			*e_tex;
	t_tex			*w_tex;

	//PLAYER_INFO
	int				fov;
	double			px_p;
	double			py_p;
	double			v_a;

	//RAYS
	t_pair_d_p		rays[WIDTH];
	size_t			zoom;
	int				wall_size;
	int				sky_size;
}								t_data;

#endif