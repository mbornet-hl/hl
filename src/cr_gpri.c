/* ============================================================================
 * Copyright (C) 2015 Martial Bornet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   (C) Copyright Martial Bornet, 2015.
 *
 *   Auteur       :     Martial BORNET (MB) - 3 Janvier 2015
 *
 *   Description  :     Globales privees
 *
 *   Fichier      :     cr_gpri.c
 *
 *   @(#)  cr_gpri.c  1.5  15/07/01  MB  
 *
 * ============================================================================
 */

#include "cr_epri.h"

struct cr_global                                  G         = { 0 };

/* Error messages
   ~~~~~~~~~~~~~~ */
char				*cr_err_malloc		= "%s: cannot allocate memory !\n";
