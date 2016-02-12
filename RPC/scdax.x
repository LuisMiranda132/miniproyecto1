/* scdax.x */

const MAXLEN = 500;

typedef string mensaje<MAXLEN>;

struct to_encrypt {
	int key;
	mensaje text;
};

program SCDAXPROG {
	version SCDAXVERS {
		struct to_encrypt DOTHETHING(to_encrypt) = 1;
	} = 1;
} = 100;