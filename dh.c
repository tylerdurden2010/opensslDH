#include<stdio.h>
#include<openssl/dh.h>
#include<openssl/ec.h>
#include<openssl/evp.h>
#include<openssl/err.h>

void handleErrors()
{
    printf("error\n ");
    exit(1);
}

int main()
{
DH *privkey;
int codes;
int secret_size;


/* Generate the parameters to be used */
if(NULL == (privkey = DH_new())) handleErrors();

int size;



if(1 != DH_generate_parameters_ex(privkey, 512, DH_GENERATOR_2, NULL)) handleErrors();
size = DH_size(privkey);

if(1 != DH_check(privkey, &codes)) handleErrors();
if(codes != 0)
{
    /* Problems have been found with the generated parameters */
    /* Handle these here - we'll just abort for this example */
    printf("DH_check failed\n");
    abort();
}


/* Generate the public and private key pair */
if(1 != DH_generate_key(privkey)) handleErrors();


const BIGNUM *p = NULL, *q = NULL, *g = NULL, *pub = NULL, *pri = NULL;
DH_get0_pqg(privkey, &p, &q, &g);
DH_get0_key(privkey, &pub, &pri);

//int a = sizeof(unsigned char) * (DH_size(privkey));


DH *dhkey;
dhkey = DH_new();
DH_set0_pqg(dhkey,p,NULL,g);
DH_generate_key(dhkey);


const BIGNUM *p1 = NULL, *q1=NULL, *g1=NULL, *pub1=NULL,*pri1=NULL;
DH_get0_key(dhkey,&pub1,&pri1);



/* Send the public key to the peer.
 *  * How this occurs will be specific to your situation (see main text below) */


/* Receive the public key from the peer. In this example we're just hard coding a value */
BIGNUM *pubkey = pub1;



/* Compute the shared secret */
unsigned char *secret;
unsigned char *secret2;
if(NULL == (secret = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(privkey))))) handleErrors();
if(NULL == (secret2 = OPENSSL_malloc(sizeof(unsigned char) * (DH_size(dhkey))))) handleErrors();

if(0 > (secret_size = DH_compute_key(secret, pub1, privkey))) handleErrors();
if(0 > (secret_size = DH_compute_key(secret2, pub, dhkey))) handleErrors();

/* Do something with the shared secret */
/* Note secret_size may be less than DH_size(privkey) */
BIO_dump_fp(stdout, secret, secret_size);
BIO_dump_fp(stdout, secret2, secret_size);
//BIO_dump_fp(stdout, ,);

/* Clean up */
OPENSSL_free(secret);
BN_free(pubkey);
DH_free(privkey);
}
