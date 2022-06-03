#include <stdio.h>
#include <stdlib.h>


// characteristic
#define P 2
// size of original field
// (over which polynomial is taken) before extension
#define Q 2

// compute Q-cyclotomic cosets modulo (Q^T - 1)
#define T 6


// max degree of any polynomial
#define D 100

// (Subset of) Elements of Fq^t
// Will be be represented by polynomials of alpha (sums of powers of alpha)
// Coefficients are integers
// (but from Zp as p.a = 0 when p is the characteristic)
struct poly1{
    int deg;
    // coefficient of each term
    int terms[D];  // integer coefficients
};
// these are the elements of Fq^t obtained as coefficients of the
// polynomial (x - alpha)(x - alpha^q) ...


// Elements of Fq^t[x]
// Coefficients are elements of Fq^t
// Coefficients are "polynomials in alpha"
struct poly2{
    int deg;
    // coefficient of each term
    struct poly1 terms[D]; // polynomials in alpha
};

// check if a poly1 polynomial is 0
int is_zero(struct poly1 a)
{
    if(a.deg == 0 && a.terms[0] == 0) return 1;
    return 0;
}
// check if a poly1 polynomial is 1
int is_one(struct poly1 a)
{
    if(a.deg == 0 && a.terms[0] == 1) return 1;
    return 0;
}

// return a 0 poly1 polynomial
struct poly1 zero_poly1()
{
    struct poly1 ans;
    ans.deg = 0;
    ans.terms[0] = 0;
    return ans;
}
// return a 1 poly1 polynomial
struct poly1 one_poly1()
{
    struct poly1 ans;
    ans.deg = 0;
    ans.terms[0] = 1;
    return ans;
}
// return single negative alpha power polynomial
struct poly1 alpha_poly1(int p)
{
    struct poly1 ans;
    ans.deg = p;
    for(int i=0; i<p; i++) ans.terms[i] = 0;
    ans.terms[p] = -1;
    return ans;
}


// max nei ?
int max(int a, int b){ if(a >= b) return a; return b; }
int abs1(int a) { if(a < 0) return -a; return a; }

void show_poly1(struct poly1 a)
{
    if(is_zero(a))
    {
	printf("0");
	return;
    }

    int num_terms = 0;
    for(int i=0; i<=a.deg; i++)
	if(a.terms[i] != 0) num_terms++;

    // if(num_terms >= 2)
	printf("(");

    // printf("show poly: deg= %d\n", a.deg);
    for(int i=a.deg; i>=0; i--)
    {
	if(i == a.deg && a.terms[i] < 0)
	{
	    printf("- ");
	}
	else if(i != a.deg)
	{
	    if(a.terms[i] > 0) printf(" + ");
	    if(a.terms[i] < 0) printf(" - ");
	}
	if(a.terms[i] == 0) continue;

	if(i > 1)
	{
	    if(a.terms[i] > 1)
		printf("%da^%d", abs1(a.terms[i]), i);
	    else
	    printf("a^%d", i);
	}
	else if(i == 1)
	{
	    if(a.terms[i] > 1)
		printf("%da", abs1(a.terms[i]));
	    else
	    printf("a");
	}
	else
	{
	    printf("%d", abs1(a.terms[i]));
	}
    }

    // if(num_terms >= 2)
	printf(")");
}

void show_poly2(struct poly2 a)
{
    if(a.deg == 0 && is_zero(a.terms[0]))
    {
	printf("0");
	return;
    }

    // printf("show poly: deg= %d\n", a.deg);
    for(int i=a.deg; i>=0; i--)
    {
	if(i != a.deg && !is_zero(a.terms[i]))
	{
	    printf("+ ");
	}
	if(is_zero(a.terms[i])) continue;

	if(i > 1)
	{
	    if(!is_one(a.terms[i]))
	    {
		show_poly1(a.terms[i]);
		printf("x^%d ", i);
	    }
	    else
	    printf("x^%d ", i);
	}
	else if(i == 1)
	{
	    if(!is_one(a.terms[i]))
	    {
		show_poly1(a.terms[i]);
		printf("x ");
	    }
	    else
	    printf("x ");
	}
	else
	{
		show_poly1(a.terms[i]);
	}
    }
}



// add 2 poly1 polynomials
struct poly1 poly1_add(struct poly1 a, struct poly1 b)
{
    struct poly1 ans;
    int m = max(a.deg, b.deg);
    ans.deg = m;
    for(int i=0; i<=m; i++) ans.terms[i] = 0;

