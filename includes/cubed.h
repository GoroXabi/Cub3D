/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubed.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:49:38 by xortega           #+#    #+#             */
/*   Updated: 2025/02/04 11:18:34 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBED_H
# define CUBED_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "structures.h"
# include "libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>

#define DISTANCE 32

# define MAX_DEPTH 25

# define PI 3.14159265358979323846

# define PIH 1.57079632679489661923

# define PI3H 4.71238898038468985769

# define SOUTH PI3H
# define NORTH PIH
# define EAST 0
# define WEST PI

# define ANGLE_TO_RADIAN 0.0174533

# define HRED	"\033[91m"
# define HBLU	"\033[94m"
# define HGRE	"\033[92m"
# define HMAG	"\033[95m"
# define RST	"\033[0m"

bool	    check_rgb(uint32_t *rgba, int32_t r, int32_t g, int32_t b);
t_tex		*make_texture(mlx_texture_t *texture);
void		make_screen(t_data *data);
t_pair_d_p	rayo(t_data *data, double angle);
void		ray_maker(t_data *data);
void		get_textures(t_data *data);
void		map_checks(t_data *data);
void		valid_character(t_data *data);
void		get_map_str(t_data *data, const char *map_path);

void		upd_p(t_data *data, double dx, double dy);
void		check_movements(t_data *data);
void		check_cam(t_data *data);
void		art_attack(t_data *data, int col);
void		print_walls(t_data *data, int col);
void		make_screen(t_data *data);

t_pair_d_d	rayo_h(t_data *data, double x, double y, double ra);
t_pair_d_d	rayo_v(t_data *data, double x, double y, double ra);
void		resize_n(t_data *data, int col, int start);
void		resize_s(t_data *data, int col, int start);
void		resize_e(t_data *data, int col, int start);
void		resize_w(t_data *data, int col, int start);

void		is_cub_file(const char *file_name);
void		map_mutate(t_data *data);
void		import_textures(t_data *data);
void		check_lines(t_data *data, char *trimed);
void		longest_line(t_data *data);

void		free_data(t_data *data);
void		free_array(void **array);
void		free_texture(t_tex *texture);
void        free_textures_path(t_data *data);
void        free_mlx_textures(t_data *data);
void        free_our_textures(t_data *data);
void		free_array(void **array);
void		error_matic(char *error, t_data *data, int scope);


#endif