/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_general.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guthybarnakoppany <guthybarnakoppany@st    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 13:18:12 by dabdulla          #+#    #+#             */
/*   Updated: 2026/09/02 11:05:56 by guthybarnak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_envs	*ft_lstnew(void *value)
{
	t_envs	*list;

	list = malloc(sizeof(t_envs));
	if (!list)
		return (NULL);
	list->value = value;
	list->next = NULL;
	return (list);
}
void	ft_lstadd_front(t_envs **lst, t_envs *new_list)
{
	if (!lst || !new_list)
		return ;
	new_list->next = *lst;
	*lst = new_list;
}

int	ft_lstsize(t_envs *lst)
{
	int	i;

	if (!lst)
		return (0);
	i = 0;
	while (lst)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

t_envs	*ft_lstlast(t_envs *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
