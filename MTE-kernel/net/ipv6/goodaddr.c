/*
 * goodaddr.ko — prints the address of a valid struct xt_match for use
 * as GOOD_KADDR_1 in poc_cysec.c.
 *
 * The dummy_match has me = NULL (built-in style), so module_put(NULL)
 * is a no-op and compat_release_entry() completes without crashing.
 *
 * Build:
 *   make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- CC=clang LLVM=1 \
 *        M=net/ipv6 modules
 * Load in QEMU:
 *   insmod /shared/goodaddr.ko
 *   dmesg | grep GOOD_KADDR
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netfilter/x_tables.h>

static bool dummy_match_fn(const struct sk_buff *skb,
			   struct xt_action_param *par)
{
	return true;
}

static struct xt_match dummy_match __read_mostly = {
	.name       = "goodaddr",
	.revision   = 0,
	.family     = NFPROTO_IPV4,
	.match      = dummy_match_fn,
	.matchsize  = 0,
	.me         = NULL,   /* built-in style: module_put(NULL) is a no-op */
};

static int __init goodaddr_init(void)
{
	int ret = xt_register_match(&dummy_match);
	if (ret) {
		pr_err("goodaddr: xt_register_match failed: %d\n", ret);
		return ret;
	}
	/* Print address for use as GOOD_KADDR_1 in poc_cysec.c */
	pr_info("GOOD_KADDR_1 = %px  (struct xt_match, me=NULL)\n",
		&dummy_match);
	return 0;
}

static void __exit goodaddr_exit(void)
{
	xt_unregister_match(&dummy_match);
}

module_init(goodaddr_init);
module_exit(goodaddr_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Prints a valid struct xt_match address for poc_cysec GOOD_KADDR test");
