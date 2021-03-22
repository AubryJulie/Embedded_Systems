/*
 * Copyright:
 * ----------------------------------------------------------------
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 *   (C) COPYRIGHT 2014, 2016 ARM Limited
 *       ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 * ----------------------------------------------------------------
 * File:     main.c
 * Release Information : Cortex-M3 DesignStart-r0p1-00rel0
 * ----------------------------------------------------------------
 *
 */

/*
 * --------Included Headers--------
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Xilinx specific headers
#include "xparameters.h"
#include "xgpio.h"
#include "EventRecorder.h" 

#include "m3_for_arty.h"        // Project specific header
#include "gpio.h"

#include "atomic.h"

// Encryption algorithm
#include "aes_128_CBC.h"

/*******************************************************************/

int main (void)
{

    // Define local variables
    int     status;
    int     i;
    int     readbackError;
   
    // BRAM base
    // Specify as volatile to ensure processor reads values back from BRAM
    // and not local storage
    volatile uint8_t *pBRAMmemory = (uint8_t *)XPAR_BRAM_0_BASEADDR;

    // CPU ID register
    volatile u32 *pCPUId = (u32 *)0xE000ED00;
    volatile u32 CPUId;
    volatile u32 alias_test;
    int          CPU_part;
		int					 CPU_rev;
		int					 CPU_var;
    char         CPU_name[20];

    // BRAM Data 
		// Text to encrypt
		uint8_t plain_text[64] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                      0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                      0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                      0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
		// Sbox function
		static const uint8_t sbox[256] = {
		//0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
			
		// Reverse Sbox function
			static const uint8_t rsbox[256] = {
		0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
		0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
		0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
		0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
		0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
		0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
		0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
		0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
		0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
		0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
		0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
		0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
		0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
		0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
		0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
		0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
			
		// The round constant word array.
		static const uint8_t Rcon[11] = {
		0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

		// The key
		uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

		// The initialization vector.
		uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
		
		// Encryption variables
		int crypt_data = 1;
		// BRAM Data sizes
	  int s_box_size;
		int rcon_size;
	  int iv_size;
	  int key_size;
		int plain_text_size;
		// For error check
		uint8_t read_data;
	  uint8_t cipher_data;
	  uint8_t plain_data;
		// Pointer to BRAM
		volatile uint8_t *S_BOX_INI = (uint8_t *)XPAR_BRAM_0_BASEADDR;
	  volatile uint8_t *RS_BOX_INI;
	  volatile uint8_t *RCON_INI;
		volatile uint8_t *IV_INI;
		volatile uint8_t *KEY_INI;
		volatile uint8_t *PLAIN_TEXT_INI;
		volatile uint8_t *CIPHER_TEXT_INI;
	  volatile uint8_t * END;
		// Encryption structure
	  struct AES_ctx ctx;

		// Initialise the Event Recorder
		EventRecorderInitialize(EventRecordAll,1);
		
    // Clear all interrupts
    NVIC_ClearAllPendingIRQ();
    
    // Initialise the GPIO
    status = InitialiseGPIO();
    if (status != XST_SUCCESS)  {
        print("Error - Xilinx GPIO failed to initialise\n");
    }
		
    // Enable GPIO Interrupts
    NVIC_EnableIRQ(GPIO0_IRQn);
    EnableGPIOInterrupts();

    // Read the CPU ID register to auto-detect the CPU and revision
    // Note however that code is compiled for a specific processor, so even though
    // the processor can be auto-detected, if the compiled code has extended commands not
    // supported by the processor, then runtime issues can occur
    CPUId    = *pCPUId;
    CPU_var  = ((CPUId & 0x00F00000) >> 20);
    CPU_part = ((CPUId & 0x0000FFF0) >> 4);
    CPU_rev  = CPUId & (0x0000000F);
    
    switch (CPU_part)
    {
      case 0xC21 : strcpy(  CPU_name, "Cortex-M1" ); break;
      case 0xC23 : strcpy(  CPU_name, "Cortex-M3" ); break;
      default    : sprintf( CPU_name, "Unknown %x", CPU_part );
    }
    

		// *****************************************************
    // Write BRAM data
    // *****************************************************
		// The printf have been commented for enabling fast test in hardware.
		// Printf( "Test the encryption of BRAM data using aes-128\r\n" );
		
		// Set the pointer
    pBRAMmemory = S_BOX_INI;
		
		// Write S_BOX to BRAM
		s_box_size = (sizeof(sbox)/sizeof(u8));
    for( i=0; i< s_box_size; i++)
		{
        *pBRAMmemory++ = sbox[i];
		}
		
		RS_BOX_INI = pBRAMmemory;
		
		// Check that no error occurs during the writing.
		// Reset the pointer
    pBRAMmemory = S_BOX_INI;

    // Readback
    for( i=0; i< s_box_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != sbox[i] )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - s_box readback corrupted.\r\n" );
    else
        printf( "s_box readback correct\r\n" );
		*/		
		// Write RS_BOX to BRAM
    for( i=0; i< s_box_size; i++)
		{
        *pBRAMmemory++ = rsbox[i];
		}
		
		RCON_INI = pBRAMmemory;
		
		// Check that no error occurs during the writing.
		// Reset the pointer
    pBRAMmemory = RS_BOX_INI;

    // Readback
    for( i=0; i< s_box_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != rsbox[i] )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - rs_box readback corrupted.\r\n" );
    else
        printf( "rs_box readback correct\r\n" );
		*/		
		// Write RCON to BRAM
		rcon_size = (sizeof(Rcon)/sizeof(u8));
    for( i=0; i< rcon_size; i++)
		{
        *pBRAMmemory++ = Rcon[i];
		}
		
		IV_INI = pBRAMmemory;
		
		// Check that no error occurs during the writing.
		// Reset the pointer
    pBRAMmemory = RCON_INI;

    // Readback
    for( i=0; i< rcon_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != Rcon[i] )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - Rcon readback corrupted.\r\n" );
    else
        printf( "Rcon readback correct\r\n" );
		*/		
		// write IV to BRAM
		iv_size = (sizeof(iv)/sizeof(u8));
    for( i=0; i< iv_size; i++)
		{
        *pBRAMmemory++ = iv[i];
		}
		
		KEY_INI = pBRAMmemory;
		
		// Check that no error occurs during the writing.
		// Reset the pointer
    pBRAMmemory = IV_INI;

    // Readback
    for( i=0; i< iv_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != iv[i] )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - iv readback corrupted.\r\n" );
    else
        printf( "iv readback correct\r\n" );
		*/		
		// write KEY to BRAM
		key_size = (sizeof(key)/sizeof(u8));
    for( i=0; i< key_size; i++)
		{
        *pBRAMmemory++ = key[i];
		}
		
		// Check that no error occurs during the writing.
		// Reset the pointer
    pBRAMmemory = KEY_INI;

    // Readback
    for( i=0; i< key_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != key[i] )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - Key readback corrupted.\r\n" );
    else
        printf( "Key readback correct\r\n" );
		*/		
		
		// Creation of the RoundKey
		AES_init_ctx_iv(&ctx, KEY_INI, IV_INI, RCON_INI, S_BOX_INI);
		
		PLAIN_TEXT_INI = KEY_INI + AES_keyExpSize;
		
		pBRAMmemory = PLAIN_TEXT_INI;
		
		// Write plain text to BRAM
	  plain_text_size = (sizeof(plain_text)/sizeof(u8));
    for( i=0; i < plain_text_size; i++)
		{
        *pBRAMmemory++ = plain_text[i];
		}
		
		CIPHER_TEXT_INI = pBRAMmemory;
		
		// Check that no error occurs during the writing.
    // Reset the pointer
    pBRAMmemory = PLAIN_TEXT_INI;

    // Readback
    for( i=0; i< plain_text_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != plain_text[i] )
        readbackError++;
    }
		
		/*
    if ( readbackError )
        printf( "ERROR - plaint text readback corrupted.\r\n" );
    else
        printf( "plaint text readback correct\r\n" );
		*/
		
		// Write copy of plain text to BRAM
    for( i=0; i< plain_text_size; i++)
		{
        *pBRAMmemory++ = plain_text[i];
		}
		
		END = pBRAMmemory;
		
		// Check that no error occurs during the writing.
    // Reset the pointer
    pBRAMmemory = CIPHER_TEXT_INI;

    // Readback
    for( i=0; i< plain_text_size; i++)
    {
			read_data = *pBRAMmemory++;
      if ( read_data != plain_text[i] )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - copy of plain text readback corrupted.\r\n" );
    else
        printf( "copy of plain text readback correct\r\n" );
		*/
		
		// *****************************************************
    // Test the encryption of BRAM data
    // *****************************************************
		
		// Encryption of the plain text
		AES_CBC_encrypt_buffer(&ctx, CIPHER_TEXT_INI, 64, S_BOX_INI);
		//printf( "encrypt done\r\n" );
		// Decryption of the cipher text
		AES_CBC_decrypt_buffer(&ctx, CIPHER_TEXT_INI, 64, RS_BOX_INI);
		//printf( "decrypt done\r\n" );
		
		// Check the encrypt-decrypt operation
		// Reset the pointer
		pBRAMmemory = PLAIN_TEXT_INI;
		
    for( i=0; i< plain_text_size; i++)
    {
			cipher_data = *(pBRAMmemory+plain_text_size);
			plain_data = *pBRAMmemory++;
      if ( cipher_data != plain_data )
        readbackError++;
    }
		/*
    if ( readbackError )
        printf( "ERROR - encryption/decryption failed.\r\n" );
    else
        printf( "encryption/decryption succeeded\r\n" );
		*/
		
		// End of initialization and test
		// Set the first led to indicate the end of initialization and test
		SetLedValue(1);
		
    // Main loop.  Handle LEDs and switches via interrupt
    while ( 1 )
    {
			// Encrypt or Decrypt data when a push button occurs
			if(button_pressed())
			{
				if(crypt_data)
				{
					// The second led is turned on when processing data
					SetLedValue(3);
					AES_CBC_encrypt_buffer(&ctx, CIPHER_TEXT_INI, 64, S_BOX_INI);
					// The second led is turned off when encryption is done
					// The third led is turned on when data are encrypted
					SetLedValue(5);
					crypt_data = 0;
				}
				else
				{
					// The second led is turned on when processing data
					// The third led is turned off when starting decryption
					SetLedValue(3);
					AES_CBC_decrypt_buffer(&ctx, CIPHER_TEXT_INI, 64, RS_BOX_INI);
					// The second led is turned off when decryption is done
					SetLedValue(1);
					crypt_data = 1;
				}
				reset_button();
			}
    }
}