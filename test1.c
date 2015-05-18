
int main(int argc, char **argv)
{
	char *s;

	s = malloc(64);
	strcpy(s, "abcd");
	printf("%s", s);
	free(s);
}
