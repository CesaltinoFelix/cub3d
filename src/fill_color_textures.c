#include "../cub3d.h"

static bool	no_digit(char *str)
{
	int		i;
	bool	found_no_digit;

	i = 0;
	found_no_digit = true;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 1)
			found_no_digit = false;
		i++;
	}
	return (found_no_digit);
}

static int	*copy_into_rgb_array(char **rgb_to_convert, int *rgb)
{
	int		i;

	i = -1;
	while (rgb_to_convert[++i])
	{
		rgb[i] = ft_atoi(rgb_to_convert[i]);
		if (rgb[i] == -1 || no_digit(rgb_to_convert[i]) == true)
		{
			free_tab((void **)rgb_to_convert);
			free(rgb);
			return (0);
		}
	}
	free_tab((void **)rgb_to_convert);
	return (rgb);
}

static int	*set_rgb_colors(char *line)
{
	char	**rgb_to_convert;
	int		*rgb;
	int		count;

	// Skip leading whitespace
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	
	rgb_to_convert = ft_split(line, ',');
	count = 0;
	while (rgb_to_convert[count])
		count++;
	if (count != 3)
	{
		free_tab((void **)rgb_to_convert);
		return (0);
	}
	rgb = malloc(sizeof(int) * 3);
	if (!rgb)
	{
		err_msg(NULL, ERR_MALLOC, 0);
		return (0);
	}
	return (copy_into_rgb_array(rgb_to_convert, rgb));
}

int	fill_color_textures(t_data *data, t_texinfo *textures, char *line, int j)
{
	if (line[j + 1] && ft_isprint(line[j + 1]) && line[j + 1] != ' ' && line[j + 1] != '\t')
		return (err_msg(data->map.path, ERR_FLOOR_CEILING, ERR));
	if (!textures->ceiling && line[j] == 'C')
	{
		textures->ceiling = set_rgb_colors(line + j + 1);
		if (textures->ceiling == 0)
			return (err_msg(data->map.path, ERR_COLOR_CEILING, ERR));
		// Convert RGB to hex
		textures->hex_ceiling = (textures->ceiling[0] << 16) | (textures->ceiling[1] << 8) | textures->ceiling[2];
	}
	else if (!textures->floor && line[j] == 'F')
	{
		textures->floor = set_rgb_colors(line + j + 1);
		if (textures->floor == 0)
			return (err_msg(data->map.path, ERR_COLOR_FLOOR, ERR));
		// Convert RGB to hex
		textures->hex_floor = (textures->floor[0] << 16) | (textures->floor[1] << 8) | textures->floor[2];
	}
	else
		return (err_msg(data->map.path, ERR_FLOOR_CEILING, ERR));
	return (SUCCESS);
}
