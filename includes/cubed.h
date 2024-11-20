/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubed.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:49:38 by xortega           #+#    #+#             */
/*   Updated: 2024/11/20 12:03:15 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBED_H
# define CUBED_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "structures.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <math.h>

# define MAP_SIZE 10

# define MAX_DEPTH round(sqrt((10 * 10 + 10 * 10)))

# define PI 3.14159265358979323846

# define PIH 1.57079632679489661923

# define PI3H 4.71238898038468985769

# define SOUTH PI3H
# define NORTH PIH
# define EAST 0
# define WEST PI

# define FOV 60

# define ANGLE_TO_RADIAN 0.0174533

# define HRED	"\033[91m"
# define HBLU	"\033[94m"
# define HGRE	"\033[92m"
# define HMAG	"\033[95m"
# define RST	"\033[0m"

t_texture	*make_texture(mlx_texture_t *texture);
void		print_walls(t_data *data);
t_pair_d_p	rayo(t_data *data, double angle);
void		ray_maker(t_data *data);
#endif