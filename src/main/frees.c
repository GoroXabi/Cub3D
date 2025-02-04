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

void	free_texture(t_tex *texture)
{
	int	i;

	i = 0;
	if (!texture)
		return ;
	while (i < texture->height)
	{
		free(texture->tex[i]);
		i++;
	}
	free(texture->tex);
	free(texture);
}

void free_our_textures(t_data *data)
{
	if (data->n_tex)
		free_texture(data->n_tex);
	if (data->s_tex)
		free_texture(data->s_tex);
	if (data->e_tex)
		free_texture(data->e_tex);
	if (data->w_tex)
		free_texture(data->w_tex);
}

void free_mlx_textures(t_data *data)
{
	if (data->north_tex)
		mlx_delete_texture(data->north_tex);
	if (data->south_tex)
		mlx_delete_texture(data->south_tex);
	if (data->east_tex)
		mlx_delete_texture(data->east_tex);
	if (data->west_tex)
		mlx_delete_texture(data->west_tex);
}

void	free_data(t_data *data)
{
	mlx_delete_image(data->mlx, data->screen_image);
	mlx_terminate(data->mlx);
	free_textures_path(data);
	free_our_textures(data);
	free_mlx_textures(data);
	free_array((void **)data->map_str);
	free_array((void **)data->map);
	free_array((void **)data->ffmap);
	free(data);
}
