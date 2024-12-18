/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubed.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:49:38 by xortega           #+#    #+#             */
/*   Updated: 2024/12/18 16:41:32 by xortega          ###   ########.fr       */
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

int32_t		make_pixel(int32_t r, int32_t g, int32_t b, int32_t a);
t_tex		*make_texture(mlx_texture_t *texture);
void		make_screen(t_data *data);
t_pair_d_p	rayo(t_data *data, double angle);
void		ray_maker(t_data *data);
int			get_texture_path(t_data *data);
void		map_checks(t_data *data, int map_size);
void		free_array(void **array);
void		error_matic(char *error);
void		valid_character(t_data *data);
void		get_map_str(t_data *data, const char *map_path);
#endif