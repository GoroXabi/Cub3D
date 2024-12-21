/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/12/21 13:22:07 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	free_array(void **array)
{
	int	i;

	i = -1;
	while (array && array[++i])
	{
		free(array[i]);
		array[i] = NULL;
	}
	if (array)
		free(array);
}

void	free_textures(t_tex *texture)
{
	int	i;

	i = 0;
	while (i < texture->height)
	{
		free(texture->tex[i]);
		i++;
	}
	free(texture->tex);
	free(texture);
}

void	free_data(t_data *data)
{
	mlx_delete_image(data->mlx, data->screen_image);
	mlx_terminate(data->mlx);
	free(data->n_tex_path);
	free(data->s_tex_path);
	free(data->e_tex_path);
	free(data->w_tex_path);
	free(data->sky_path);
	free(data->floor_path);
	free_textures(data->n_tex);
	free_textures(data->s_tex);
	free_textures(data->e_tex);
	free_textures(data->w_tex);
	mlx_delete_texture(data->north_tex);
	mlx_delete_texture(data->south_tex);
	mlx_delete_texture(data->east_tex);
	mlx_delete_texture(data->west_tex);
	free_array((void **)data->map_str);
	free_array((void **)data->map);
	free_array((void **)data->ffmap);
	free(data);
}