    for(int i=0; i<=m; i++)
    {
	/* printf("poly1_add-- terms:"); */
	/* for(int j=0; j<=36; j++) printf("%d ", ans.terms[j]); */
	/* printf("\n"); */

	ans.terms[i] = 0;

	if(a.deg >= i)
	    ans.terms[i] = ans.terms[i] + a.terms[i];

	if(b.deg >= i)
	    ans.terms[i] = ans.terms[i] + b.terms[i];

	ans.terms[i] = ans.terms[i] % P;
    }

    // adjust degree if terms reduce
    while(ans.terms[ans.deg] == 0 && ans.deg > 0) ans.deg--;


    return ans;
}


// shift a poly1 polynomial by s places to the left
// (multiply by alpha^s)
struct poly1 poly1_shift(struct poly1 a, int s)
{
    struct poly1 ans;
    ans.deg = a.deg + s;
    for(int i = a.deg + s; i>=0; i--)
    {
	if(i - s >= 0)
	    ans.terms[i] = a.terms[i - s];
	else
	    ans.terms[i] = 0;
    }

    return ans;
}

// reduce poly1 polynomial using the fact that x^6 + x + 1 = 0
// only works for this particular field, polynomial
struct poly1 reduce_poly1(struct poly1 a)
{
    for(int i=a.deg; i>=0; i--)
    {
	/* printf("terms1: "); */
	/* for(int j=0; j<=36; j++) printf("%d ", a.terms[j]); */
	/* printf("\n"); */

	if(i < 6) break;
	if(i > a.deg) continue; // a.deg will be modified in loop

	/* printf("reduce: %d\n", i); */
	/* show_poly1(a); */
	/* printf("\n"); */

	struct poly1 tmp = zero_poly1();

	// a^k = a^k-5 + a^k-6 for k > 6
	tmp.deg = i - 5;
	for(int j=0; j<=tmp.deg; j++) tmp.terms[j] = 0;
	tmp.terms[i - 5] = a.terms[i]; // 1 or -1
	tmp.terms[i - 6] = a.terms[i];

	// a.deg = a.deg - 1;
	a.terms[a.deg] = 0;
	while(a.terms[a.deg] == 0) a.deg--;

	/* printf("terms2: "); */
	/* for(int j=0; j<=36; j++) printf("%d ", a.terms[j]); */
	/* printf("\n"); */

	a = poly1_add(a, tmp);
	// after poly add a.terms[ anything > a.deg ] undefined hote pare

	/* printf("terms3: "); */
	/* for(int j=0; j<=36; j++) printf("%d ", a.terms[j]); */
	/* printf("\n"); */

	/* show_poly1(a); */
	/* printf("\n\n"); */
    }
    return a;
}


// multiply 2 poly1 polynomials
struct poly1 poly1_mul(struct poly1 a, struct poly1 b)
{
    
    struct poly1 ans;
    ans.deg = 0;
    ans.terms[0] = 0;

    for(int i=0; i<=a.deg; i++)
    {
	struct poly1 tmp = poly1_shift(b, i);

	for(int j=0; j<=tmp.deg; j++)
	{
	    tmp.terms[j] = tmp.terms[j] * a.terms[i];
	    tmp.terms[j] = tmp.terms[j] % P;
	}

	/* printf("mul-- tmp polynomial:\n"); */
	/* show_poly(tmp); */

	ans = poly1_add(ans, tmp);
    }
    return ans;
}

// ----------------------------------------------------------------------


// add 2 poly2 polynomials
struct poly2 poly2_add(struct poly2 a, struct poly2 b)
{
    struct poly2 ans;
    int m = max(a.deg, b.deg);
    ans.deg = m;

    for(int i=0; i<=m; i++)
    {
	ans.terms[i] = zero_poly1();

	if(a.deg >= i)
	    ans.terms[i] = poly1_add(ans.terms[i], a.terms[i]);

	if(b.deg >= i)
	    ans.terms[i] = poly1_add(ans.terms[i], b.terms[i]);

    }

    // adjust degree if terms reduce
    while(is_zero(ans.terms[ans.deg]) && ans.deg > 0) ans.deg--;

    return ans;
}


// shift a poly2 polynomial by s places to the left
// (multiply by x^s)
struct poly2 poly2_shift(struct poly2 a, int s)
{
    struct poly2 ans;
    ans.deg = a.deg + s;
    for(int i = a.deg + s; i>=0; i--)
    {
	if(i - s >= 0)
	    ans.terms[i] = a.terms[i - s];
	else
	    ans.terms[i] = zero_poly1();
    }

    return ans;
}



