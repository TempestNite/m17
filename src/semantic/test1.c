// test of compiler ability to parse code and process declarations in sequential and nested scopes

int
main( int argc , char ** argv )
{
	int c, d, e;

	c = d + e;
	{
		int	f, g;

		return f;
	}
	{
		int	h[10], i;

		return h[0] + f;
	}
	return 0;
}
