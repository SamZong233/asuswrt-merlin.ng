/*
 * ABI shim for closed ASUS networkmap binaries built with BWDPI support.
 * Returning failure activates networkmap built-in client discovery fallback.
 */
int bwdpi_client_info(const void *mac, const void *ip, void *result)
{
	(void)mac;
	(void)ip;
	(void)result;
	return 0;
}
