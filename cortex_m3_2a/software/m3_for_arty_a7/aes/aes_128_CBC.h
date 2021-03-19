#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>
#include <stddef.h>

#define AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only

#define AES_KEYLEN 16   // Key length in bytes
#define AES_keyExpSize 176

struct AES_ctx
{
  volatile uint8_t *ROUNDKEY_INI;
  volatile uint8_t *Iv;
};

void AES_init_ctx_iv(struct AES_ctx* ctx, volatile uint8_t* KEY_INI, volatile uint8_t* IV_INI, volatile uint8_t *RCON_INI, volatile uint8_t *S_BOX_INI);

void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, volatile uint8_t* buf, size_t length, volatile uint8_t *S_BOX_INI);
void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, volatile uint8_t* buf, size_t length, volatile uint8_t *RS_BOX_INI);

#endif // _AES_H_