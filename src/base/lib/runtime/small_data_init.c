/*
 * Copyright (C) 2007, The EROS Group, LLC.
 *
 * This file is part of the Coyotos Operating System.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/** @file Small Space Data initialization */

#include <stddef.h>
#include <inttypes.h>
#include <coyotos/coytypes.h>
#include <coyotos/runtime.h>
#include <idl/coyotos/AddressSpace.h>
#include <idl/coyotos/Range.h>
#include <idl/coyotos/SpaceBank.h>

#define CAP_NULL	CAP_REG(CAPREG_NULL)
#define CAP_REPLYEPT	CAP_REG(CAPREG_REPLYEPT)
#define CAP_SPACEBANK	CAP_REG(CAPREG_SPACEBANK)
#define CAP_NEWADDR	CAP_REG(CAPREG_APP + 0) /* new address space */
#define CAP_NEWPAGE	CAP_REG(CAPREG_APP + 1) /* available for new page */
#define CAP_OLDADDR	CAP_REG(CAPREG_APP + 2) /* old address space */
#define CAP_OLDPAGE	CAP_REG(CAPREG_APP + 3) /* old page */
#define CAP_RETURN	CAP_REG(CAPREG_RETURN) /* reply capability */

void
__small_data_init(uintptr_t data, uintptr_t end)
{
  size_t cur;
  IDL_Environment _IDL_E = {
    .replyCap = CAP_REPLYEPT,
    .epID = 0ULL
  };
  size_t first_slot = data / COYOTOS_PAGE_SIZE;
  size_t last_slot = (end + COYOTOS_PAGE_SIZE - 1) / COYOTOS_PAGE_SIZE;

  for (cur = first_slot; cur < last_slot; cur++) {
    if (coyotos_AddressSpace_getSlot(CAP_OLDADDR, cur, CAP_OLDPAGE, &_IDL_E) ||
	coyotos_SpaceBank_alloc(CAP_SPACEBANK,
				coyotos_Range_obType_otPage,
				coyotos_Range_obType_otInvalid,
				coyotos_Range_obType_otInvalid,
				CAP_NEWPAGE,
				CAP_NULL,
				CAP_NULL,
				&_IDL_E) ||
	coyotos_AddressSpace_copyFrom(CAP_NEWPAGE, CAP_OLDPAGE, 
				      CAP_NEWPAGE, &_IDL_E) ||
	coyotos_AddressSpace_setSlot(CAP_NEWADDR, cur, CAP_NEWPAGE, &_IDL_E))
      goto fail;
  }
  return;

 fail:
  coyotos_SpaceBank_destroyBankAndReturn(CAP_SPACEBANK, CAP_RETURN,
                                         RC_coyotos_SpaceBank_LimitReached,
                                         &_IDL_E);
  return;
}
