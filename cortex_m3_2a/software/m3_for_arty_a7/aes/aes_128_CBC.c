/*
This is an implementation of the AES algorithm CBC mode.
*/



/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include <string.h> // CBC mode, for memset
#include "aes_128_CBC.h"

/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4

#define Nk 4        // The number of 32 bit words in a key.
#define Nr 10       // The number of rounds in AES Cipher.

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
// state - array holding the intermediate results during decryption.
typedef uint8_t state_t[4][4];

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states. 
static void KeyExpansion(volatile uint8_t* ROUNDKEY_INI, volatile uint8_t *RCON_INI, volatile uint8_t *S_BOX_INI)
{
  unsigned i, j, k;
  uint8_t tempa[4]; // Used for the column/row operations

  // All other round keys are found from the previous round keys.
  for (i = Nk; i < Nb * (Nr + 1); ++i)
  {
    {
      k = (i - 1) * 4;
      tempa[0]=ROUNDKEY_INI[k + 0];
      tempa[1]=ROUNDKEY_INI[k + 1];
      tempa[2]=ROUNDKEY_INI[k + 2];
      tempa[3]=ROUNDKEY_INI[k + 3];

    }

    if (i % Nk == 0)
    {
      // Shifts the 4 bytes in a word to the left once.
      // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]
			const uint8_t u8tmp = tempa[0];
			tempa[0] = tempa[1];
			tempa[1] = tempa[2];
			tempa[2] = tempa[3];
			tempa[3] = u8tmp;

      // Takes a four-byte input word and 
      // applies the S-box to each of the four bytes to produce an output word.

			tempa[0] = S_BOX_INI[tempa[0]];
			tempa[1] = S_BOX_INI[tempa[1]];
			tempa[2] = S_BOX_INI[tempa[2]];
			tempa[3] = S_BOX_INI[tempa[3]];
		
      tempa[0] = tempa[0] ^ RCON_INI[i/Nk];
    }
    j = i * 4; k=(i - Nk) * 4;
    ROUNDKEY_INI[j + 0] = ROUNDKEY_INI[k + 0] ^ tempa[0];
    ROUNDKEY_INI[j + 1] = ROUNDKEY_INI[k + 1] ^ tempa[1];
    ROUNDKEY_INI[j + 2] = ROUNDKEY_INI[k + 2] ^ tempa[2];
    ROUNDKEY_INI[j + 3] = ROUNDKEY_INI[k + 3] ^ tempa[3];
  }
}

// Initialization of ROUNDKEY
void AES_init_ctx_iv(struct AES_ctx* ctx, volatile uint8_t* KEY_INI, volatile uint8_t *IV_INI, volatile uint8_t *RCON_INI, volatile uint8_t *S_BOX_INI)
{
	ctx->ROUNDKEY_INI = KEY_INI;
  KeyExpansion(ctx->ROUNDKEY_INI, RCON_INI, S_BOX_INI);
  ctx->Iv = IV_INI;
}

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
static void AddRoundKey(uint8_t round, state_t* state, const volatile uint8_t* RoundKey)
{
  uint8_t i,j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
    }
  }
}

// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void SubBytes(state_t* state, volatile uint8_t *S_BOX_INI)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = S_BOX_INI[(*state)[j][i]];
    }
  }
}

// The ShiftRows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
static void ShiftRows(state_t* state)
{
  uint8_t temp;

  // Rotate first row 1 columns to left  
  temp           = (*state)[0][1];
  (*state)[0][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[3][1];
  (*state)[3][1] = temp;

  // Rotate second row 2 columns to left  
  temp           = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp           = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to left
  temp           = (*state)[0][3];
  (*state)[0][3] = (*state)[3][3];
  (*state)[3][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[1][3];
  (*state)[1][3] = temp;
}

static uint8_t xtime(uint8_t x)
{
  return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}

// MixColumns function mixes the columns of the state matrix
static void MixColumns(state_t* state)
{
  uint8_t i;
  uint8_t Tmp, Tm, t;
  for (i = 0; i < 4; ++i)
  {  
    t   = (*state)[i][0];
    Tmp = (*state)[i][0] ^ (*state)[i][1] ^ (*state)[i][2] ^ (*state)[i][3] ;
    Tm  = (*state)[i][0] ^ (*state)[i][1] ; Tm = xtime(Tm);  (*state)[i][0] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][1] ^ (*state)[i][2] ; Tm = xtime(Tm);  (*state)[i][1] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][2] ^ (*state)[i][3] ; Tm = xtime(Tm);  (*state)[i][2] ^= Tm ^ Tmp ;
    Tm  = (*state)[i][3] ^ t ;              Tm = xtime(Tm);  (*state)[i][3] ^= Tm ^ Tmp ;
  }
}

// Multiply is used to multiply numbers in the field GF(2^8)
// Note: The last call to xtime() is unneeded, but often ends up generating a smaller binary
//       The compiler seems to be able to vectorize the operation better this way.
//       See https://github.com/kokke/tiny-AES-c/pull/34

#define Multiply(x, y)                                \
      (  ((y & 1) * x) ^                              \
      ((y>>1 & 1) * xtime(x)) ^                       \
      ((y>>2 & 1) * xtime(xtime(x))) ^                \
      ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^         \
      ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))   \