// multiply 2 poly2 polynomials
struct poly2 poly2_mul(struct poly2 a, struct poly2 b)
{
    
    struct poly2 ans;
    ans.deg = 0;
    ans.terms[0] = zero_poly1();

    for(int i=0; i<=a.deg; i++)
    {
	struct poly2 tmp = poly2_shift(b, i);

	for(int j=0; j<=tmp.deg; j++)
	{
	    tmp.terms[j] = poly1_mul(tmp.terms[j], a.terms[i]);
	    // tmp.terms[j] = tmp.terms[j] * a.terms[i];
	    // tmp.terms[i] = tmp.terms[i] % P;
	}

	/* printf("mul-- tmp polynomial:\n"); */
	/* show_poly(tmp); */

	ans = poly2_add(ans, tmp);
    }
    for(int i=0; i<=ans.deg; i++)
    {
	ans.terms[i] = reduce_poly1(ans.terms[i]);
    }
	
    return ans;
}



int exp1(int a, int b)
{
    if(b == 0) return 1;
    if(b == 1) return a;
    int ans = exp1(a*a, b/2);
    if(b&1) ans = ans * a;
    return ans;
}


// get the q cyclotomic coset modulo (q^t - 1)
// containing y
// returns number of elements in the coset
int cyclotomic(int q, int t, int y, int* coset)
{
    int start = y; // initial power of primitive element (the starting root)
    int curr = y;
    int idx = 0;
    int M = exp1(q, t) - 1;
    while(1)
    {
	// printf("cyclotomic y:%d  curr:%d\n", y, curr);
	// printf("%d  ", curr);
	coset[idx++] = curr;
	curr = curr * q; curr = curr % M;
	if(curr == start) break;
    }
    return idx;
}

// create polynomial from cyclotomic coset
void create_polynomial(int* coset, int size)
{
    struct poly2 ans;
    ans.deg = 0;
    ans.terms[0] = one_poly1();
    for(int i=0; i<size; i++)
    {
	struct poly2 tmp;
	tmp.deg = 1;
	tmp.terms[1] = one_poly1();
	tmp.terms[0] = alpha_poly1(coset[i]);

	/* printf("tmp: "); */
	/* show_poly2(tmp); */
	/* printf("\n"); */

	ans = poly2_mul(ans, tmp);

	/* printf("tmp-ans: "); */
	/* show_poly2(ans); */
	/* printf("\n"); */
    }
    printf(">> ");
    show_poly2(ans);
    printf("\n");
}


void test2()
{
    // array to mark which powers of primitive
    // element alpha have already been covered
    // in some coset
    int present[exp1(Q, T) -2 + 1];
    for(int i=0; i<=exp1(Q, T)-2; i++)
	present[i] = 0;

    int* curr_coset = (int*)malloc((exp1(Q, T)-1) * sizeof(int));
    // alpha^i gives all elements of Fq^t*
    for(int i=0; i<=exp1(Q, T) - 2; i++)
    {
	// printf("itr: %d\n", i);
	if(present[i]) continue;

	int size = cyclotomic(Q, T, i, curr_coset);

	// show coset
	printf("coset: ");
	for(int i=0; i<size; i++)
	{
	    printf("%d ", curr_coset[i]);
	    present[curr_coset[i]] = 1;
	}
	printf("\n");

	create_polynomial(curr_coset, size);
	printf("\n\n");
    }
    free(curr_coset);
}

void test1()
{
    /* cyclotomic(2, 4, 1); */
    /* printf("exp %d\n", exp1(2, 4)); */
    /* printf("exp %d\n", exp1(3, 4)); */

    struct poly1 a;
    a.deg = 2;
    a.terms[0] = 1;
    a.terms[1] = 0;
    a.terms[2] = 1;

    struct poly1 b;
    b.deg = 2;
    b.terms[0] = 1;
    b.terms[1] = 0;
    b.terms[2] = 1;

    struct poly1 c = poly1_mul(a, b);
    show_poly1(c);
}

void test3()
{
    struct poly1 a;

    a.deg = 54;
    for(int i=0; i<a.deg; i++) a.terms[i] = 0;
    a.terms[54] = 1;
    a.terms[45] = 1;
    a.terms[27] = 1;

    /* a.deg = 8; */
    /* for(int i=0; i<a.deg; i++) a.terms[i] = 0; */
    /* a.terms[8] = 1; */
    /* a.terms[7] = 1; */


    struct poly1 b = reduce_poly1(a);
    show_poly1(b);
}

int main()
{
    test2();

    printf("\n\n");
    return 0;
}
