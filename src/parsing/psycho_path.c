/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   psycho_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xortega <xortega@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 12:08:50 by andefern          #+#    #+#             */
/*   Updated: 2024/12/09 14:47:24 by xortega          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

void	longest_line(t_data *data)
{
	int		l;
	size_t	v;

	l = 0;
	v = ft_strlen(data->map[l]);
	while (data->map[l])
	{
		if (ft_strlen(data->map[l]) > v)
			v = ft_strlen(data->map[l]);
		l++;
	}
	data->longest = v;
}

void	new_line_eraser(t_data *data)
{
	int		new_tall;
	int		y;
	char	**new_map;

	new_tall = data->tall - 1;
	while (new_tall > 0 && data->map[new_tall - 1][0] == '\n')
		new_tall--;
	new_map = malloc(sizeof(char *) * (new_tall + 1));
	y = 0;
	while (y < new_tall)
	{
		new_map[y] = ft_strdup(data->map[y]);
		y++;
	}
	data->tall = y;
	new_map[y] = NULL;
	free_array((void **)data->map);
	data->map = new_map;
}

void	erase_new_line(t_data *data)
{
	char	*line;
	int		k;

	k = 0;
	while (data->map[k])
	{
		line = ft_strtrim(data->map[k], "\n");
		free(data->map[k]);
		data->map[k] = line;
		k++;
	}
	data->map[k] = NULL;
}

void	map_shaper(t_data *data, int map_size, int line)
{
	int	i;

	i = 0;
	//map_size son el total de lineas del archivo, line donde terminan las lineas vacias despues de las texturas
	data->tall = map_size - line;
	data->map = malloc(sizeof(char *) * (data->tall + 1));
	while (data->map_str[line])
	{
		data->map[i] = ft_strdup(data->map_str[line]);
		i++;
		line++;
	}
	//hago un dup en vez de igualarlas para luego liberar memoria mas facil
	data->map[i] = NULL;
}

void	line_filler(t_data *data)
{
	int		i;
	size_t	k;
	char	*line;

	i = 0;
	while (data->map[i])
	{
		if (ft_strlen(data->map[i]) < (size_t)data->longest)
		{
			line = malloc(sizeof(char) * data->longest);
			line[0] = '\0';
			ft_strlcat(line, data->map[i], ft_strlen(data->map[i]) + 1);
			k = ft_strlen(line);
			while (k < (size_t)data->longest)
			{
				line[k] = '0';
				k++;
			}
			line[k] = '\0';
			free(data->map[i]);
			data->map[i] = line;
		}
		i++;
	}
}

void	spaces_replacer(t_data *data)
{
	int	i;
	int	k;

	i = 0;
	while (data->map[i])
	{
		k = 0;
		while (data->map[i][k])
		{
			if (data->map[i][k] == ' ')
				data->map[i][k] = '0';
			k++;
		}
		i++;
	}
}

void	flud_fil(t_data *data, int y, int x)
{
	if (data->map[y][x] == '1')
		return ;
	if (x == 0 || y == 0
		|| y == data->tall - 1 || x == data->longest - 1)
		error_matic("Invalid map\n");
	data->map[y][x] = '1';
	flud_fil(data, y - 1, x);
	flud_fil(data, y + 1, x);
	flud_fil(data, y, x + 1);
	flud_fil(data, y, x - 1);
}

int	get_texture_path(t_data *data)
{
	int		line;
	char	*trimed;

	line = 0;
	//no necesitais trimear si no quereis pero tendriais que aseguraros no haya tabulaciones ni espacios al principio de todo el archivo
	while (data->map_str[line])
	{
		trimed = ft_strtrim(data->map_str[line], "\t ");
		if (trimed[0] == 'N' && trimed[1] == 'O')
			data->north_texture_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'S' && trimed[1] == 'O')
			data->south_texture_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'E' && trimed[1] == 'A')
			data->east_texture_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'W' && trimed[1] == 'E')
			data->west_texture_path = ft_strtrim(trimed + 2, " \t\n");
		else if (trimed[0] == 'F')
			data->floor_texture_path = ft_strtrim(trimed + 1, " \t\n");
		else if (trimed[0] == 'C')
			data->sky_texture_path = ft_strtrim(trimed + 1, " \t\n");
		free(trimed);
		line++;
		if (data->north_texture_path && data->south_texture_path
			&& data->east_texture_path && data->west_texture_path
				&& data->floor_texture_path && data->sky_texture_path)
			break ;
	}
	return(line);
}
void print_map(t_data *data, char *color)
{
	printf("%s", color);
	for (int i = 0; data->map[i]; i++)
		printf("%s\n", data->map[i]);
	printf("---------------------\n"RST);
}
void	no_se(t_data *data, int map_size)
{
	int line;

	line = get_texture_path(data);
	//line es en que linea del archivo terminan los paths de las texturas y los colores
	
	printf("%d %d \n", map_size, line);
	if (line + 1 == map_size)
		error_matic("Missing parts in the .cub");

	while (data->map_str[line] && data->map_str[line][0] == '\n')
		line++;

	map_shaper(data, map_size, line);
	//inicializa el char**map que es el mapa de verdad, la parte de los numeros
	
	longest_line(data);
	//mido la linea mas larga comparandolas todas
	
	new_line_eraser(data);
	//me cargo los saltos de linea de despues del mapa, no tendriais que hacerlo si no lo admitis

	erase_new_line(data);
	//quito el salto de linea del final de las lineas por que me da cosica

	//de aqui para abajo es mi psicopatia, si lo haceis hasta aqui tendriais que tener el mapa bien y las texturas tambien
	print_map(data, HBLU);
	spaces_replacer(data);
	print_map(data, HGRE);
	line_filler(data);
	print_map(data, HRED);
	valid_character(data);

	
	printf("p_cords : (%f , %f) \n", data->px_p, data->py_p);
	printf("[%d, %d]\n", data->tall, data->longest);
	flud_fil(data, data->py_p / 32, data->px_p / 32);
	printf(HMAG"n_path : [%s]\n", data->north_texture_path);
	printf("s_path : [%s]\n", data->south_texture_path);
	printf("e_path : [%s]\n", data->east_texture_path);
	printf("w_path : [%s]\n", data->west_texture_path);
	printf("f_path : [%s]\n", data->floor_texture_path);
	printf("c_path : [%s]\n"RST, data->sky_texture_path);
	printf("line : [%d]\n", line);
	printf(HBLU"longest_line : [%d]\n", data->longest);
	printf("map_tall : [%d]\n", data->tall);
}
