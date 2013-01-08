extern int main();

// this is where the assemble jumps to, so it needs the annotation
__attribute__((no_instrument_function))  void os_init(void) {
	main();
}