// MixColumns function mixes the columns of the state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
static void InvMixColumns(state_t* state)
{
  int i;
  uint8_t a, b, c, d;
  for (i = 0; i < 4; ++i)
  { 
    a = (*state)[i][0];
    b = (*state)[i][1];
    c = (*state)[i][2];
    d = (*state)[i][3];

    (*state)[i][0] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
    (*state)[i][1] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
    (*state)[i][2] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
    (*state)[i][3] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
  }
}


// The SubBytes Function Substitutes the values in the
// state matrix with values in an S-box.
static void InvSubBytes(state_t* state, volatile uint8_t *RS_BOX_INI)
{
  uint8_t i, j;
  for (i = 0; i < 4; ++i)
  {
    for (j = 0; j < 4; ++j)
    {
      (*state)[j][i] = RS_BOX_INI[(*state)[j][i]];
    }
  }
}

// The InvShiftRows() function shifts the rows in the state to the right.
static void InvShiftRows(state_t* state)
{
  uint8_t temp;

  // Rotate first row 1 columns to right  
  temp = (*state)[3][1];
  (*state)[3][1] = (*state)[2][1];
  (*state)[2][1] = (*state)[1][1];
  (*state)[1][1] = (*state)[0][1];
  (*state)[0][1] = temp;

  // Rotate second row 2 columns to right 
  temp = (*state)[0][2];
  (*state)[0][2] = (*state)[2][2];
  (*state)[2][2] = temp;

  temp = (*state)[1][2];
  (*state)[1][2] = (*state)[3][2];
  (*state)[3][2] = temp;

  // Rotate third row 3 columns to right
  temp = (*state)[0][3];
  (*state)[0][3] = (*state)[1][3];
  (*state)[1][3] = (*state)[2][3];
  (*state)[2][3] = (*state)[3][3];
  (*state)[3][3] = temp;
}

// Cipher is the main function that encrypts the PlainText.
static void Cipher(state_t* state, const volatile uint8_t* RoundKey, volatile uint8_t *S_BOX_INI)
{
  uint8_t round = 0;

  // Add the First round key to the state before starting the rounds.
  AddRoundKey(0, state, RoundKey);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr rounds are executed in the loop below.
  // Last one without MixColumns()
  for (round = 1; ; ++round)
  {
		// Replace the byte in state by the corresponding S-box value
    SubBytes(state, S_BOX_INI);
		
		// Shift the rows
    ShiftRows(state);
    if (round == Nr) {
      break;
    }
		
		// Strange mixing of column.
    MixColumns(state);
		
		// Add round key to the round
    AddRoundKey(round, state, RoundKey);
  }
  // Add round key to last round
  AddRoundKey(Nr, state, RoundKey);
}

// InvCipher is the main function that decrypts the CipherText.
static void InvCipher(state_t* state, const volatile uint8_t* RoundKey, volatile uint8_t *RS_BOX_INI)
{
  uint8_t round = 0;
	
  // Add the First round key to the state before starting the rounds.
  AddRoundKey(Nr, state, RoundKey);

  // There will be Nr rounds.
  // The first Nr-1 rounds are identical.
  // These Nr rounds are executed in the loop below.
  // Last one without InvMixColumn()
  for (round = (Nr - 1); ; --round)
  {
		// Shift rows
    InvShiftRows(state);
		
		// Replace the byte in state by the corresponding reverse S-box value.
    InvSubBytes(state, RS_BOX_INI);

		// Add the round key to the rounds.
    AddRoundKey(round, state, RoundKey);
    if (round == 0) {
      break;
    }
	
		// Strange mixing of column.
    InvMixColumns(state);
		
  }

}

// Xor between buf and Iv
static void XorWithIv( volatile uint8_t* buf, const volatile uint8_t* Iv)
{
  uint8_t i;
  for (i = 0; i < AES_BLOCKLEN; ++i) // The block in AES is always 128bit no matter the key size
  {
    buf[i] ^= Iv[i];
  }
}


void AES_CBC_encrypt_buffer(struct AES_ctx *ctx, volatile uint8_t* buf, size_t length, volatile uint8_t *S_BOX_INI)
{
  size_t i;
  volatile uint8_t *Iv = ctx->Iv;
  for (i = 0; i < length; i += AES_BLOCKLEN)
  {
	 // Make a XOR with the Iv for the first row of buf.
    XorWithIv(buf, Iv);
		
		// Encryption
    Cipher((state_t*)buf, ctx->ROUNDKEY_INI, S_BOX_INI);
    Iv = buf;
		
		// Go for the next row of the buf.
    buf += AES_BLOCKLEN;
  }
}

void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, volatile uint8_t* buf, size_t length, volatile uint8_t *RS_BOX_INI)
{
  size_t i;
	size_t j;
  uint8_t storeNextIv[AES_BLOCKLEN];
	uint8_t storePrevIv[AES_BLOCKLEN];
	for (j = 0;j< AES_BLOCKLEN;j++)
	{
		storePrevIv[j] = ctx->Iv[j];
	}
  for (i = 0; i < length; i += AES_BLOCKLEN)
  {
		for (j = 0;j< AES_BLOCKLEN;j++)
		{
			storeNextIv[j] = buf[j];
		}
		
		// Decryption
    InvCipher((state_t*)buf, ctx->ROUNDKEY_INI, RS_BOX_INI);
		
		//Make a XOR with the Iv for the first row of buf.
    XorWithIv(buf, storePrevIv);
		
		// Go for the next row of the buf.
    buf += AES_BLOCKLEN;
		
		for (j = 0;j< AES_BLOCKLEN;j++)
		{
			storePrevIv[j] = storeNextIv[j];
		}
		
  }
}
