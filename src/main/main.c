/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:51:23 by xortega           #+#    #+#             */
/*   Updated: 2024/12/21 13:10:22 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	init_data(t_data *data)
{
	data->fov = 60;
	data->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", false);
	data->screen_image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->n_tex_path = NULL;
	data->s_tex_path = NULL;
	data->e_tex_path = NULL;
	data->w_tex_path = NULL;
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
	get_textures(data);
	map_mutate(data);
	map_checks(data);
	mlx_image_to_window(data->mlx, data->screen_image, 0, 0);
	ray_maker(data);
	mlx_loop_hook(data->mlx, ft_hook, data);
	//mlx_loop(data->mlx);
	free_data(data);
	return (0);
}
