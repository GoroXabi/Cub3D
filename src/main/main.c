/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/12/19 14:36:34 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	free_textures(t_tex *texture)
{
	int	i;

	i = 0;
	while (i < texture->height)
	{
		free(texture->tex[i]);
		i++;
	}
	free(texture);
}

void	free_data(t_data *data)
{
	mlx_delete_image(data->mlx, data->screen_image);
	free_textures(data->n_tex);
	free_textures(data->s_tex);
	free_textures(data->e_tex);
	free_textures(data->w_tex);
	mlx_delete_texture(data->north_tex);
	mlx_delete_texture(data->south_tex);
	mlx_delete_texture(data->east_tex);
	mlx_delete_texture(data->west_tex);
}

void	init_data(t_data *data)
{
	data->fov = 60;
	data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->north_tex_path = NULL;
	data->south_tex_path = NULL;
	data->east_tex_path = NULL;
	data->west_tex_path = NULL;
}

void	ft_hook(void *param)
{
	t_data	*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	check_movements(data);
	check_cam(data);
	ray_maker(data);
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc < 2)
		return (0);
	is_cub_file(argv[1]);
	data = malloc(sizeof(t_data));
	init_data(data);
	get_map_str(data, argv[1]);
	map_checks(data);
	mlx_image_to_window(data->mlx, data->screen_image, 0, 0);
	ray_maker(data);
	mlx_loop_hook(data->mlx, ft_hook, data);
	mlx_loop(data->mlx);
	free_data(data);
	mlx_terminate(data->mlx);
	free(data);
	return (0);
}
