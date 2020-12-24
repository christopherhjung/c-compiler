int (main(int a))
{
	struct S
	{
		int a;
	};
	struct S s;
	struct S (*sp);
	(a = (s.a));
	(s = (*sp));
	(((sp[0]).a) = a);
	(((*(&(sp[0]))).a) = 1);
	return a;
	return (s.a);
	return (sp->a);
}