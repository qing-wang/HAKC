
#include <linux/linkage.h>
#include <asm/assembler.h>

	.arch_extension pauth

/*
 * void *hakc_auth_data_ptr(const void *address, pac_salt_t modifier)
 *
 * Safety guard: only call autia when modifier is a plausible PAC_MODIFIER.
 * All valid PAC_MODIFIERs = (claque_id << 16) | HAKC_MASK_COLOR(color)
 * have a maximum value of 0x00FF8000, so bits[63:24] must all be zero.
 * Callers may inadvertently pass access_tok (e.g. 0x72000000) as the
 * modifier when the PMC pass transforms check_hakc_access; the range
 * check below ensures autia is never invoked with such an invalid value.
 */
SYM_FUNC_START(hakc_auth_data_ptr)
	cbz	x1, 1f			/* modifier == 0 → nothing to auth */
	lsr	x9, x1, #24
	cbnz	x9, 1f			/* modifier > 0x00FF8000 → invalid */
	autia	x0, x1
1:	ret
SYM_FUNC_END(hakc_auth_data_ptr)

/*
 * void *hakc_auth_code_ptr(const void *address, pac_salt_t modifier)
 */
SYM_FUNC_START(hakc_auth_code_ptr)
	cbz	x1, 1f
	lsr	x9, x1, #24
	cbnz	x9, 1f
	autia	x0, x1
1:	ret
SYM_FUNC_END(hakc_auth_code_ptr)
