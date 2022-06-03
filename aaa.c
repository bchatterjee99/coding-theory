#include <stdio.h>


// characteristic
#define P 2
// size of original field
// (over which polynomial is taken) before extension
#define Q 2

// compute Q-cyclotomic cosets modulo (Q^T - 1)
#define T 4


// max degree of any polynomial
#define D 50

// Elements of Fq^t
// Will be be represented by polynomials of alpha
// Integers as coefficients
// (but from Zp as p.a = 0 when p is the characteristic)
struct poly1{
    int deg;
    // coefficient of each term
    int terms[D];  // integers representing elements of Zp
};


// Elements of Fq[x]
// "polynomials in alpha" as coefficients
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
// return a 0 poly1 polynomial
struct poly1 zero_poly1()
{
    struct poly1 ans;
    ans.deg = 0;
    ans.temrs[0] = 0;
    return ans;
}

void show_poly(struct poly1 a)
{
    if(is_zero(a))
    {
	printf("0");
	return;
    }

    // printf("show poly: deg= %d\n", a.deg);
    for(int i=a.deg; i>=0; i--)
    {
	if(i != a.deg || a.terms[i] < 0)
	{
	    if(a.terms[i] > 0) printf("+ ");
	    if(a.terms[i] < 0) printf("- ");
	}
	if(a.terms[i] == 0) continue;

	if(i > 1)
	{
	    if(a.terms[i] > 1)
	    printf("%da^%d ", a.terms[i], i);
	    else
	    printf("a^%d ", i);
	}
	else if(i == 1)
	{
	    if(a.terms[i] > 1)
	    printf("%da ", a.terms[i], i);
	    else
	    printf("a ", i);
	}
	else
	{
	    printf("%d", a.terms[i]);
	}
    }
}


// max nei ?
int max(int a, int b){ if(a >= b) return a; return b; }

// add 2 poly1 polynomials
struct poly1 poly1_add(struct poly1 a, struct poly1 b)
{
    struct poly1 ans;
    int m = max(a.deg, b.deg);
    ans.deg = m;

    for(int i=0; i<=m; i++)
    {
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
	    tmp.terms[i] = tmp.terms[i] * a.terms[i];
	    tmp.terms[i] = tmp.terms[i] % P;
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
	    tmp.terms[i] = tmp.terms[i] * a.terms[i];
	    tmp.terms[i] = tmp.terms[i] % P;
	}

	/* printf("mul-- tmp polynomial:\n"); */
	/* show_poly(tmp); */

	ans = poly2_add(ans, tmp);
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
	printf("%d  ", curr);
	coset[idx++] = curr;
	curr = curr * q; curr = curr % M;
	if(curr == start) break;
    }
    return idx;
}

void test1()
{
    /* cyclotomic(2, 4, 1); */
    /* printf("exp %d\n", exp1(2, 4)); */
    /* printf("exp %d\n", exp1(3, 4)); */

    struct poly1 a;
    a.deg = 2;
    a.terms[0] = 1;
    a.terms[1] = 1;
    a.terms[2] = 1;

    struct poly1 b;
    b.deg = 2;
    b.terms[0] = 1;
    b.terms[1] = 0;
    b.terms[2] = 1;

    struct poly1 c = poly1_mul(a, b);
    show_poly(c);
}

int main()
{
    test1();

    printf("\n\n");
    return 0;
}
