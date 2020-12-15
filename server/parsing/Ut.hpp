/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 02:25:31 by racohen           #+#    #+#             */
/*   Updated: 2020/12/14 21:56:55 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <vector>
#include "Parsing.hpp"


typedef std::string                    stds;
typedef stds::iterator                 iterator;
typedef std::pair<int, stds>           pi;
typedef std::pair<stds, stds>          ps;

bool				valid(stds name, const char **valid_names);
iterator			getBrackets(iterator next, iterator end);
size_t				getMcbs(stds s);
std::vector<stds>	splitWhitespace(stds str);
int					to_int(char const *s, size_t count);

#endif
