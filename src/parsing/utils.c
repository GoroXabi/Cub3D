/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xabier <xabier@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:41:42 by xortega           #+#    #+#             */
/*   Updated: 2024/12/21 12:53:31 by xabier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubed.h"

int32_t	make_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
	if (r > 255 || g > 255 || b > 255 || a > 255)
		error_matic("color out of scope");
	return (r << 24 | g << 16 | b << 8 | a);
}

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
